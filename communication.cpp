#include <iostream>
#include <vector>
#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "Ws2_32.lib")

struct BoundingBox {
    int x, y, width, height;
};

// Function to receive bounding boxes from Python
std::vector<BoundingBox> receiveData(SOCKET sock) {
    std::vector<BoundingBox> boxes;
    char buffer[16]; // 4 integers * 4 bytes each

    while (true) {
        int bytesReceived = recv(sock, buffer, sizeof(buffer), 0);
        if (bytesReceived <= 0) break; // Break if no more data is received

        BoundingBox box;
        memcpy(&box, buffer, sizeof(BoundingBox));
        boxes.push_back(box);
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
        std::vector<BoundingBox> boxes = receiveData(clientSocket);
        sendDataToUnity(boxes);
    }

    closesocket(clientSocket);
    closesocket(listenSocket);
    WSACleanup();

    return 0;
}
