import torch
import torch.nn as nn

class DiceLoss(nn.Module):
    def __init__(self, smooth=1e-6):
        super().__init__()
        self.smooth = smooth

    def forward(self, pred, target):
        pred = pred.view(-1)
        target = target.view(-1)
        intersection = (pred * target).sum()
        dice = (2. * intersection + self.smooth) / (pred.sum() + target.sum() + self.smooth)
        return 1 - dice

class FocalLoss(nn.Module):
    def __init__(self, alpha=0.25, gamma=2.0):
        super().__init__()
        self.alpha = alpha
        self.gamma = gamma

    def forward(self, pred, target):
        pred = pred.clamp(1e-6, 1 - 1e-6)
        bce = -(target * torch.log(pred) + (1 - target) * torch.log(1 - pred))
        focal = self.alpha * (1 - pred) ** self.gamma * bce
        return focal.mean()

def get_loss(name, device):
    if name == "bce":
        return nn.BCELoss()
    elif name == "bce_logits":
        pos_weight = torch.tensor([5.0]).to(device)
        return nn.BCEWithLogitsLoss(pos_weight=pos_weight)
    elif name == "dice":
        return DiceLoss()
    elif name == "focal":
        return FocalLoss()
    else:
        raise ValueError(f"Loss '{name}' not recognized.")
   