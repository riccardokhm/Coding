import torch
import numpy as np

def compute_metrics(preds, gts, thresholds=torch.linspace(0.01, 0.99, steps=99)):
    """
    Calculate ODS, OIS e AP for a list of predictions and ground truth.
    Args:
        preds: tensor list [B, 1, H, W] with values in [0,1]
        gts: binary tensor list [B, 1, H, W]
        thresholds: test threshold tensor
    Returns:
        dict with 'ODS', 'OIS', 'AP'
    """
    eps = 1e-6
    all_precisions = []
    all_recalls = []
    all_f1s = []

    # For each threshold, compute global precision/recall
    for t in thresholds:
        tp, fp, fn = 0, 0, 0
        for pred, gt in zip(preds, gts):
            pred_bin = (pred > t).float()
            gt = gt.float()

            tp += (pred_bin * gt).sum().item()
            fp += (pred_bin * (1 - gt)).sum().item()
            fn += ((1 - pred_bin) * gt).sum().item()

        precision = tp / (tp + fp + eps)
        recall = tp / (tp + fn + eps)
        f1 = 2 * precision * recall / (precision + recall + eps)

        all_precisions.append(precision)
        all_recalls.append(recall)
        all_f1s.append(f1)

    # ODS: maximum F1 across all thresholds
    ods = max(all_f1s)

    # OIS: maximum F1 per image
    f1_per_image = []
    for pred, gt in zip(preds, gts):
        best_f1 = 0
        for t in thresholds:
            pred_bin = (pred > t).float()
            gt = gt.float()

            tp = (pred_bin * gt).sum().item()
            fp = (pred_bin * (1 - gt)).sum().item()
            fn = ((1 - pred_bin) * gt).sum().item()

            precision = tp / (tp + fp + eps)
            recall = tp / (tp + fn + eps)
            f1 = 2 * precision * recall / (precision + recall + eps)
            best_f1 = max(best_f1, f1)

        f1_per_image.append(best_f1)

    ois = np.mean(f1_per_image)

    # AP: area under the precision-recall curve also called AUC
    sorted_pairs = sorted(zip(all_recalls, all_precisions))
    recalls_sorted, precisions_sorted = zip(*sorted_pairs)
    ap = np.trapz(precisions_sorted, recalls_sorted)

    return {"ODS": ods, "OIS": ois, "AP": ap}