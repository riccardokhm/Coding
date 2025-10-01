import os
import numpy as np
import cv2
from scipy.io import loadmat

def extract_edges(mat_path):

    data = loadmat(mat_path)
    gt = data['groundTruth'][0]
    edge_maps = [gt[i][0][0]["Boundaries"] for i in range(len(gt))]
    avg_edge = np.mean(np.stack(edge_maps), axis=0)
    binary_edge = (avg_edge > 0.5).astype(np.uint8) * 255
    return binary_edge

def convert_split(split):

    BASE_DATASET_PATH = os.path.join(os.path.dirname(__file__), "..", "Dataset")

    mat_dir = os.path.join(BASE_DATASET_PATH, "raw_data", "ground_truth", split)
    img_dir = os.path.join(BASE_DATASET_PATH, "raw_data", "images", split)
    out_img_dir = os.path.join(BASE_DATASET_PATH, "data", split, "images")
    out_mask_dir = os.path.join(BASE_DATASET_PATH, "data", split, "masks")

    os.makedirs(out_img_dir, exist_ok=True)
    os.makedirs(out_mask_dir, exist_ok=True)

    for fname in os.listdir(mat_dir):
        if not fname.endswith('.mat'):
            continue
        base = fname.split('.')[0]
        mat_path = os.path.join(mat_dir, fname)
        img_path = os.path.join(img_dir, base + '.jpg')

        if not os.path.exists(img_path):
            print(f"Image {img_path} not found, skipping.")
            continue

        mask = extract_edges(mat_path)
        img = cv2.imread(img_path)

        # Resize image to match mask if needed
        if img.shape[:2] != mask.shape:
            img = cv2.resize(img, (mask.shape[1], mask.shape[0]))

        cv2.imwrite(os.path.join(out_img_dir, base + '.jpg'), img)
        cv2.imwrite(os.path.join(out_mask_dir, base + '.png'), mask)

        print(f"Converted {base}")

for split in ['train', 'val', 'test']:
    convert_split(split)
