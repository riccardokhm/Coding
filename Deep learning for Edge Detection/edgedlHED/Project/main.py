import subprocess
import sys
import os
import torch

def run_conversion():
    print("\nStarting dataset conversion...")
    script_path = os.path.join(os.path.dirname(__file__), "convert_bsds500_mat_to_png.py")
    subprocess.run(["python", script_path])
    print("Conversion completed.\n")

def run_training(debug=False):
    print("\nStarting model training...")
    script_path = os.path.join(os.path.dirname(__file__), "train.py")
    args = ["python", script_path]
    if debug:
        args.append("--debug")
    subprocess.run(args)
    print("Training completed.\n")


def run_inference():
    print("\nStarting inference on real images...")
    script_path = os.path.join(os.path.dirname(__file__), "infer.py")
    subprocess.run(["python", script_path])
    print("Inference completed.\n")

def check_cuda():
    if not torch.cuda.is_available():
        print("CUDA not starting. Verify drivers and GPU compatibility!")
        sys.exit(1)
    else:
        print("GPU found:", torch.cuda.get_device_name(0))

def show_menu():
    print("\nOption selection:")
    print("1. Start dataset conversion")
    print("2. Start training (without debug)")
    print("3. Start training (with debug)")
    print("4. Start inference")
    print("5. Exit")

if __name__ == "__main__":
    check_cuda()

    while True:
        show_menu()
        choice = input("Insert operation number: ").strip()

        if choice == '1':
            run_conversion()
        elif choice == '2':
            run_training(debug=False)
        elif choice == '3':
            run_training(debug=True)
        elif choice == '4':
            run_inference()
        elif choice == '5':
            print("Exit from launcher.")
            break
        else:
            print("Wrong choice. Retry!.")
    