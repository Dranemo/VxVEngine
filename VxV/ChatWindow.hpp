#define _WINSOCK_DEPRECATED_NO_WARNINGS
#pragma once

#include <deque>
#include <string>
#include <thread>
#include <mutex>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <imgui.h>

#pragma comment(lib, "Ws2_32.lib")

struct ChatWindow {
    std::string username;
    std::deque<std::string> messages;
    char inputBuf[256];
    SOCKET clientSocket;
    std::string serverIP = "Your Ipv4 Adress";  // Adresse IP par d�faut
    std::thread receiveThread;
    std::mutex mutex;
    bool isConnected = false;

    ChatWindow() {
        memset(inputBuf, 0, sizeof(inputBuf));
    }

    ~ChatWindow() {
        closeNetwork();
    }

    void setupConnection(const std::string& user, const std::string& ip) {
        username = user;
        serverIP = ip;
        initNetwork();
    }

    void initNetwork() {
        WSADATA wsaData;
        int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
        if (result != 0) {
            addMessage("Winsock initialization failed.");
            return;
        }

        clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        if (clientSocket == INVALID_SOCKET) {
            WSACleanup();
            addMessage("Socket creation failed.");
            return;
        }

        sockaddr_in serverAddr;
        serverAddr.sin_family = AF_INET;
        serverAddr.sin_port = htons(12345);
        inet_pton(AF_INET, serverIP.c_str(), &serverAddr.sin_addr);

        if (connect(clientSocket, (SOCKADDR*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
            closesocket(clientSocket);
            WSACleanup();
            addMessage("Connection failed.");
            return;
        }
        send(clientSocket, username.c_str(), username.length(), 0); // Send username right after connecting
        receiveThread = std::thread(&ChatWindow::receiveMessages, this);
        receiveThread.detach();  // It's detached to prevent blocking.
        isConnected = true;
    }

    void closeNetwork() {
        if (clientSocket != INVALID_SOCKET) {
            closesocket(clientSocket);
        }
        WSACleanup();
        if (receiveThread.joinable()) {
            receiveThread.join();
        }
    }

    void receiveMessages() {
        char buffer[1024];
        while (true) {
            int bytesReceived = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);
            if (bytesReceived > 0) {
                buffer[bytesReceived] = '\0';
                addMessage(std::string(buffer));
            }
            else {
                if (bytesReceived == 0) {
                    addMessage("Connection closed by server.");
                }
                else {
                    addMessage("Error receiving message.");
                }
                break;
            }
        }
    }

    void addMessage(const std::string& message) {
        std::lock_guard<std::mutex> lock(mutex);
        messages.push_back(message);
    }

    void sendMessage(const std::string& message) {
        std::string fullMessage = username + ": " + message;
        int bytesSent = send(clientSocket, fullMessage.c_str(), fullMessage.length(), 0);
        if (bytesSent == SOCKET_ERROR) {
            addMessage("Error sending message: " + std::to_string(WSAGetLastError()));
        }
        else {
            addMessage("You: " + message);
        }
    }

    void DrawLogin() {
        static char usernameBuf[256] = "";
        static char serverIPBuf[256] = "Your Ipv4 Adress";
        if (!isConnected) {  // Ne montrer la fen�tre de login que si non connect�
            ImGui::Begin("Login");
            ImGui::InputText("Username", usernameBuf, sizeof(usernameBuf));
            ImGui::InputText("Server IP", serverIPBuf, sizeof(serverIPBuf));
            if (ImGui::Button("Connect")) {
                setupConnection(std::string(usernameBuf), std::string(serverIPBuf));
                if (isConnected) {
                    // La connexion a r�ussi, pr�parer l'interface pour le chat
                    memset(usernameBuf, 0, sizeof(usernameBuf));  // Clear buffers after successful login
                    memset(serverIPBuf, 0, sizeof(serverIPBuf));
                    ImGui::End();  // Fermer la fen�tre de login
                    return;  // S'assurer de ne pas redessiner la fen�tre de login
                }
            }
            ImGui::End();
        }
    }

    void Draw() {
        if (!isConnected) {
            DrawLogin();
        }
        else {
            ImGui::Begin("Chat");
            if (ImGui::BeginChild("ScrollingRegion", ImVec2(0, -ImGui::GetFrameHeightWithSpacing()), false, ImGuiWindowFlags_HorizontalScrollbar)) {
                for (const auto& message : messages) {
                    ImGui::TextUnformatted(message.c_str());
                }
                if (ImGui::GetScrollY() >= ImGui::GetScrollMaxY()) {
                    ImGui::SetScrollHereY(1.0f);
                }
            }
            ImGui::EndChild();

            bool reclaim_focus = false;
            if (ImGui::InputText("Input", inputBuf, sizeof(inputBuf), ImGuiInputTextFlags_EnterReturnsTrue)) {
                std::string message = inputBuf;
                if (!message.empty()) {
                    sendMessage(message);
                    reclaim_focus = true;
                }
                memset(inputBuf, 0, sizeof(inputBuf));
            }
            if (reclaim_focus) {
                ImGui::SetKeyboardFocusHere(-1);
            }
            ImGui::End();
        }
    }
};
