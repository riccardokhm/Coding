import os
import argparse
import torch
import torch.nn as nn
from torch.utils.data import DataLoader
from tqdm import tqdm
from models.edgenet import EdgeNet
from utils.dataset import EdgeDataset
from utils.losses import get_loss
from utils.metrics_contour import compute_metrics
import matplotlib.pyplot as plt
from datetime import datetime
from torch.utils.tensorboard import SummaryWriter

# Config
timestamp = datetime.now().strftime("run_%Y-%m-%d_%H-%M")
DATASET_PATH = os.path.abspath(os.path.join(os.path.dirname(__file__), "..", "Dataset", "data"))
DEBUG_PATH = os.path.join(os.path.dirname(__file__), "debug", timestamp)
CHECKPOINT_PATH = os.path.join(os.path.dirname(__file__), "checkpoints", timestamp)

def save_edge_maps(img, mask, outputs, epoch, batch_idx, debug_path):
    os.makedirs(debug_path, exist_ok=True)
    img_np = img[0].cpu().numpy().transpose(1, 2, 0)
    img_np = (img_np * 0.5) + 0.5
    img_np = img_np.clip(0, 1)

    mask_np = mask[0][0].cpu().numpy()
    fused_np = outputs[-1][0][0].detach().cpu().numpy()

    plt.imsave(os.path.join(debug_path, f"epoch{epoch}_batch{batch_idx}_input.png"), img_np)
    plt.imsave(os.path.join(debug_path, f"epoch{epoch}_batch{batch_idx}_gt_mask.png"), mask_np, cmap='gray')
    plt.imsave(os.path.join(debug_path, f"epoch{epoch}_batch{batch_idx}_fused.png"), fused_np, cmap='gray')

    edge_names = ["side2", "side3", "side4"]
    for i, edge in enumerate(outputs[:-1]):
        edge_np = edge[0][0].detach().cpu().numpy()
        plt.imsave(os.path.join(debug_path, f"epoch{epoch}_batch{batch_idx}_{edge_names[i]}.png"), edge_np, cmap='gray')

def train(debug=False):
    # Hyperparameters
    PATIENCE = 3
    wait = 0
    BATCH_SIZE = 4
    EPOCHS = 30
    LR = 1e-4
    DEVICE = torch.device("cuda" if torch.cuda.is_available() else "cpu")
    best_val_loss = float('inf')

    # Folder setup
    os.makedirs(DEBUG_PATH, exist_ok=True) if debug else None
    os.makedirs(CHECKPOINT_PATH, exist_ok=True)

    # Dataset
    train_set = EdgeDataset(os.path.join(DATASET_PATH, "train", "images"),
                            os.path.join(DATASET_PATH, "train", "masks"),
                            size=(321, 481), augment=True)
    val_set = EdgeDataset(os.path.join(DATASET_PATH, "val", "images"),
                          os.path.join(DATASET_PATH, "val", "masks"),
                          size=(321, 481), augment=False)

    train_loader = DataLoader(train_set, batch_size=BATCH_SIZE, shuffle=True)
    val_loader = DataLoader(val_set, batch_size=BATCH_SIZE, shuffle=False)

    # Model and optimizer
    model = EdgeNet().to(DEVICE)
    optimizer = torch.optim.Adam(model.parameters(), lr=LR)

    # Loss selection
    available_losses = ["bce", "bce_logits", "dice", "focal"]
    print("\n📘 Seleziona la funzione di loss:")
    for i, name in enumerate(available_losses):
        print(f"{i+1}. {name}")
    choice = input("➡️ Inserisci il numero della loss: ")
    loss_name = available_losses[int(choice)-1]
    print(f"✅ Hai selezionato: {loss_name}\n")
    loss_fn = get_loss(loss_name, DEVICE)

    writer = SummaryWriter(log_dir=os.path.join("runs", f"{loss_name}_{timestamp}"))

    for epoch in range(EPOCHS):
        model.train()
        epoch_loss = 0
        loop = tqdm(train_loader, desc=f"Epoch {epoch+1}/{EPOCHS}")

        for batch_idx, (img, mask) in enumerate(loop):
            img, mask = img.to(DEVICE), mask.to(DEVICE)
            outputs = model(img)
            side_loss = sum([loss_fn(out, mask) for out in outputs[:-1]])
            fused_loss = loss_fn(outputs[-1], mask)
            loss = 0.5 * side_loss + 1.0 * fused_loss

            optimizer.zero_grad()
            loss.backward()
            optimizer.step()

            epoch_loss += loss.item()
            loop.set_postfix(loss=loss.item())

            if debug and batch_idx == 0:
                save_edge_maps(img, mask, outputs, epoch+1, batch_idx, debug_path=DEBUG_PATH + f"_loss_{loss_name}")

        print(f"✅ Epoch {epoch+1} completed. Avg loss: {epoch_loss / len(train_loader):.4f}")

        # Validation
        model.eval()
        val_loss = 0
        side_names = ["side2", "side3", "side4", "fused"]
        all_preds = [[] for _ in range(4)]
        all_gts = []
        fused_means = []
        mask_means = []

        with torch.no_grad():
            for img, mask in val_loader:
                img, mask = img.to(DEVICE), mask.to(DEVICE)
                outputs = model(img)

                val_loss += sum([loss_fn(out, mask) for out in outputs]).item()

                for i, out in enumerate(outputs):
                    all_preds[i].extend(out.cpu())
                all_gts.extend(mask.cpu())

                fused_means.append(outputs[-1].mean().item())
                mask_means.append(mask.mean().item())

        val_loss /= len(val_loader)
        avg_fused_mean = sum(fused_means) / len(fused_means)
        avg_mask_mean = sum(mask_means) / len(mask_means)

        print(f"📉 Validation loss: {val_loss:.4f}")
        writer.add_scalar("Loss/val", val_loss, epoch)
        writer.add_scalar("Fused/mean", avg_fused_mean, epoch)
        writer.add_scalar("Mask/mean", avg_mask_mean, epoch)

        # Metric evaluation
        for i in range(4):
            metrics = compute_metrics(all_preds[i], all_gts)
            print(f"📐 {side_names[i]} → ODS: {metrics['ODS']:.4f} | OIS: {metrics['OIS']:.4f} | AP: {metrics['AP']:.4f}")
            writer.add_scalar(f"{side_names[i]}/ODS", metrics["ODS"], epoch)
            writer.add_scalar(f"{side_names[i]}/OIS", metrics["OIS"], epoch)
            writer.add_scalar(f"{side_names[i]}/AP", metrics["AP"], epoch)

        # Checkpointing
        if val_loss < best_val_loss:
            best_val_loss = val_loss
            wait = 0
            torch.save(model.state_dict(), os.path.join(CHECKPOINT_PATH, "edgenet_best.pt"))
        else:
            wait += 1
            if wait >= PATIENCE:
                print("⏸️ Early stopping triggered.")
                break

if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument("--debug", action="store_true", help="Salva gli edge maps durante il training")
    args = parser.parse_args()
    train(debug=args.debug)