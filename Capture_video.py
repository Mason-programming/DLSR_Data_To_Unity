import gphoto2 as gp
import cv2
import numpy as np

def capture_frame():
    context = gp.Context()
    camera = gp.Camera()
    camera.init(context)

    file_path = camera.capture(gp.GP_CAPTURE_IMAGE)
    camera_file = camera.file_get(file_path.folder, file_path.name, gp.GP_FILE_TYPE_NORMAL)
    camera_file.save(file_path.name)

    image = cv2.imread(file_path.name)
    return image

def process_frame(frame):
    # Convert frame to grayscale for processing
    gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)

    # Example: Use a simple edge detection method
    edges = cv2.Canny(gray, 50, 150)

    # Find contours (these could represent object boundaries)
    contours, _ = cv2.findContours(edges, cv2.RETR_TREE, cv2.CHAIN_APPROX_SIMPLE)
    
    bounding_boxes = []
    for cnt in contours:
        x, y, w, h = cv2.boundingRect(cnt)
        bounding_boxes.append((x, y, w, h))
        cv2.rectangle(frame, (x, y), (x+w, y+h), (0, 255, 0), 2)  # Draw bounding box

    return frame, bounding_boxes

# Transmit data to Unity via sockets
import socket
import pickle

def send_data_to_unity(bounding_boxes):
    data = pickle.dumps(bounding_boxes)  # Serialize bounding box data
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
        s.connect(('127.0.0.1', 65432))
        s.sendall(data)
