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
#include "data_package.hpp"
#include "functions_server.h"

int main() {
  using namespace sheer_rey;
  using std::cout, std::cin, std::cerr, std::endl;
  using std::memset, std::strcpy;
  using std::pair;
  using std::string;

  /* Winsock for Windows Initialization Begin */
#ifdef PROGRAMMING_ON_WINDOWS
  cout << "Winsock api initialization..." << endl;
  WSAData wsa_data;
  if (WSAStartup(MAKEWORD(2, 2), &wsa_data) != 0) {
    cerr << "Winsock api initialization failed..." << endl;
    exit(EXIT_FAILURE);
  }
#endif
  /* Winsock for Windows Initialization End */

  /* Main Body Begin */

  /* ↓ create server socket ↓ */
  cout << "Server socket initialization..." << endl;
  SOCKET server_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  if (server_socket == INVALID_SOCKET) {
    cerr << "Server socket initialization failed..." << endl;
    exit(EXIT_FAILURE);
  }

  /* ↓ set server socket's address and bind ↓ */
  sockaddr_in server_addr{};

#ifdef PROGRAMMING_ON_WINDOWS
  server_addr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
#endif

#ifdef PROGRAMMING_ON_LINUX
  server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
#endif

  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(IP_ADDR_PORT_SERVER);

  cout << "Bind port..." << endl;
  if (bind(server_socket, (sockaddr*)&server_addr, sizeof(server_addr)) ==
      SOCKET_ERROR) {
    cerr << "Bind port failed..." << endl;
    closesocket(server_socket);
    exit(EXIT_FAILURE);
  }

  /* ↓ listen to binded port ↓ */
  cout << "Listen to port..." << endl;
  if (listen(server_socket, SOMAXCONN) == SOCKET_ERROR) {
    cerr << "Listen to port failed..." << endl;
    closesocket(server_socket);
    exit(EXIT_FAILURE);
  }

  /* ↓ create socket for handling current request ↓ */
  SOCKET handled_socket = INVALID_SOCKET;
  sockaddr_in client_addr;
  socklen_t client_addr_lenth = sizeof(client_addr);

  while (true) {
    /* ↓ accept client request ↓ */
    cout << "Accept client socket..." << endl;
    handled_socket =
        accept(server_socket, (sockaddr*)&client_addr, &client_addr_lenth);
    if (handled_socket == INVALID_SOCKET) {
      cerr << "Accept client socket error..." << endl;
      closesocket(server_socket);
      exit(EXIT_FAILURE);
    }

    cout << "Client Addr: " << inet_ntoa(client_addr.sin_addr) << ':'
         << ntohs(client_addr.sin_port) << endl;

    while (true) {
      /* ↓ exchange data with client ↓ */
      PackageHeader package_header;
      if (GetPackageHeader(handled_socket, package_header) == -2) break;

      // check request type
      switch (package_header.command) {
        case CMD_Hello: {
          /* ↓ echo message request ↓ */
          EchoMessageServer(handled_socket, package_header);
          break;
        }
        case CMD_Calculator: {
          /* ↓ calculator request ↓ */
          CalculatorServer(handled_socket, package_header);
          break;
        }
        default: cerr << "Unidentified command." << endl; break;
      }
    }
    cout << "Client " << inet_ntoa(client_addr.sin_addr) << ':'
         << ntohs(client_addr.sin_port) << " disconnect..." << endl;
    closesocket(handled_socket);
  }

/* ↓ close server socket ↓ */
#ifdef PROGRAMMING_ON_WINDOWS
  closesocket(server_socket);
#endif
#ifdef PROGRAMMING_ON_LINUX
  close(server_socket);
#endif
  /* Main Body End */

  /* Winsock Cleanup Begin */
#ifdef PROGRAMMING_ON_WINDOWS
  WSACleanup();
/* Winsock Cleanup End */
#endif

  return 0;
}
