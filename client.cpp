#include <iostream>
#include <winsock2.h>
#include <cstdlib>

#define DEFAULT_PORT 7
#define DEFAULT_ADDRESS INADDR_LOOPBACK

using namespace std;

int main(int argc, char **argv) {

    int port = DEFAULT_PORT;
    uint32_t address = htonl(DEFAULT_ADDRESS);

    // Get port and ip
    if(argc > 1) {
        port = atoi(argv[1]);
        if(port < 0 || port > 65535) {
            cerr << "Invalid port number supplied" << endl;
            return 1;
        }
    }
    if(argc > 2)
        address = inet_addr(argv[2]);

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

    // Connect
    struct sockaddr_in sockaddr;
    sockaddr.sin_family = AF_INET;
    sockaddr.sin_port = htons(port);
    sockaddr.sin_addr.s_addr = address;
    result = connect(sock, (struct sockaddr *) &sockaddr, sizeof(sockaddr_in));
    if (result == SOCKET_ERROR) {
        cerr << "Unable to connect. Error: " << WSAGetLastError << endl;
        WSACleanup();
        return EXIT_FAILURE;
    }

    while(true) {

        // Read text
        cout << ">> ";
        char buffer[100];
        gets(buffer);
        int length = strlen(buffer);
        buffer[length] = 0;
        if(strcmp(buffer, "exit") == 0)
            break;

        // Send
        int sent_bytes = send(sock, buffer, length, 0);
        if (sent_bytes == SOCKET_ERROR) {
            cerr << "Unable to send data. Error: " << WSAGetLastError << endl;
            WSACleanup();
            return EXIT_FAILURE;
        }
        printf("(%d bytes sent)", sent_bytes);

        // Receive
        size_t buffer_size = 100;
        char recv_buffer[buffer_size + 1];
        int bytes_received = recv(sock, recv_buffer, buffer_size, 0);
        recv_buffer[bytes_received] = 0;
        printf(recv_buffer);
        printf("(%d bytes received)\n", bytes_received);

    }

    // Close
    result = closesocket(sock);
    if(result == SOCKET_ERROR) {
        cerr << "Unable to close socket. Error: " << WSAGetLastError << endl;
        WSACleanup();
        return EXIT_FAILURE;
    }

    WSACleanup();
    return EXIT_SUCCESS;
}