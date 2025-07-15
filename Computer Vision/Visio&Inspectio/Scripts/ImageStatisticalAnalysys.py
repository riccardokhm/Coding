import cv2
import numpy as np
import matplotlib.pyplot as plt
from scipy.stats import f_oneway # Anova test - included for completeness, not used in this specific logic
from sklearn.mixture import GaussianMixture # EM and Mixture of Gaussians
import tkinter as tk
from tkinter import filedialog, messagebox
from PIL import Image, ImageTk # Used for Tkinter image display (though not directly in this version, good to keep for GUI)
import os # Import for path manipulation
from scipy.signal import find_peaks # For detecting peaks in histograms

#region Image Preprocessing and Histogram Functions

def preprocess_images(img):

    if img is None:
        return None
    # Gaussian blur with a 5x5 kernel and 0 standard deviation (auto-calculated)
    return cv2.GaussianBlur(img, (5, 5), 0)

def calc_histogram(img):
    """
    Calculates the histogram for the given grayscale image.
    """
    if img is None:
        return None
    # Calculate histogram for the single channel (0), with 256 bins, range 0-256
    return cv2.calcHist([img], [0], None, [256], [0, 256])

def normalize_histogram(hist):
    """
    Normalizes the histogram to obtain a Probability Density Function (PDF).
    """
    if hist is None:
        return None
    hist_sum = hist.sum()
    if hist_sum == 0:
        return np.zeros_like(hist, dtype=float) # Return zeros if histogram is empty
    return hist / hist_sum

def find_histogram_peaks(hist_pdf, prominence=0.001, width=1):
    """
    Finds peaks in a histogram PDF using scipy.signal.find_peaks.
    with normalized histogram (PDF).
    prominence of peaks. A peak's prominence is the vertical distance between the peak and its lowest contour line
    to the left or right, extending to the peak itself.
    Lower values detect more subtle peaks.
    width of peaks in samples. Helps filter out very narrow spikes.
    """
    # Find peaks in the PDF. 'distance' could also be useful to ensure peaks are not too close.
    peaks, properties = find_peaks(hist_pdf.flatten(), prominence=prominence, width=width)
    return peaks, properties

# --- Canny Threshold Estimation Logic ---

def estimate_canny_thresholds(img_real_blurred, pdf_ar_edges):
    """
    Estimates Canny thresholds for the real image using a Gaussian Mixture Model (GMM).
    The GMM is guided by the AR image's histogram, specifically identifying the background
    and assuming black edges (intensity 0) in the AR image.
    """
    if img_real_blurred is None or pdf_ar_edges is None:
        print("Error: Input images for threshold estimation are None.")
        return None, None

    data_real = img_real_blurred.flatten().reshape(-1, 1)

    # Step 1: Analyze AR Image Histogram to infer structure (background and edge location) ---
    # The AR image has blaack edges (intensity 0) and the same background as the real image.
    # So, the AR histogram will have a small peak at 0 and --> (maybe) several dominant peaks for the background.

    # Find peaks in AR image PDF. Use low prominence to catch the distinct black edge peak at 0.
    ar_peaks_indices, _ = find_histogram_peaks(pdf_ar_edges, prominence=0.001, width=1)
    print(f"AR histogram peaks found at intensities: {ar_peaks_indices}")

    # Determine AR background intensity: the brightest prominent peak, excluding the 0-intensity peak.
    ar_background_intensity = -1
    
    # Filter for peaks in the higher intensity ranges first --> to correctly identify the brights background in the AR image.
    candidate_bg_peaks = [
        p for p in ar_peaks_indices 
        if p > 100 # Assume background is typically brighter than 100
    ]

    if not candidate_bg_peaks: # If no peaks > 100, try again with a wider range but still not 0
        candidate_bg_peaks = [p for p in ar_peaks_indices if p > 50] # Try > 50
    if not candidate_bg_peaks: # If still empty, include all non-zero peaks
        candidate_bg_peaks = [p for p in ar_peaks_indices if p != 0]

    if candidate_bg_peaks:
        # Find the *brightest* intensity among the candidate background peaks, rather than the one with the highest PDF value (most prominent).
        ar_background_intensity = max(candidate_bg_peaks) 
    else: # Fallback if no non-zero peaks at all (highly unlikely for a proper AR image) --> to general mode of upper half if all else fails
        ar_background_intensity = np.argmax(pdf_ar_edges[100:]) + 100
        print(f"Warning: No clear AR background peak detected from filtered peaks. Falling back to dominant upper-half intensity: {ar_background_intensity}")

    print(f"AR image's estimated background intensity: {ar_background_intensity}")


    # Step 2: Determine n_components for GMM on REAL image
    # The number of components should reflect the complexity of the real image's intensity distributions.
    hist_real = calc_histogram(img_real_blurred)
    pdf_real = normalize_histogram(hist_real)
    # Use a slightly higher prominence for real image peaks to find more significant clusters
    real_peaks_indices, _ = find_histogram_peaks(pdf_real, prominence=0.005, width=3)

    # Ensure at least 2 components for basic segmentation (object/background)
    n_components = max(2, len(real_peaks_indices))
    # Cap components to avoid overfitting if the real histogram is very noisy or has many minor peaks (for example, 7 is a reasonable))
    n_components = min(n_components, 7) 
    print(f"Detected {len(real_peaks_indices)} peaks in Real image histogram. Using n_components for GMM: {n_components}")

    # Step 3: Fit GMM to the real image data
    # Using kmeans --> (segmentation problems) for robust initialization, which is generally good for multiple components.
    gmm = GaussianMixture(n_components=n_components, random_state=0, init_params='kmeans')

    try:
        gmm.fit(data_real)
    except ValueError as e:
        messagebox.showerror("GMM Error", f"Error during GMM fitting: {e}\nIt might require a larger dataset or different GMM parameters.")
        return None, None

    means = gmm.means_.flatten()
    covariances = gmm.covariances_.flatten()
    weights = gmm.weights_.flatten()

    print(f"\n--- Real Image GMM Components ---")
    print(f"GMM component means (sorted): {np.sort(means)}")
    print(f"GMM component std_devs (sorted by mean): {np.sqrt(covariances)[np.argsort(means)]}")
    print(f"GMM component weights (sorted by mean): {weights[np.argsort(means)]}")

    # Step 4: Identify "Background" and "Object/Edge Transition" Components in Real Image GMM

    # Find the GMM component in the REAL image that best represents the background.
    # This is the component whose mean is closest to the ar_background_intensity identified from the AR image.
    background_comp_idx = np.argmin(np.abs(means - ar_background_intensity))
    real_background_mean = means[background_comp_idx]
    real_background_std = np.sqrt(covariances[background_comp_idx])
    print(f"Real image GMM background component: Mean={real_background_mean:.2f}, StdDev={real_background_std:.2f}")


    # Identify potential "object" components (not background, not too bright, significant weight)
    # Filter for components that are reasonably darker than the background.
    object_candidates = []
    for i in range(n_components):
        if i == background_comp_idx: # Skip the identified background component
            continue
        # Check if component is darker than background AND has significant weight
        if means[i] < real_background_mean - (real_background_std / 4) and weights[i] > 0.01:
            object_candidates.append({'mean': means[i], 'std': np.sqrt(covariances[i]), 'weight': weights[i], 'idx': i})

    # Sort candidates by mean (darkest first)
    object_candidates.sort(key=lambda x: x['mean'])

    low_threshold = None
    high_threshold = None

    if len(object_candidates) > 0:
        # Choose the darkest, most significant candidate as the primary "object" component for edges
        object_comp_mean = object_candidates[0]['mean']
        object_comp_std = object_candidates[0]['std']
        print(f"Real image GMM chosen object component: Mean={object_comp_mean:.2f}, StdDev={object_comp_std:.2f}")

        # Step 5: Determine Canny Thresholds
        # Low Threshold--> transition value between the darker object parts and the background.
        low_threshold_candidate_1 = int((object_comp_mean + real_background_mean) / 2)
        # another possibility --> slightly above the object mean, to catch gradients rising from the object
        low_threshold_candidate_2 = int(object_comp_mean + 1.0 * object_comp_std)

        # Take the minimum of these to ensure it leans towards the object side
        low_threshold = min(low_threshold_candidate_1, low_threshold_candidate_2)
        # Ensure low threshold is not too high relative to background mean, and not below 0
        low_threshold = max(0, min(low_threshold, int(real_background_mean - real_background_std / 4)))


        # High Threshold: A stricter threshold for strong edges. Often a multiple of the low threshold--> Adjusted multiplier to prevent capping at 255
        high_threshold = int(low_threshold * 1.5) # Started from 2.5, 2.0 maybe also 1.8 or 1.5

        # Ensure high_threshold is meaningfully higher than low_threshold, and within [0, 255]
        high_threshold = max(high_threshold, low_threshold + int(0.1 * low_threshold) + 10) # Minimum gap
        high_threshold = min(255, high_threshold)

    else: # Fallback if no distinct object components darker than background were found
        print("Warning: No distinct object component darker than background found in real image GMM based on criteria.")
        print("Using a general heuristic based on the overall most prominent component.")
        most_prominent_comp_idx = np.argmax(weights)
        fallback_mean = means[most_prominent_comp_idx]
        fallback_std = np.sqrt(covariances[most_prominent_comp_idx])

        # Apply a general Canny heuristic (often 1:2 or 1:3 ratio)
        low_threshold = max(0, int(fallback_mean * 0.6))
        high_threshold = min(255, int(fallback_mean * 1.4))
        high_threshold = max(high_threshold, low_threshold + 5) # Ensure high > low

    # Final clamping to [0, 255]
    low_threshold = max(0, min(255, low_threshold))
    high_threshold = max(0, min(255, high_threshold))

    # Final check to ensure high_threshold is strictly greater than low_threshold
    if low_threshold >= high_threshold:
        if high_threshold < 255:
            high_threshold += 1 # Increment high if possible
        elif low_threshold > 0:
            low_threshold -= 1 # Decrement low if possible
        else: # Both are 0 or 255 and equal. Force a small gap.
            low_threshold = max(0, low_threshold - 1)
            high_threshold = min(255, high_threshold + 1)

    print(f"\nFinal Estimated Canny thresholds: Low={low_threshold}, High={high_threshold}")
    return low_threshold, high_threshold

#endregion

#region Tkinter GUI application class

class ImageProcessorApp:
    def __init__(self, root):
        self.root = root
        self.root.title("Canny Edge Detection with Adaptive Thresholding")

        self.img_real = None
        self.img_ar_edges = None
        self.img_real_blurred = None
        self.img_ar_edges_blurred = None
        self.edges_result = None
        self.real_image_path = None
        self.ar_image_path = None

        self.create_widgets()

    def create_widgets(self):
        load_frame = tk.Frame(self.root, padx=10, pady=10)
        load_frame.pack(fill=tk.X)
        self.btn_load_real = tk.Button(load_frame, text="Load Real Image", command=self.load_real_image)
        self.btn_load_real.pack(side=tk.LEFT, padx=5, pady=5)
        self.btn_load_ar = tk.Button(load_frame, text="Load AR Image (Ground Truth)", command=self.load_ar_image)
        self.btn_load_ar.pack(side=tk.LEFT, padx=5, pady=5)
        process_frame = tk.Frame(self.root, padx=10, pady=10)
        process_frame.pack(fill=tk.X)
        self.btn_process = tk.Button(process_frame, text="Process and Find Canny Edges", command=self.process_images)
        self.btn_process.pack(pady=10)

    def load_image_dialog(self):
        file_path = filedialog.askopenfilename(
            title="Select Image",
            filetypes=[("Image files", "*.png *.jpg *.jpeg *.bmp *.gif *.tiff")]
        )
        if file_path:
            img = cv2.imread(file_path, cv2.IMREAD_GRAYSCALE)
            if img is None:
                messagebox.showerror("Loading Error", "Could not load the image. Check the file format or permissions.")
            return img, file_path
        return None, None

    def load_real_image(self):
        self.img_real, self.real_image_path = self.load_image_dialog()
        if self.img_real is not None:
            messagebox.showinfo("Real Image Loaded", f"Real image loaded: {os.path.basename(self.real_image_path)}")
            # self.display_image_matplotlib(self.img_real, "Loaded Real Image (Grayscale)") # Removed, will show later in combined plot
            self.img_real_blurred = preprocess_images(self.img_real)
            print("Real image preprocessed (blurred).")

    def load_ar_image(self):
        self.img_ar_edges, self.ar_image_path = self.load_image_dialog()
        if self.img_ar_edges is not None:
            messagebox.showinfo("AR Image Loaded", f"AR edges image loaded: {os.path.basename(self.ar_image_path)}")
            # self.display_image_matplotlib(self.img_ar_edges, "Loaded AR Image (Grayscale)") # Removed, will show later in combined plot
            self.img_ar_edges_blurred = preprocess_images(self.img_ar_edges)
            print("AR image preprocessed (blurred).")

    def display_image_matplotlib(self, img_cv, title="Image"):
        # This function is now effectively unused if all plotting is done in process_images
        # It's kept for potential standalone debugging or future single-image displays
        if img_cv is not None:
            plt.figure(title)
            plt.imshow(img_cv, cmap='gray')
            plt.title(title)
            plt.axis('off')
            plt.show(block=False)

    def process_images(self):
        if self.img_real_blurred is None or self.img_ar_edges_blurred is None:
            messagebox.showwarning("Missing Images", "Load both Real and AR Images before proceeding.")
            return

        print("\n--- Starting Image Processing ---")

        intensity_levels = np.arange(256)

        hist_real = calc_histogram(self.img_real_blurred)
        hist_ar_edges = calc_histogram(self.img_ar_edges_blurred)

        pdf_real = normalize_histogram(hist_real)
        pdf_ar_edges = normalize_histogram(hist_ar_edges)

        if pdf_real is None or pdf_ar_edges is None:
            messagebox.showerror("Processing Error", "Could not calculate histograms/PDFs. Check image content.")
            return

        low_threshold, high_threshold = estimate_canny_thresholds(self.img_real_blurred, pdf_ar_edges)

        if low_threshold is None or high_threshold is None:
            messagebox.showerror("Threshold Estimation Error", "Could not estimate Canny thresholds. See console for details.")
            return

        self.edges_result = cv2.Canny(self.img_real_blurred, low_threshold, high_threshold)
        print(f"Canny edge detection applied with thresholds: Low={low_threshold}, High={high_threshold}")

        # --- Display in Two Separate Matplotlib Windows ---

        # Window 1: Original B&W Images and Histograms
        plt.figure("Original Images and Histograms", figsize=(16, 8)) # Adjusted size for better visibility

        # Subplot 1: Original Real Image
        plt.subplot(2, 2, 1) # 2 rows, 2 columns, 1st plot
        plt.imshow(self.img_real, cmap='gray')
        plt.title('Original Real Image (Grayscale)')
        plt.axis('off')

        # Subplot 2: Original AR Image
        plt.subplot(2, 2, 2) # 2 rows, 2 columns, 2nd plot
        plt.imshow(self.img_ar_edges, cmap='gray')
        plt.title('Original AR Image (Grayscale)')
        plt.axis('off')

        # Subplot 3: Real Image Histogram
        plt.subplot(2, 2, 3) # 2 rows, 2 columns, 3rd plot
        plt.title('Real Image Histogram (PDF)')
        plt.plot(intensity_levels, pdf_real)
        real_peaks, _ = find_histogram_peaks(pdf_real, prominence=0.005, width=3)
        plt.plot(real_peaks, pdf_real[real_peaks], "x", color='red', markersize=8, label='Detected Peaks')
        plt.xlabel('Grayscale Intensity')
        plt.ylabel('Probability')
        plt.legend()
        plt.grid(True, linestyle='--', alpha=0.7)

        # Subplot 4: AR Image Histogram
        plt.subplot(2, 2, 4) # 2 rows, 2 columns, 4th plot
        plt.title('AR Image (CAD Model) Histogram (PDF)')
        plt.plot(intensity_levels, pdf_ar_edges)
        ar_peaks, _ = find_histogram_peaks(pdf_ar_edges, prominence=0.001, width=1)
        plt.plot(ar_peaks, pdf_ar_edges[ar_peaks], "o", color='green', markersize=8, label='Detected Peaks')
        plt.xlabel('Grayscale Intensity')
        plt.ylabel('Probability')
        plt.legend()
        plt.grid(True, linestyle='--', alpha=0.7)

        plt.tight_layout() # Adjust subplot parameters for a tight layout
        plt.show(block=False) # Show this window, but allow the code to continue to the next plot

        # Window 2: Canny Edges Result (separate window)
        plt.figure("Canny Edges Result")
        plt.imshow(self.edges_result, cmap='gray')
        plt.title(f'Canny Edges (Low={low_threshold}, High={high_threshold})')
        plt.axis('off')
        plt.show(block=True) # This will block execution until this window is closed

        messagebox.showinfo("Processing Complete",
                             f"Canny edges extracted with thresholds: Low={low_threshold}, High={high_threshold}\n"
                             "Check the separate Matplotlib windows for results.")

        # This calls the save function which already saves all 4 desired images separately
        self.save_processed_images(low_threshold, high_threshold)

    def save_processed_images(self, low_thresh, high_thresh):
        save_dir = filedialog.askdirectory(title="Select Folder to Save Processed Images")
        if not save_dir:
            messagebox.showwarning("Save Cancelled", "Image saving cancelled by user.")
            return

        # Get base name for saving
        base = "processed_image"
        if self.real_image_path:
            real_img_name = os.path.basename(self.real_image_path)
            base, _ = os.path.splitext(real_img_name)
        elif self.ar_image_path:
            ar_img_name = os.path.basename(self.ar_image_path)
            base, _ = os.path.splitext(ar_img_name)


        if self.img_real is not None:
            save_path_real_grayscale = os.path.join(save_dir, f"{base}_real_grayscale.png")
            cv2.imwrite(save_path_real_grayscale, self.img_real)
            print(f"Saved original real grayscale image to: {save_path_real_grayscale}")

        if self.img_ar_edges is not None:
            save_path_ar_grayscale = os.path.join(save_dir, f"{base}_ar_grayscale.png")
            cv2.imwrite(save_path_ar_grayscale, self.img_ar_edges)
            print(f"Saved original AR grayscale image to: {save_path_ar_grayscale}")

        if self.edges_result is not None:
            save_path_canny = os.path.join(save_dir, f"{base}_canny_L{low_thresh}_H{high_thresh}.png")
            cv2.imwrite(save_path_canny, self.edges_result)
            print(f"Saved Canny result to: {save_path_canny}")
            
        # Save histograms as a single image
        hist_real = calc_histogram(self.img_real_blurred)
        hist_ar_edges = calc_histogram(self.img_ar_edges_blurred)
        if hist_real is not None and hist_ar_edges is not None:
            hist_real_normalized = normalize_histogram(hist_real)
            hist_ar_edges_normalized = normalize_histogram(hist_ar_edges)

            plt.figure(figsize=(12, 6)) # New figure for saving histograms
            plt.subplot(1, 2, 1)
            plt.title('Real Image Histogram (PDF)')
            plt.plot(np.arange(256), hist_real_normalized)
            plt.xlabel('Grayscale Intensity')
            plt.ylabel('Probability')
            plt.grid(True, linestyle='--', alpha=0.7)

            plt.subplot(1, 2, 2)
            plt.title('AR Image (CAD Model) Histogram (PDF)')
            plt.plot(np.arange(256), hist_ar_edges_normalized)
            plt.xlabel('Grayscale Intensity')
            plt.ylabel('Probability')
            plt.grid(True, linestyle='--', alpha=0.7)

            plt.tight_layout()
            hist_save_path = os.path.join(save_dir, f"{base}_histograms.png")
            plt.savefig(hist_save_path)
            print(f"Saved histograms to: {hist_save_path}")
            plt.close() # Close the figure after saving to prevent it from showing up

        messagebox.showinfo("Images Saved", f"Processed images saved to:\n{save_dir}")

#endregion

if __name__ == "__main__":
    root = tk.Tk()
    app = ImageProcessorApp(root)
    root.mainloop()