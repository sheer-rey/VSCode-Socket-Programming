/*******************************************************************************
 * @note Platform Definition
 * @param PROGRAMMING_ON_WINDOWS
 * @param PROGRAMMING_ON_LINUX
 * ****************************************************************************/
#define PROGRAMMING_ON_WINDOWS

/*******************************************************************************
 * @note Programming Under Windows Operating System
 * ****************************************************************************/
#ifdef PROGRAMMING_ON_WINDOWS
#include <iostream>
#include <cstring>
#include <windows.h>
#include <winsock2.h>

int main()
{
    /* Winsock Initialization Begin */
    WSAData wsa_data;
    if (WSAStartup(MAKEWORD(2, 2), &wsa_data) != 0)
        std::cerr << "WSAStartup() error!" << std::endl;
    /* Winsock Initialization End */

    /* Main Body Begin */
    // create client socket
    SOCKET client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == INVALID_SOCKET)
        std::cerr << "Server socket initialization fail..." << std::endl;
    else
        std::cout << "Server socket initialization successful..." << std::endl;

    // set client socket's address and connect to server
    SOCKADDR_IN server_addr = {};
    server_addr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(1314);
    if (connect(client_socket, (sockaddr *)&server_addr, sizeof(server_addr)) == SOCKET_ERROR)
        std::cerr << "Connect to server fail..." << std::endl;
    else
        std::cout << "Connect to server successful..." << std::endl;

    // receive data from server
    char message[256] = {};
    if (recv(client_socket, message, sizeof(message) - 1, 0) == -1)
        std::cerr << "Receive data error" << std::endl;
    else
        std::cout << "Received message: " << message << std::endl;

    // close client socket
    closesocket(client_socket);
    /* Main Body End */

    /* Winsock Cleanup Begin */
    WSACleanup();
    /* Winsock Cleanup End */
    return 0;
}

#endif

/*******************************************************************************
 * @note Programming Under Linux Operating System
 * ****************************************************************************/
#ifdef PROGRAMMING_ON_LINUX
#include <sys/socket.h>
#include <iostream>
int main()
{

    return 0;
}
#endif