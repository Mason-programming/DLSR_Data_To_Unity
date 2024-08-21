# DLSR_Data_To_Unity
Developed a custom reliable communication protocol (CRCP) in C++ for real-time video transmission, enabling seamless integration between a Canon DSLR camera and a Unity-based application. The camera captured live video, processed using computer vision (CV) techniques to detect and track objects within the frame. These objects were dynamically mapped onto a Unity grid, with bounding boxes assigned to each detected object, and collision detection implemented for interactions within the virtual environment. A PyQt-based interface provided intuitive control and monitoring, demonstrating expertise in network programming, real-time video processing, and 3D rendering in Unity.

Python Script:
Captures the raw video frames from the Canon DSLR camera.
Sends the raw frames directly to the C++ application over a socket.

C++ Custom Communication Protocol:
Receives the raw video frames.
Performs object detection using OpenCV in C++.
Processes the detected objects and generates bounding boxes.
Transmits bounding box data to Unity for rendering and collision detection.

Unity:
Receives bounding box data from the C++ application.
Renders corresponding objects in the Unity scene and handles collision detection.
