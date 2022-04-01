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

#include "data_package.hpp"
#include "functions_server.h"

int main()
{
    using namespace sheer_rey;
    using std::cout, std::cin, std::cerr, std::endl;
    using std::memset, std::strcpy;
    using std::pair;
    using std::string;

    /* Winsock Initialization Begin */
    WSAData wsa_data;
    if (WSAStartup(MAKEWORD(2, 2), &wsa_data) != 0)
        cerr << "WSAStartup() error!" << endl;
    /* Winsock Initialization End */

    /* Main Body Begin */

    /* ↓ create server socket ↓ */
    SOCKET server_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (server_socket == INVALID_SOCKET)
        cerr << "Server socket initialization fail..." << endl;
    else
        cout << "Server socket initialization successful..." << endl;

    /* ↓ set server socket's address and bind ↓ */
    SOCKADDR_IN server_addr;
    server_addr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(1314);
    if (bind(server_socket, (sockaddr *)&server_addr, sizeof(server_addr)) == SOCKET_ERROR)
        cerr << "Bind port fail..." << endl;
    else
        cout << "Bind port successful..." << endl;

    /* ↓ listen to binded port ↓ */
    if (listen(server_socket, 5) == SOCKET_ERROR)
        cerr << "Listen to port fail..." << endl;
    else
        cout << "Listen to port successful..." << endl;

    /* ↓ create socket for handling current request ↓ */
    SOCKET handled_socket = INVALID_SOCKET;
    SOCKADDR_IN client_addr;
    int client_addr_lenth = sizeof(client_addr);

    while (true)
    {
        /* ↓ accept client request ↓ */
        handled_socket = accept(server_socket, (sockaddr *)&client_addr, &client_addr_lenth);
        if (handled_socket == INVALID_SOCKET)
            cerr << "accept client socket error..." << endl;
        else
            cout << "accept client socket success..." << endl;
        cout << "Client Addr: " << inet_ntoa(client_addr.sin_addr)
             << ':' << ntohs(client_addr.sin_port) << endl;

        /* ↓ exchange data with client ↓ */
        PackageHeader package_header;
        int receive_status = 0;
        int receive_lenth = 0;

        // get package header first
        while (receive_lenth < int(sizeof(package_header)))
        {
            receive_status = recv(handled_socket,
                                  (char *)&package_header + receive_lenth,
                                  int(sizeof(package_header)) - receive_lenth, 0);
            if (receive_status == SOCKET_ERROR)
                cerr << "Receive data error!" << endl;
            else
                receive_lenth += receive_status;
        }

        // check request type
        switch (package_header.command)
        {
        case CMD_Hello:
        {
            /* ↓ echo message request ↓ */
            PackageHello message(package_header);
            receive_lenth = receive_status = 0;

            // get package content except header
            while (receive_lenth < int(sizeof(message) - sizeof(package_header)))
            {
                receive_status = recv(handled_socket,
                                      (char *)&message + int(sizeof(package_header)) + receive_lenth,
                                      int(sizeof(message) - sizeof(package_header)) - receive_lenth, 0);
                if (receive_status == SOCKET_ERROR)
                    cerr << "Receive data error!" << endl;
                else
                    receive_lenth += receive_status;
            }

            // echo received message
            send(handled_socket, (char *)&message, sizeof(message), 0);
            break;
        }
        case CMD_Calculator:
        {
            /* ↓ calculator request ↓ */
            PackageCalculator calculator(package_header);
            receive_lenth = receive_status = 0;

            // get package content except header
            while (receive_lenth < int(sizeof(calculator) - sizeof(package_header)))
            {
                receive_status = recv(handled_socket,
                                      (char *)&calculator + int(sizeof(package_header)) + receive_lenth,
                                      int(sizeof(calculator) - sizeof(package_header)) - receive_lenth, 0);
                if (receive_status == SOCKET_ERROR)
                    cerr << "Receive data error!" << endl;
                else
                    receive_lenth += receive_status;
            }

            /* ↓ check received package ↓ */
            if (calculator.calculation_status == cUnCalculate && calculator.is_result == false)
            {
                string infix(calculator.infix_expression);
                pair<CalculateStatus, double> calculate_result = Calculator(infix);

                // pack calculation result
                memset(calculator.infix_expression, 0, cMaxBufferSize);
                strcpy(calculator.infix_expression, infix.c_str());
                calculator.is_result = true;
                calculator.calculation_status = calculate_result.first;
                calculator.result = calculate_result.second;

                // send calculation result
                send(handled_socket, (char *)&calculator, sizeof(calculator), 0);
            }
            else
                cout << "Unidentified command." << endl;
            break;
        }
        default:
            cerr << "Unidentified command." << endl;
            break;
        }
    }

    /* ↓ close socket ↓ */
    closesocket(server_socket);
    closesocket(handled_socket);
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