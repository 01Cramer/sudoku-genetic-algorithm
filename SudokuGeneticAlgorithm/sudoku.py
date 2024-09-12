import subprocess
import sys
import pkg_resources

def install(package):
    subprocess.check_call([sys.executable, "-m", "pip", "install", package])

required_packages = {
    "opencv-python": "cv2",
    "numpy": "numpy",
    "inference-sdk": "inference_sdk"
}

for package, import_name in required_packages.items():
    try:
        pkg_resources.get_distribution(package)
        __import__(import_name)
    except (pkg_resources.DistributionNotFound, ImportError):
        install(package)

import cv2
import numpy as np
import sys
from concurrent.futures import ThreadPoolExecutor, as_completed
from inference_sdk import InferenceHTTPClient

CLIENT = InferenceHTTPClient(
    api_url="https://detect.roboflow.com",
    api_key="" # Put your API key here
)

class_to_digit = {
    'zero': 0,
    'one': 1,
    'two': 2,
    'three': 3,
    'four': 4,
    'five': 5,
    'six': 6,
    'seven': 7,
    'eight': 8,
    'nine': 9
}

image_path = str(sys.argv[1])  # Holding path for sudoku board image

def preprocess_image(image_path):
    image = cv2.imread(image_path, cv2.IMREAD_GRAYSCALE)
    processed_image = cv2.GaussianBlur(image, (5, 5), 0)
    processed_image = cv2.adaptiveThreshold(processed_image, 255, cv2.ADAPTIVE_THRESH_GAUSSIAN_C, cv2.THRESH_BINARY_INV, 11, 2)
    return processed_image

def find_sudoku_grid(processed_image):
    contours, _ = cv2.findContours(processed_image, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)
    contours = sorted(contours, key=cv2.contourArea, reverse=True)
    for contour in contours:
        perimeter = cv2.arcLength(contour, True)
        approx = cv2.approxPolyDP(contour, 0.02 * perimeter, True)
        if len(approx) == 4:
            return approx
    return None

def extract_sudoku_grid(image, corners):
    def order_points(pts):
        rect = np.zeros((4, 2), dtype="float32")
        s = pts.sum(axis=1)
        rect[0] = pts[np.argmin(s)]
        rect[2] = pts[np.argmax(s)]
        diff = np.diff(pts, axis=1)
        rect[1] = pts[np.argmin(diff)]
        rect[3] = pts[np.argmax(diff)]
        return rect

    ordered_corners = order_points(corners.reshape(4, 2))
    (tl, tr, br, bl) = ordered_corners
    widthA = np.linalg.norm(br - bl)
    widthB = np.linalg.norm(tr - tl)
    maxWidth = max(int(widthA), int(widthB))
    heightA = np.linalg.norm(tr - br)
    heightB = np.linalg.norm(tl - bl)
    maxHeight = max(int(heightA), int(heightB))
    dst = np.array([
        [0, 0],
        [maxWidth - 1, 0],
        [maxWidth - 1, maxHeight - 1],
        [0, maxHeight - 1]], dtype="float32")
    M = cv2.getPerspectiveTransform(ordered_corners, dst)
    warped = cv2.warpPerspective(image, M, (maxWidth, maxHeight))
    return warped

def is_empty(cell):
    margin_ratio = 0.1
    h, w = cell.shape

    y_start = int(h * margin_ratio)
    y_end = h - int(h * margin_ratio)
    x_start = int(w * margin_ratio)
    x_end = w - int(w * margin_ratio)

    central_region = cell[y_start:y_end, x_start:x_end]

    white_pixels = cv2.countNonZero(central_region)
    total_pixels = central_region.size
    white_ratio = white_pixels / total_pixels

    return white_ratio < 0.05

def get_sorted_predictions(predictions):
    return sorted(predictions, key=lambda x: x['confidence'], reverse=True)

def recognize_row(i, sudoku_grid, cell_height, cell_width):
    row = []
    for j in range(9):
        x_start = j * cell_width
        y_start = i * cell_height
        x_end = (j + 1) * cell_width
        y_end = (i + 1) * cell_height
        cell = sudoku_grid[y_start:y_end, x_start:x_end]

        if len(cell.shape) == 3:
            cell = cv2.cvtColor(cell, cv2.COLOR_BGR2GRAY)

        cell = cv2.threshold(cell, 0, 255, cv2.THRESH_BINARY_INV + cv2.THRESH_OTSU)[1]

        if is_empty(cell):
            row.append(0)
        else:
            result = CLIENT.infer(cell, model_id="mnist-cjkff/2")
            predictions = result['predictions']
            most_probable = max(predictions, key=lambda x: x['confidence'])
            if float(most_probable['confidence']) > 0.7:
                if class_to_digit[most_probable['class']] == 0:
                    sorted_predictions = get_sorted_predictions(predictions)
                    if len(sorted_predictions) > 1:
                        second_most_probable = sorted_predictions[1]
                        row.append(class_to_digit[second_most_probable['class']])
                    else:
                        row.append(0)
                else:
                    row.append(class_to_digit[most_probable['class']])
            else:
                row.append(0)
    return i, row

def recognize_digits(sudoku_grid):
    sudoku_numbers = [None] * 9
    cell_height = sudoku_grid.shape[0] // 9
    cell_width = sudoku_grid.shape[1] // 9

    with ThreadPoolExecutor() as executor:
        futures = {executor.submit(recognize_row, i, sudoku_grid, cell_height, cell_width): i for i in range(9)}

        for counter, future in enumerate(as_completed(futures)):
            i, row = future.result()
            sudoku_numbers[i] = row
            print(f"Processed {(counter+1)*9}/81 digits.")

    return [digit for row in sudoku_numbers for digit in row]

def process_image(image_path):
    img = cv2.imread(image_path)
    processed_image = preprocess_image(img)
    corners = find_sudoku_grid(processed_image)

    if corners is None:
        return "Couldn't find sudoku board on image."

    sudoku_grid = extract_sudoku_grid(img, corners)
    sudoku_numbers = recognize_digits(sudoku_grid)
    return ' '.join(map(str, sudoku_numbers))

print("PROCESSING STARTED")

                    ### EXECUTION ###
processed_image = preprocess_image(image_path)
sudoku_corners = find_sudoku_grid(processed_image)

if sudoku_corners is not None:
    original_image = cv2.imread(image_path)
    extracted_sudoku = extract_sudoku_grid(original_image, sudoku_corners)
    sudoku_numbers = recognize_digits(extracted_sudoku)
    sudoku_string = ''.join(map(str, sudoku_numbers))
    with open("encoded_board.txt", 'w', encoding='utf-8') as file:
        file.write(sudoku_string)

else:
    print("Couldn't find sudoku board on image.")
