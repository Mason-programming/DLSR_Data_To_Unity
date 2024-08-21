import gphoto2 as gp
import cv2
import socket
import pickle

# Capture frame from Canon DSLR
def capture_frame(camera):
    file_path = camera.capture(gp.GP_CAPTURE_IMAGE)
    camera_file = camera.file_get(file_path.folder, file_path.name, gp.GP_FILE_TYPE_NORMAL)
    camera_file.save(file_path.name)
    frame = cv2.imread(file_path.name)
    return frame

# Send raw frame data to C++ application
def send_frame_to_cpp(frame):
    _, encoded_frame = cv2.imencode('.jpg', frame)
    data = pickle.dumps(encoded_frame)
    
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
        s.connect(('127.0.0.1', 65431))
        s.sendall(data)

# Main loop to capture and send raw frames
def main():
    context = gp.Context()
    camera = gp.Camera()
    camera.init(context)

    while True:
        frame = capture_frame(camera)
        send_frame_to_cpp(frame)

if __name__ == '__main__':
    main()

