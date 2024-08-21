#include <iostream>
#include <vector>
#include <opencv2/opencv.hpp>
#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "Ws2_32.lib")

struct BoundingBox {
    int x, y, width, height;
};

// Function to receive raw frames from Python
cv::Mat receiveFrame(SOCKET sock) {
    std::vector<char> buffer(1024*1024); // Adjust size as needed
    int bytesReceived = recv(sock, buffer.data(), buffer.size(), 0);
    std::vector<uchar> data(buffer.begin(), buffer.begin() + bytesReceived);
    cv::Mat frame = cv::imdecode(data, cv::IMREAD_COLOR);
    return frame;
}

// Perform object detection using OpenCV
std::vector<BoundingBox> detectObjects(const cv::Mat& frame) {
    cv::Mat gray, edges;
    cv::cvtColor(frame, gray, cv::COLOR_BGR2GRAY);
    cv::Canny(gray, edges, 50, 150);
    std::vector<std::vector<cv::Point>> contours;
    cv::findContours(edges, contours, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE);

    std::vector<BoundingBox> boxes;
    for (const auto& cnt : contours) {
        cv::Rect rect = cv::boundingRect(cnt);
        boxes.push_back({ rect.x, rect.y, rect.width, rect.height });
    }
    return boxes;
}

// Function to forward bounding boxes to Unity
void sendDataToUnity(const std::vector<BoundingBox>& boxes) {
    SOCKET unitySocket;
    sockaddr_in unityAddr;

    unitySocket = socket(AF_INET, SOCK_STREAM, 0);
    unityAddr.sin_family = AF_INET;
    unityAddr.sin_port = htons(65432);
    inet_pton(AF_INET, "127.0.0.1", &unityAddr.sin_addr);

    connect(unitySocket, (sockaddr*)&unityAddr, sizeof(unityAddr));

    for (const auto& box : boxes) {
        send(unitySocket, reinterpret_cast<const char*>(&box), sizeof(box), 0);
    }

    closesocket(unitySocket);
}

int main() {
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);

    SOCKET listenSocket = socket(AF_INET, SOCK_STREAM, 0);
    sockaddr_in serverAddr;

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(65431);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    bind(listenSocket, (sockaddr*)&serverAddr, sizeof(serverAddr));
    listen(listenSocket, 5);

    std::cout << "Waiting for connection..." << std::endl;

    SOCKET clientSocket = accept(listenSocket, nullptr, nullptr);
    std::cout << "Connected to Python application." << std::endl;

    while (true) {
        cv::Mat frame = receiveFrame(clientSocket);
        std::vector<BoundingBox> boxes = detectObjects(frame);
        sendDataToUnity(boxes);
    }

    closesocket(clientSocket);
    closesocket(listenSocket);
    WSACleanup();

    return 0;
}

