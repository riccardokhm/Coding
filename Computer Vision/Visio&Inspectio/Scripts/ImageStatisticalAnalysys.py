import cv2
import numpy as np
import matplotlib.pyplot as plt
from scipy.stats import f_oneway # Anova test
from sklearn.mixture import GaussianMixture # EM and Mixture of Gaussians
import tkinter as tk
from tkinter import filedialog, messagebox
from PIL import Image, ImageTk
import os # Import for path manipulation

def preprocess_images(img):
    """
    Applies a Gaussian filter for preprocessing the image.
    """
    if img is None:
        return None
    return cv2.GaussianBlur(img, (5, 5), 0)

def calc_histogram(img):
    """
    Calculates the histogram for the given image.
    """
    if img is None:
        return None
    return cv2.calcHist([img], [0], None, [256], [0, 256])

def normalize_histogram(hist):
    """
    Normalizes the histogram to obtain a Probability Density Function (PDF).
    """
    if hist is None:
        return None
    return hist / hist.sum()

def estimate_canny_thresholds(img_real_blurred, pdf_ar_edges):
    """
    Estimates Canny thresholds using a Gaussian Mixture Model (GMM)
    and the PDF of the AR image to guide edge identification.
    """
    if img_real_blurred is None or pdf_ar_edges is None:
        return None, None

    data_real = img_real_blurred.flatten().reshape(-1, 1)

    # GMM initialization
    gmm = GaussianMixture(n_components=2, random_state=0)

    # Initialize GMM with the PDF of the AR edges.
    intensity_levels = np.arange(256)
    mean_ar_edges = np.sum(intensity_levels * pdf_ar_edges.flatten())

    gmm.means_ = np.array([[mean_ar_edges - 50], [mean_ar_edges + 50]]) # Attempted initialization
    gmm.precisions_init_ = np.array([[[1/50**2]], [[1/50**2]]]) # Initialize precisions (1/variance)

    try:
        gmm.fit(data_real)
    except ValueError as e:
        messagebox.showerror("GMM Error", f"Error during GMM fitting: {e}\nIt might require a larger dataset or different GMM parameters.")
        return None, None

    means = gmm.means_.flatten()
    covariances = gmm.covariances_.flatten()
    weights = gmm.weights_.flatten()

    print(f"GMM component means: {means}")
    print(f"GMM component variances: {covariances}")
    print(f"GMM component weights: {weights}")

    closest_component_idx = np.argmin(np.abs(means - mean_ar_edges))

    edge_component_mean = means[closest_component_idx]
    edge_component_std = np.sqrt(covariances[closest_component_idx])

    low_threshold = max(0, int(edge_component_mean - 1.5 * edge_component_std))
    high_threshold = min(255, int(edge_component_mean + 1.5 * edge_component_std))

    print(f"First Estimated Canny thresholds: Low={low_threshold}, High={high_threshold}")

    if len(means) == 2:
        other_component_idx = 1 - closest_component_idx

        x = np.arange(0, 256).reshape(-1, 1)
        pdf_gmm_comp1 = np.exp(gmm.score_samples(x)) * weights[closest_component_idx]
        pdf_gmm_comp2 = np.exp(gmm.score_samples(x)) * weights[other_component_idx]

        if means[other_component_idx] < means[closest_component_idx]: # The other component is the darker one (non-edge)
            low_threshold = int(means[other_component_idx] + np.sqrt(covariances[other_component_idx])) # A bit beyond the mean of the dark component
        else: # The other component is lighter, which is unusual for non-edges
            low_threshold = int(means[closest_component_idx] - np.sqrt(covariances[closest_component_idx])) # A bit below the mean of the edge component

        low_threshold = min(low_threshold, high_threshold - 1)
        low_threshold = max(0, low_threshold)
        high_threshold = min(255, high_threshold)

        print(f"Second Estimated Canny thresholds: Low={low_threshold}, High={high_threshold}")

    return low_threshold, high_threshold

class ImageProcessorApp:
    def __init__(self, root):
        self.root = root
        self.root.title("Canny Edge Detection with Histogram Analysis")

        self.img_real = None
        self.img_ar_edges = None
        self.img_real_blurred = None
        self.img_ar_edges_blurred = None
        self.edges_result = None

        self.real_image_path = None
        self.ar_image_path = None

        self.create_widgets()

    def create_widgets(self):
        # Frame for loading buttons
        load_frame = tk.Frame(self.root, padx=10, pady=10)
        load_frame.pack(fill=tk.X)

        self.btn_load_real = tk.Button(load_frame, text="Load Real Image", command=self.load_real_image)
        self.btn_load_real.pack(side=tk.LEFT, padx=5, pady=5)

        self.btn_load_ar = tk.Button(load_frame, text="Load AR Image", command=self.load_ar_image)
        self.btn_load_ar.pack(side=tk.LEFT, padx=5, pady=5)

        # Frame for processing
        process_frame = tk.Frame(self.root, padx=10, pady=10)
        process_frame.pack(fill=tk.X)

        self.btn_process = tk.Button(process_frame, text="Process and Find Canny Edges", command=self.process_images)
        self.btn_process.pack(pady=10)

    def load_image_dialog(self):
        """Opens a dialog window to select an image file."""
        file_path = filedialog.askopenfilename(
            title="Select Image",
            filetypes=[("Image files", "*.png *.jpg *.jpeg *.bmp *.gif *.tiff")]
        )
        if file_path:
            img = cv2.imread(file_path, cv2.IMREAD_GRAYSCALE)
            if img is None:
                messagebox.showerror("Loading Error", "Could not load the image. Check the file format.")
            return img, file_path
        return None, None

    def load_real_image(self):
        self.img_real, self.real_image_path = self.load_image_dialog()
        if self.img_real is not None:
            messagebox.showinfo("Real Image Loaded", f"Real image loaded: {os.path.basename(self.real_image_path)}")
            # Opzionalmente, mostra l'immagine caricata immediatamente in una finestra Matplotlib
            self.display_image_matplotlib(self.img_real, "Real image")
            self.img_real_blurred = preprocess_images(self.img_real)

    def load_ar_image(self):
        self.img_ar_edges, self.ar_image_path = self.load_image_dialog()
        if self.img_ar_edges is not None:
            messagebox.showinfo("AR Image Loaded", f"AR edges image loaded: {os.path.basename(self.ar_image_path)}")
            # Opzionalmente, mostra l'immagine AR caricata immediatamente in una finestra Matplotlib
            self.display_image_matplotlib(self.img_ar_edges, "AR image")
            self.img_ar_edges_blurred = preprocess_images(self.img_ar_edges)

    def display_image_matplotlib(self, img_cv, title="Immagine"):
        """Display an image using Matplotlib."""
        if img_cv is not None:
            plt.figure(title) # Crea una nuova figura con il titolo specificato
            plt.imshow(img_cv, cmap='gray') # Mostra l'immagine in scala di grigi
            plt.title(title)
            plt.axis('off') # Nasconde gli assi
            plt.show(block=False) # Mostra la figura senza bloccare l'interfaccia Tkinter
                                 # Sarà necessario plt.show() alla fine del process_images
                                 # per assicurarsi che tutte le figure vengano visualizzate.

    def process_images(self):
        if self.img_real_blurred is None or self.img_ar_edges_blurred is None:
            messagebox.showwarning("Missing Images", "Load both images before proceeding.")
            return

        hist_real = calc_histogram(self.img_real_blurred)
        hist_ar_edges = calc_histogram(self.img_ar_edges_blurred)

        pdf_real = normalize_histogram(hist_real)
        pdf_ar_edges = normalize_histogram(hist_ar_edges)

        if pdf_real is None or pdf_ar_edges is None:
            messagebox.showerror("Processing Error", "Could not calculate histograms/PDFs.")
            return

        low_threshold, high_threshold = estimate_canny_thresholds(self.img_real_blurred, pdf_ar_edges)

        if low_threshold is None or high_threshold is None:
            messagebox.showerror("Threshold Estimation Error", "Could not estimate Canny thresholds.")
            return

        self.edges_result = cv2.Canny(self.img_real_blurred, low_threshold, high_threshold)

        # Visualizza i risultati in finestre Matplotlib separate
        self.display_image_matplotlib(self.img_real, "Real image (Grayscale)")
        self.display_image_matplotlib(self.edges_result, f"Canny Edges (L={low_threshold}, H={high_threshold})")

        # Salva le immagini
        self.save_processed_images(low_threshold, high_threshold)

        # Visualizza gli istogrammi in una finestra Matplotlib separata
        plt.figure(figsize=(12, 5))
        plt.subplot(1, 2, 1)
        plt.title('Real Image Histogram')
        plt.plot(pdf_real)
        plt.xlabel('Grayscale Intensity')
        plt.ylabel('Probability')

        plt.subplot(1, 2, 2)
        plt.title('AR Image (Edges) Histogram')
        plt.plot(pdf_ar_edges)
        plt.xlabel('Grayscale Intensity')
        plt.ylabel('Probability')
        plt.tight_layout()

        # Blocca l'esecuzione finché tutte le figure Matplotlib non vengono chiuse
        # Questo è importante per assicurarsi che le finestre rimangano aperte.
        plt.show()

        messagebox.showinfo("Processing Complete", f"Canny edges extracted with thresholds: Low={low_threshold}, High={high_threshold}")

    def save_processed_images(self, low_thresh, high_thresh):
        save_dir = filedialog.askdirectory(title="Select Folder to Save Images")
        if not save_dir:
            messagebox.showwarning("Save Cancelled", "Image saving cancelled.")
            return

        # Save grayscale real image
        if self.img_real is not None and self.real_image_path is not None:
            real_img_name = os.path.basename(self.real_image_path)
            base, ext = os.path.splitext(real_img_name)
            save_path_real_grayscale = os.path.join(save_dir, f"{base}_grayscale.png")
            cv2.imwrite(save_path_real_grayscale, self.img_real)
            print(f"Saved real grayscale image to: {save_path_real_grayscale}")

        # Save grayscale AR image
        if self.img_ar_edges is not None and self.ar_image_path is not None:
            ar_img_name = os.path.basename(self.ar_image_path)
            base, ext = os.path.splitext(ar_img_name)
            save_path_ar_grayscale = os.path.join(save_dir, f"{base}_ar_grayscale.png")
            cv2.imwrite(save_path_ar_grayscale, self.img_ar_edges)
            print(f"Saved AR grayscale image to: {save_path_ar_grayscale}")

        # Save Canny result
        if self.edges_result is not None and self.real_image_path is not None:
            real_img_name = os.path.basename(self.real_image_path)
            base, ext = os.path.splitext(real_img_name)
            save_path_canny = os.path.join(save_dir, f"{base}_canny_L{low_thresh}_H{high_thresh}.png")
            cv2.imwrite(save_path_canny, self.edges_result)
            print(f"Saved Canny result to: {save_path_canny}")
            messagebox.showinfo("Images Saved", f"Processed images saved to:\n{save_dir}")

# --- Main Application ---
if __name__ == "__main__":
    root = tk.Tk()
    app = ImageProcessorApp(root)
    root.mainloop()