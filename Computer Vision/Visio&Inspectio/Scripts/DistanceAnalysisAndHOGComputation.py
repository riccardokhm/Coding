import cv2
import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
from skimage import feature
from sklearn.metrics.pairwise import cosine_similarity
import math
import tkinter as tk
from tkinter.filedialog import askopenfilename, askdirectory
from datetime import datetime
import os
import sys
import time
import math

# Exponrntial distribution function for distance similarity score distribution
def ScoreDistribution(x, sigma):
  return np.exp(-np.power(x / sigma, 2) / 2)

# Function to analyse distances between ground truth and computer vision images
def AnalysingDistances(gt,cv,d):
  rows = gt.shape[0]
  cols = gt.shape[1]

  count_dist = 0
  count_g = 0
  count_max = 0

  similarity = 0
  recall = 0
  iter = 0
  tp_px = 0

  for i in range(0,rows):
    for j in range(0,cols):
      #max_gaussian_value = 0
      if (gt[i,j] == 255).any():
        iter += 1
        #gaussian_value = 0
        max_gaussian_value = 0
        #kernel_count = 0
        for k in range(-d, d+ 1):
          for l in range(-d, d+1):
            if i + k < rows and j + l < cols:
              if (cv[i + k, j + l] == 255).any():

                distance = np.sqrt(k * k + l * l)
                if distance == 0:
                  tp_px += 1
                  g = 1.0
                else:
                  count_dist += distance
                  g = ScoreDistribution(distance, 3)

                if g > 0:
                  if g >= max_gaussian_value:
                    max_gaussian_value = g
                    count_max += max_gaussian_value
                  count_g += g

                #if g > max_gaussian_value:
                  #max_gaussian_value = g
                  #count_max += max_gaussian_value
                  #count_max += 1
          #if kernel_count > 0:
            #gaussian_value /= kernel_count

        count_dist = int(math.ceil(count_dist))
        count_g = int(math.ceil(count_g))
        count_max = int(math.ceil(count_max))
        similarity += int(math.ceil(max_gaussian_value))

  #print("count_dist: ", int(math.ceil(count_dist)))
  #print("count_g: ", int(math.ceil(count_g)))
  #print("count_max: ", int(math.ceil(count_max)))

  #print("Similarity: ", int(math.ceil(similarity)))
  #print("tp_px", tp_px)
  #print("iter", iter)

  similarity = int(math.ceil(similarity)) / iter
  recall = tp_px / iter
  return (similarity, recall)

# Function to split a path into its components
def splitall(path):
    allparts = []
    while True:
        parts = os.path.split(path)
        if parts[0] == path:
            allparts.insert(0, parts[0])
            break
        elif parts[1] == path:
            allparts.insert(0, parts[1])
            break
        else:
            path = parts[0]
            allparts.insert(0, parts[1])
    return allparts

# Function to select an image file using a file dialog
def ImageSelection(title="Seleziona un'immagine"):
    tk.Tk().withdraw()
    path = askopenfilename(title=title)
    print("Hai selezionato:", path)
    return path

# Function to select a folder using a file dialog
# Please not selecting One Drive folders
def FolderSelection():
    tk.Tk().withdraw()
    folder_path = askdirectory(title="Seleziona la cartella di destinazione")
    print("Cartella selezionata:", folder_path)
    return folder_path

# Function to compute HOG features for both the computer vision image and the CAD image
def HOGComputation(canny_img, canny_cad_img):
    cv_hog, cv_hog_image = feature.hog(
        canny_img, orientations=9, pixels_per_cell=(10, 10), cells_per_block=(2, 2), visualize=True
    )
    cad_hog, cad_hog_image = feature.hog(
        canny_cad_img, orientations=9, pixels_per_cell=(10, 10), cells_per_block=(2, 2), visualize=True
    )
    return cv_hog, cv_hog_image, cad_hog, cad_hog_image

# Function to save an image safely, ensuring it is in the correct format
def safe_save_image(image, path, name):
    if image is not None:
        if image.dtype != np.uint8:
            image = np.clip(image * 255, 0, 255).astype(np.uint8)
        full_path = os.path.join(path, name)
        success = cv2.imwrite(full_path, image)
        if success:
            print(f"Immagine salvata: {full_path}")
        else:
            print(f"Errore nel salvataggio: {full_path}")
    else:
        print(f"Immagine {name} è None e non è stata salvata.")

# Main function to run the image analysis
if __name__ == "__main__":
    print("Please select correct real image...\n")
    cv_img_path = ImageSelection("Seleziona l'immagine reale")
    splitted_path = splitall(cv_img_path)

    print("Please select CAD mask image...\n")
    cad_img_path = ImageSelection("Seleziona la maschera CAD")

    print("Please select output folder (not OneDrive folders)...\n")
    base_folder = FolderSelection()

    # Load images safely
    def safe_imread(path):
        with open(path, 'rb') as f:
            file_bytes = np.asarray(bytearray(f.read()), dtype=np.uint8)
            return cv2.imdecode(file_bytes, cv2.IMREAD_GRAYSCALE)

    cv_img = safe_imread(cv_img_path)
    cad_img = safe_imread(cad_img_path)

    if cv_img is None or cad_img is None:
        print("Error in loading images. Please check the file paths and formats.")
        sys.exit(1)

    # Image pre-processing
    blurred_img = cv2.bilateralFilter(cv_img, 15, 75, 75)
    canny_cv_img = cv2.Canny(blurred_img, 20, 90)

    # Dilating CAD mask
    kernel = np.ones((11, 11), np.uint8)
    cad_dilated_img = cv2.dilate(cad_img, kernel, iterations=1)
    cv_mask_img = cv2.bitwise_and(canny_cv_img, canny_cv_img, mask=cad_dilated_img)

    # Canny edge detection on CAD image
    canny_cad_img = cv2.Canny(cad_img, 50, 150)

    # Analysing distances
    d = 5  # Kernel size for distance analysis
    similarity, recall = AnalysingDistances(canny_cad_img, cv_mask_img, d)
    print(f"Similarity: {similarity:.4f}, Recall: {recall:.4f}")

    # HOG
    cv_hog, cv_hog_img, cad_hog, cad_hog_img = HOGComputation(cv_mask_img, canny_cad_img)

    # Cosine Similarity
    cos_similarity = cosine_similarity([cv_hog], [cad_hog])
    #cosine_similarity_transformed = (similarity[0, 0] + 1) / 2
    #exp_cos_sim = math.exp(similarity[0, 0])
    #exp_cos_sim_trans = math.exp(cosine_similarity_transformed)

    print(f'Cosine Similarity: {cos_similarity[0, 0]:.4f}')
    #print(f'Trasformata: {cosine_similarity_transformed:.4f}')
    #print(f'Exp: {exp_cos_sim:.4f}')
    #print(f'Exp trasformata: {exp_cos_sim_trans:.4f}')

    # Stampa media di cosine similarity e similarity
    print(f'Total Similarity: {(similarity + cos_similarity[0, 0]) / 2:.4f}')

    # Salvataggio risultati
    date = datetime.today().strftime('%Y-%m-%d')
    time = datetime.today().strftime('%H-%M-%S')
    result_path = os.path.join(base_folder, f"{date}_{time}")
    os.makedirs(result_path, exist_ok=True)

    # Salvataggio immagini
    safe_save_image(canny_cv_img, result_path, 'cvImage.png')
    safe_save_image(cv_mask_img, result_path, 'cvMaskImage.png')
    safe_save_image(canny_cad_img, result_path, 'cannyCadImage.png')
    safe_save_image(cv_hog_img, result_path, 'cvHogImage.png')
    safe_save_image(cad_hog_img, result_path, 'cadHogImage.png')

    # CSV
    file_path = os.path.join(result_path, "results.csv")
    columns = ["item", "body", "date", "time", "similarity", "recall", "cosine_similarity", "total_similarity"]

    if not os.path.exists(file_path):
        df = pd.DataFrame(columns=columns)
        df.to_csv(file_path, index=False)

    df = pd.read_csv(file_path)
    new_row = {
        'item': splitted_path[5] if len(splitted_path) > 5 else '',
        'body': splitted_path[9] if len(splitted_path) > 9 else '',
        'date': date,
        'time': time,
        'similarity': similarity,
        'recall': recall,
        'cosine_similarity': cos_similarity[0, 0],
        "total_similarity": (similarity + cos_similarity[0, 0]) / 2
    }

    df.loc[len(df)] = new_row
    df = df.drop_duplicates()
    df.to_csv(file_path, mode='w', index=False, header=True)

    # Visualizzazione
    cv2.imshow("CV mask image", cv_mask_img)
    cv2.imshow("Canny CAD image", canny_cad_img)
    cv2.imshow("CV HOG", cv_hog_img)
    cv2.imshow("CAD HOG", cad_hog_img)
    cv2.waitKey(0)
    cv2.destroyAllWindows()
