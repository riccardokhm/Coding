import os
import argparse
import torch
import torch.nn as nn
from torch.utils.data import DataLoader
from tqdm import tqdm
from models.edgenet import EdgeNet
from utils.dataset import EdgeDataset
from utils.losses import get_loss
import matplotlib.pyplot as plt
from datetime import datetime
from torch.utils.tensorboard import SummaryWriter
from utils.metrics import compute_iou, compute_f1

# Config
timestamp = datetime.now().strftime("run_%Y-%m-%d_%H-%M")

DATASET_PATH = os.path.abspath(os.path.join(os.path.dirname(__file__), "..", "Dataset", "data"))
#DEBUG_PATH = os.path.join(os.path.dirname(__file__), "debug")
DEBUG_PATH = os.path.join(os.path.dirname(__file__), "debug", timestamp)
CHECKPOINT_PATH = os.path.join(os.path.dirname(__file__), "checkpoints", timestamp)

def save_edge_maps(img, mask, outputs, epoch, batch_idx, debug_path):
    img_np = img[0].cpu().numpy().transpose(1, 2, 0)  # [H,W,C]
    img_np = (img_np * 0.5) + 0.5  # Denormalizza da [-1,1] a [0,1]
    img_np = img_np.clip(0, 1)

    mask_np = mask[0][0].cpu().numpy()
    fused_np = outputs[-1][0][0].detach().cpu().numpy()

    # Overlay
    #overlay = img_np.copy()
    #overlay[..., 0] += fused_np * 0.6  # canale rosso
    #overlay = overlay.clip(0, 1)

    # Savings
    plt.imsave(os.path.join(debug_path, f"epoch{epoch}_batch{batch_idx}_input.png"), img_np)
    plt.imsave(os.path.join(debug_path, f"epoch{epoch}_batch{batch_idx}_gt_mask.png"), mask_np, cmap='gray')
    plt.imsave(os.path.join(debug_path, f"epoch{epoch}_batch{batch_idx}_fused.png"), fused_np, cmap='gray')
    #plt.imsave(os.path.join(debug_path, f"epoch{epoch}_batch{batch_idx}_overlay.png"), overlay)
    
    # Side outputs
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

    # Debug folder
    if debug and not os.path.exists(DEBUG_PATH):
        os.makedirs(DEBUG_PATH)
    # Checkpoint folder
    if not os.path.exists(CHECKPOINT_PATH):
        os.makedirs(CHECKPOINT_PATH)
    
    train_set = EdgeDataset(
    os.path.join(DATASET_PATH, "train", "images"),
    os.path.join(DATASET_PATH, "train", "masks"),
    size=(321, 481),
    augment=True
    )
    val_set = EdgeDataset(
        os.path.join(DATASET_PATH, "val", "images"),
        os.path.join(DATASET_PATH, "val", "masks"),
        size=(321, 481),
        augment=False
    )

    train_loader = DataLoader(train_set, batch_size=BATCH_SIZE, shuffle=True)
    val_loader = DataLoader(val_set, batch_size=BATCH_SIZE, shuffle=False)

    model = EdgeNet().to(DEVICE)
    optimizer = torch.optim.Adam(model.parameters(), lr=LR)

    # Loss function selection
    available_losses = ["bce", "bce_logits", "dice", "focal"]
    print("\nSelect loss function:")
    for i, name in enumerate(available_losses):
        print(f"{i+1}. {name}")

    choice = input("Enter number for loss: ")
    loss_name = available_losses[int(choice)-1]
    print(f"Selected: {loss_name}\n")

    loss_fn = get_loss(loss_name, DEVICE)

    #writer = SummaryWriter(log_dir=os.path.join("runs", timestamp))
    writer = SummaryWriter(log_dir=os.path.join("runs", f"{loss_name}_{timestamp}"))

    #loss_fn = nn.BCELoss()
    #loss_fn = nn.BCEWithLogitsLoss(pos_weight=torch.tensor([5.0]).to(DEVICE))  # Bilancia il peso dei pixel di bordo

    for epoch in range(EPOCHS):
        model.train()
        epoch_loss = 0
        loop = tqdm(train_loader, desc=f"Epoch {epoch+1}/{EPOCHS}")

        for batch_idx, (img, mask) in enumerate(loop):
            img, mask = img.to(DEVICE), mask.to(DEVICE)

            outputs = model(img)  # [s2, s3, s4, fuse]
            #loss = sum([loss_fn(out, mask) for out in outputs])
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

        print(f"Epoch {epoch+1} completed. Avg loss: {epoch_loss / len(train_loader):.4f}")

        # Evaluation on validation set
        model.eval()
        val_loss = 0
        iou_total = 0
        f1_total = 0
        with torch.no_grad():
            for img, mask in val_loader:
                img, mask = img.to(DEVICE), mask.to(DEVICE)
                outputs = model(img)
                fused = outputs[-1]

                val_loss += sum([loss_fn(out, mask) for out in outputs]).item()
                iou_total += compute_iou(fused, mask)
                f1_total += compute_f1(fused, mask)

        val_loss /= len(val_loader)
        iou_score = iou_total / len(val_loader)
        f1_score = f1_total / len(val_loader)

        print(f"📉 Validation loss: {val_loss:.4f} | IoU: {iou_score:.4f} | F1: {f1_score:.4f}")

        writer.add_scalar("Loss/val", val_loss, epoch)
        writer.add_scalar("IoU/val", iou_score, epoch)
        writer.add_scalar("F1/val", f1_score, epoch)

        writer.add_scalar("Mask/mean", mask.mean().item(), epoch)
        writer.add_scalar("Fused/mean", fused.mean().item(), epoch)

        # Saving checkpoint every epoch
        #torch.save(model.state_dict(), os.path.join(CHECKPOINT_PATH, f"edgenet_epoch{epoch+1}.pt"))

        # Saving best model based on validation loss
        if val_loss < best_val_loss:
            best_val_loss = val_loss
            wait = 0
            torch.save(model.state_dict(), os.path.join(CHECKPOINT_PATH, "edgenet_best.pt"))
        else:
            # Early stopping
            wait += 1
            if wait >= PATIENCE:
                print("Early stopping triggered.")
                break
            

if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument("--debug", action="store_true", help="Salva gli edge maps durante il training")
    args = parser.parse_args()

    train(debug=args.debug)