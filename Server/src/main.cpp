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
    // create server socket
    SOCKET server_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (server_socket == INVALID_SOCKET)
        std::cerr << "Server socket initialization fail..." << std::endl;
    else
        std::cout << "Server socket initialization successful..." << std::endl;

    // set server socket's address and bind
    SOCKADDR_IN server_addr;
    server_addr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(1314);
    if (bind(server_socket, (sockaddr *)&server_addr, sizeof(server_addr)) == SOCKET_ERROR)
        std::cerr << "Bind port fail..." << std::endl;
    else
        std::cout << "Bind port successful..." << std::endl;

    // listen to binded port
    if (listen(server_socket, 5) == SOCKET_ERROR)
        std::cerr << "Listen to port fail..." << std::endl;
    else
        std::cout << "Listen to port successful..." << std::endl;

    // create client socket
    SOCKET server_client = INVALID_SOCKET;
    SOCKADDR_IN client_addr;
    int client_addr_lenth = sizeof(client_addr);

    while (true)
    {
        // accept client request
        server_client = accept(server_socket, (sockaddr *)&client_addr, &client_addr_lenth);
        if (server_client == INVALID_SOCKET)
            std::cerr << "accept client socket error..." << std::endl;
        else
            std::cout << "accept client socket success..." << std::endl;
        std::cout << "Client Addr: " << inet_ntoa(client_addr.sin_addr)
                  << ':' << ntohs(client_addr.sin_port) << std::endl;
        const char message[] = "Hello, I'm server!\n";
        send(server_client, message, std::strlen(message), 0);
    }

    // close socket
    closesocket(server_socket);
    closesocket(server_client);
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