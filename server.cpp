#include <iostream>
#include <winsock2.h>
#include <cstdlib>
#include <unistd.h>
#include <windows.h>

#define DEFAULT_PORT 8

using namespace std;

bool running = true;

int main(int argc, char **argv) {

    int port = DEFAULT_PORT;

    // Get port
    if(argc > 1) {
        port = atoi(argv[1]);
        if(port < 0 || port > 65535) {
            cerr << "Invalid port number supplied" << endl;
            return 1;
        }
    }
    cout << "Server is listening on port " << port << endl;

    // WSAStartup
    WSADATA wsadata;
    int result = WSAStartup(MAKEWORD(2, 2), &wsadata);
    if (result != 0) {
        cerr << "Unable to initialize winsock. Error: " << WSAGetLastError() << endl;
        return EXIT_FAILURE;
    }

    // Socket
    SOCKET sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock == INVALID_SOCKET) {
        cerr << "Unable to create socket. Error: " << WSAGetLastError() << endl;
        WSACleanup();
        return EXIT_FAILURE;
    }

    // Bind
    struct sockaddr_in sockaddr = {0};
    sockaddr.sin_family = AF_INET;
    sockaddr.sin_port = htons(port);
    sockaddr.sin_addr.s_addr = INADDR_ANY;
    result = bind(sock, (struct sockaddr *) &sockaddr, sizeof(sockaddr_in));
    if (result == SOCKET_ERROR) {
        cerr << "Unable to bind. Error: " << WSAGetLastError << endl;
        WSACleanup();
        return EXIT_FAILURE;
    }

    struct sockaddr_in client_address;
    int client_address_len = sizeof(client_address);

    while(running) {

        // Receive
        char buffer[1000];
        int received_bytes = recvfrom(sock, buffer, 1000, 0, (struct sockaddr *)&client_address, &client_address_len);
        cout << received_bytes << endl;
        if (received_bytes <= 0) continue;

        // Display info
        string msg(buffer, received_bytes);
        cout << "Client " << inet_ntoa(client_address.sin_addr)  << " SAYS: " << msg;
        cout << " (" << received_bytes << " bytes)" << endl;

        // Send
        result = sendto(sock, buffer, received_bytes, 0, (struct sockaddr *)&client_address, sizeof(client_address));
        if (result == SOCKET_ERROR) continue;
    }

    // close socket
    result = closesocket(sock);
    if(result == SOCKET_ERROR) {
        cerr << "Unable to close socket. Error: " << WSAGetLastError << endl;
        WSACleanup();
        exit(EXIT_FAILURE);
    }

    // WSACleanup
    WSACleanup();

    // Display info
    cout << "Server closed" << endl;
    sleep(5);
    exit(EXIT_SUCCESS);
}