# DLSR_Data_To_Unity
Developed a custom reliable communication protocol (CRCP) in C++ for real-time video transmission, enabling seamless integration between a Canon DSLR camera and a Unity-based application. The camera captured live video, processed using computer vision (CV) techniques to detect and track objects within the frame. These objects were dynamically mapped onto a Unity grid, with bounding boxes assigned to each detected object, and collision detection implemented for interactions within the virtual environment. A PyQt-based interface provided intuitive control and monitoring, demonstrating expertise in network programming, real-time video processing, and 3D rendering in Unity.

Python Script:
Captures and processes video frames from the Canon DSLR.
Extracts bounding boxes for detected objects and sends this data to the C++ application over a socket.

C++ Custom Communication Protocol:
Receives bounding box data from the Python script.
Processes and forwards this data to Unity.

Unity:
Receives the bounding box data from the C++ application.
Creates corresponding objects in the Unity scene and handles collision detection.
