import os
from PIL import Image
from torch.utils.data import Dataset
import torchvision.transforms as transforms
import torchvision.transforms.functional as TF
import random

class EdgeDataset(Dataset):
    def __init__(self, image_dir, mask_dir, size=(321, 481), augment=False):
        self.image_paths = sorted([
            os.path.join(image_dir, f) for f in os.listdir(image_dir) if f.endswith('.jpg')
        ])
        self.mask_paths = sorted([
            os.path.join(mask_dir, f) for f in os.listdir(mask_dir) if f.endswith('.png')
        ])
        self.augment = augment
        self.image_transform = transforms.Compose([
            transforms.Resize(size),
            transforms.ToTensor(),  # Converts to [0,1] and shape [C,H,W]
            transforms.Normalize(mean=[0.5]*3, std=[0.5]*3)  # Optional: center around 0
        ])
        self.mask_transform = transforms.Compose([
            transforms.Resize(size),
            transforms.ToTensor()  # Converts to [0,1] and shape [1,H,W]
        ])

    def __len__(self):
        return len(self.image_paths)

    def apply_augmentations(self, img, mask):
        if random.random() > 0.5:
            img = TF.hflip(img)
            mask = TF.hflip(mask)
        if random.random() > 0.5:
            angle = random.uniform(-15, 15)
            img = TF.rotate(img, angle)
            mask = TF.rotate(mask, angle)
        if random.random() > 0.5:
            img = TF.adjust_brightness(img, brightness_factor=random.uniform(0.8, 1.2))
            img = TF.adjust_contrast(img, contrast_factor=random.uniform(0.8, 1.2))
        return img, mask

    def __getitem__(self, idx):
        img = Image.open(self.image_paths[idx]).convert("RGB")
        mask = Image.open(self.mask_paths[idx]).convert("L")
        if self.augment:
            img, mask = self.apply_augmentations(img, mask)

        img = self.image_transform(img)
        mask = self.mask_transform(mask)
        mask = (mask > 0.5).float()  # Forcing binarize mask

        return img, mask