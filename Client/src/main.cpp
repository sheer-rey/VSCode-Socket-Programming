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
#include <string>
#include <cstring>
#include <windows.h>
#include <winsock2.h>

#include "data_package.hpp"
#include "functions_client.h"

int main()
{
    using namespace sheer_rey;
    using std::cout, std::cin, std::cerr, std::endl;
    using std::string;

    /* Winsock Initialization Begin */
    WSAData wsa_data;
    if (WSAStartup(MAKEWORD(2, 2), &wsa_data) != 0)
        cerr << "WSAStartup() error!" << endl;
    /* Winsock Initialization End */

    /* Main Body Begin */
    // create client socket
    SOCKET client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == INVALID_SOCKET)
        cerr << "Server socket initialization fail..." << endl;
    else
        cout << "Server socket initialization successful..." << endl;

    // set client socket's address and connect to server
    SOCKADDR_IN server_addr = {};
    server_addr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(1314);
    if (connect(client_socket, (sockaddr *)&server_addr, sizeof(server_addr)) == SOCKET_ERROR)
        cerr << "Connect to server fail..." << endl;
    else
        cout << "Connect to server successful..." << endl;

    // exchange data with server
    cout << "What you want to do? (enter the number, q to quit): " << endl;
    cout << "1. Send message to server and echo." << endl;
    cout << "2. Send infix expression to server's calculator." << endl;
    string command_line;
    bool quit_flag = false;
    while (!quit_flag)
    {
        cin >> command_line;
        cin.sync();
        if (command_line.size() > 1)
        {
            cerr << "Error input, please renter! (enter the number, q to quit)" << endl;
            cout << "1. Send message to server and echo." << endl;
            cout << "2. Send infix expression to server's calculator." << endl;
            continue;
        }
        else
        {
            switch (command_line.at(0))
            {
            case '1':
                EchoMessage(client_socket);
                break;

            case '2':
                /* code */
                break;

            case 'q':
                quit_flag = true;
                break;

            default:
                cerr << "Unidentified command." << endl;
                break;
            }
        }
        cout << "\nWhat you want to do next? (enter the number, q to quit): " << endl;
        cout << "1. Send message to server and echo." << endl;
        cout << "2. Send infix expression to server's calculator." << endl;
    }

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