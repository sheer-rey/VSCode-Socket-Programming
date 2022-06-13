/*******************************************************************************
 * @note Platform Definition
 * @param PROGRAMMING_ON_WINDOWS
 * @param PROGRAMMING_ON_LINUX
 * @attention You need to add global difinition to compiler command line
 * parameter
 * @example For GCC, in Windows platform, you need add
 * "-DPROGRAMMING_ON_WINDOWS" parameter to g++ command line. In Linux platform,
 * you need add
 * "-DPROGRAMMING_ON_LINUX" parameter to g++ command line.
 * ****************************************************************************/
#include "global_definition.h"

#include <cstring>
#include <iostream>
#include <string>
#include "data_package.hpp"
#include "functions_client.h"

int main() {
  using namespace sheer_rey;
  using std::cout, std::cin, std::cerr, std::endl;
  using std::string;

  /* Winsock Initialization Begin */
#ifdef PROGRAMMING_ON_WINDOWS
  cout << "Winsock api initialization..." << endl;
  WSAData wsa_data;
  if (WSAStartup(MAKEWORD(2, 2), &wsa_data) != 0) {
    cerr << "Winsock api initialization failed..." << endl;
    exit(EXIT_FAILURE);
  }
#endif
  /* Winsock Initialization End */

  /* Main Body Begin */

  /* ↓ create client socket ↓ */
  cout << "Client socket initialization..." << endl;
  SOCKET client_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  if (client_socket == INVALID_SOCKET) {
    cerr << "Client socket initialization failed..." << endl;
    exit(EXIT_FAILURE);
  }

  /* ↓ set client socket's address and connect to server ↓ */
  sockaddr_in server_addr{};

#ifdef PROGRAMMING_ON_WINDOWS
  server_addr.sin_addr.S_un.S_addr = IP_ADDR_SERVER;
#endif

#ifdef PROGRAMMING_ON_LINUX
  server_addr.sin_addr.s_addr = IP_ADDR_SERVER;
#endif

  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(IP_ADDR_PORT_SERVER);

  // connect to server
  cout << "Connect to server..." << endl;
  if (connect(client_socket, (sockaddr*)&server_addr, sizeof(server_addr)) ==
      SOCKET_ERROR) {
    cerr << "Connect to server failed..." << endl;
    closesocket(client_socket);
    exit(EXIT_FAILURE);
  }

  /* ↓ exchange data with server and user ↓ */
  cout << "\nWhat you want to do? (enter the number, q to quit): " << endl;
  cout << "1. Send message to server and echo." << endl;
  cout << "2. Send infix expression to server's calculator." << endl;

  string command_line;
  bool quit_flag = false;
  while (true) {
    cin >> command_line;
    cin.sync();
    if (command_line.size() > 1) {
      cerr << "Error input, please renter! (enter the number, q to quit)"
           << endl;
      cout << "1. Send message to server and echo." << endl;
      cout << "2. Send infix expression to server's calculator." << endl;
      continue;
    } else {
      switch (command_line.at(0)) {
        case '1': EchoMessageClient(client_socket); break;
        case '2': CalculatorClient(client_socket); break;
        case 'Q':
        case 'q': quit_flag = true; break;
        default: cerr << "Unidentified command." << endl; break;
      }
    }

    if (quit_flag) { break; }
    cout << "\nWhat you want to do next? (enter the number, q to quit): "
         << endl;
    cout << "1. Send message to server and echo." << endl;
    cout << "2. Send infix expression to server's calculator." << endl;
  }

  /* ↓ close client socket ↓ */
#ifdef PROGRAMMING_ON_WINDOWS
  closesocket(client_socket);
#endif
#ifdef PROGRAMMING_ON_LINUX
  close(client_socket);
#endif
  /* Main Body End */

  /* Winsock Cleanup Begin */
#ifdef PROGRAMMING_ON_WINDOWS
  WSACleanup();
/* Winsock Cleanup End */
#endif

  return 0;
}
