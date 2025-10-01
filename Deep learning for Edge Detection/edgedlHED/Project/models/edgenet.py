import torch
import torch.nn as nn
import torchvision.models as models
from torchvision.models import VGG16_Weights

class EdgeNet(torch.nn.Module):
    def __init__(self):
        super(EdgeNet, self).__init__()

        #deprecated 
        #vgg = models.vgg16(pretrained=True).features
        #vgg = models.vgg16(weights=VGG16_Weights.IMAGENET1K_V1).features
        #or
        vgg = models.vgg16(weights=VGG16_Weights.DEFAULT).features
        self.stage1 = torch.nn.Sequential(*vgg[:5])   # Conv1_1 to Conv2_2
        self.stage2 = torch.nn.Sequential(*vgg[5:10]) # Conv3_1 to Conv3_3
        self.stage3 = torch.nn.Sequential(*vgg[10:17])# Conv4_1 to Conv4_3
        self.stage4 = torch.nn.Sequential(*vgg[17:24])# Conv5_1 to Conv5_3

        self.side2 = nn.Conv2d(128, 1, kernel_size=1)
        self.side3 = nn.Conv2d(256, 1, kernel_size=1)
        self.side4 = nn.Conv2d(512, 1, kernel_size=1)

        # Simple fuse
        #self.fuse = nn.Conv2d(3, 1, kernel_size=1)

        # Alternative fuse with more layers
        self.fuse = nn.Sequential(
            nn.Conv2d(3, 8, kernel_size=3, padding=1),
            nn.ReLU(),
            nn.Conv2d(8, 1, kernel_size=1)
        )

    def forward(self, x):
        h1 = self.stage1(x)
        h2 = self.stage2(h1)
        h3 = self.stage3(h2)
        h4 = self.stage4(h3)

        s2 = torch.sigmoid(self.side2(h2))
        s3 = torch.sigmoid(self.side3(h3))
        s4 = torch.sigmoid(self.side4(h4))

        s2_up = nn.functional.interpolate(s2, size=x.size()[2:], mode='bilinear', align_corners=False)
        s3_up = nn.functional.interpolate(s3, size=x.size()[2:], mode='bilinear', align_corners=False)
        s4_up = nn.functional.interpolate(s4, size=x.size()[2:], mode='bilinear', align_corners=False)

        # Alterntive 1
        #fuse = torch.sigmoid(self.fuse(torch.cat((s2_up, s3_up, s4_up), 1)))

        # Alternative 2
        fuse_input = torch.cat((s2_up, s3_up, s4_up), dim=1)
        fuse = self.fuse(fuse_input)
        fuse = torch.sigmoid(fuse)

        return [s2_up, s3_up, s4_up, fuse]