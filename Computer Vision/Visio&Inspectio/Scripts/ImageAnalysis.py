import cv2
import numpy as np
import time
import math

def ScoreDistribution(x, sigma):
  return np.exp(-np.power(x / sigma, 2) / 2)

def AnalysingDistances(gt,cv,d):
  rows = gt.shape[0]
  cols = gt.shape[1]

  factor = 3 * np.sqrt(2 * np.pi)

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

  print("count_dist: ", int(math.ceil(count_dist)))
  print("count_g: ", int(math.ceil(count_g)))
  print("count_max: ", int(math.ceil(count_max)))

  print("Similarity: ", int(math.ceil(similarity)))
  print("tp_px", tp_px)
  print("iter", iter)

  similarity = int(math.ceil(similarity)) / iter
  recall = tp_px / iter
  return (similarity, recall)


if __name__ == "__main__":
    gt = cv2.imread("/content/Cad_Mask_Canny_InspectionModel_SemicassaT.png")
    cv = cv2.imread("/content/Canny_Compare_InspectionModel_SemicassaT.png")

    start_time = time.time()
    (similarity, recall) = AnalysingDistances(gt,cv,3)

    print(similarity)
    print(recall)

    print("--- %s seconds ---" % (time.time() - start_time))