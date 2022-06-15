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
#include <list>
#include <utility>

#include "data_package.hpp"
#include "functions_server.h"

int main() {
  using namespace sheer_rey;
  using std::cout, std::cin, std::cerr, std::endl;
  using std::list;
  using std::memset, std::strcpy;
  using std::pair, std::make_pair;
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

  /* Select Function Initialization Begin */
  fd_set read_fds_backup = {};
  timeval timeout_backup = {0, 0};
  FD_ZERO(&read_fds_backup);
  FD_SET(server_socket, &read_fds_backup);
  // ↓ recordig opened sockets by using list, forward_list or set.
  // ↓ do not use vector or unordered_set because its iterator may be invalid
  // ↓ while insert, remove elements or force rehash.
  // ↓ using pair to store both socket and associated socket address
  list<pair<SOCKET, sockaddr_in>> sockets = {
      make_pair(server_socket, server_addr)};
  // ↓ recording sockets to be remove from sockets list, and remove them
  // ↓ together after the traverse of sockets list.
  // ↓ do not remove sockets during the list traversal, otherwise, the invalid
  // ↓ iterator of removed element will interrupt the list traversal and throw
  // ↓ an exception
  vector<SOCKET> sockets_to_be_remove;
  // ↓ only valid in Linux, to specify the max file descriptors in select poll
  int poll_fds_max = server_socket + 1;
  /* Select Function Initialization End */

  while (true) {
    // ↓ update the read_fds and timeout value for new round of select
    // ↓ function's inquiry
    fd_set read_fds = read_fds_backup;
    timeval timeout = timeout_backup;
    sockets_to_be_remove.clear();
    int read_nums = select(poll_fds_max, &read_fds, nullptr, nullptr, &timeout);
    if (read_nums == -1) {
      cerr << "Select function error..." << endl;
      break;
    } else if (read_nums == 0) {
      continue;
    }

    // there's some file descriptors(here is sockets) are ready to read
    for (const auto& [fd, sock_addr] : sockets) {
      if (FD_ISSET(fd, &read_fds)) {
        if (fd == server_socket) {
          /* ↓ for server socket, accept client's request ↓ */
          cout << "Accept client socket..." << endl;
          handled_socket = accept(server_socket, (sockaddr*)&client_addr,
                                  &client_addr_lenth);
          if (handled_socket == INVALID_SOCKET) {
            cerr << "Accept client socket error..." << endl;
            continue;
          }
          // ↓ accept client's request successful, then add socket address to
          // ↓ list and add hanled socket to both list and read_fds_backup.
          // ↓ add socket to read_fds_backup rather than read_fds to prevent the
          // ↓ influence to curent read_fds status returned by select function
          sockets.push_back(make_pair(handled_socket, client_addr));
          FD_SET(handled_socket, &read_fds_backup);

          // ↓ only valid in Linux, update the poll_fds_max for select function
          if (static_cast<int>(handled_socket) >= poll_fds_max)
            poll_fds_max = handled_socket + 1;

          cout << "Client Addr: " << inet_ntoa(client_addr.sin_addr) << ':'
               << ntohs(client_addr.sin_port) << endl;
        } else {
          /* ↓ for other handled sockets, exchange data with client ↓ */
          PackageHeader package_header;
          if (GetPackageHeader(fd, package_header) == -2) {
            // ↓ client has performed an orderly shutdown, then close the
            // ↓ corresponding socket and remove socket and socket address from
            // ↓ list and read_fds_backup remove socket from read_fds_backup
            // ↓ rather than read_fds to prevent the influence to curent
            // ↓ read_fds status returned by select function
            sockets_to_be_remove.push_back(fd);
            FD_CLR(fd, &read_fds_backup);

            cout << "Client " << inet_ntoa(sock_addr.sin_addr) << ':'
                 << ntohs(sock_addr.sin_port) << " disconnect..." << endl;
            closesocket(fd);
            continue;
          }

          // check request type
          switch (package_header.command) {
            case CMD_Hello: {
              /* ↓ echo message request ↓ */
              EchoMessageServer(fd, package_header);
              break;
            }
            case CMD_Calculator: {
              /* ↓ calculator request ↓ */
              CalculatorServer(fd, package_header);
              break;
            }
            default: {
              cerr << "Unidentified command, ";
              // ↓ if client send an unidentified command, then close the
              // ↓ corresponding socket and remove socket and socket address
              // ↓ from list and read_fds_backup remove socket from
              // ↓ read_fds_backup rather than read_fds to prevent the influence
              // ↓ to curent read_fds status returned by select function
              sockets_to_be_remove.push_back(fd);
              FD_CLR(fd, &read_fds_backup);

              cout << "force disconnect from client "
                   << inet_ntoa(sock_addr.sin_addr) << ':'
                   << ntohs(sock_addr.sin_port) << "..." << endl;
              closesocket(fd);
              break;
            }
          }
        }
      }
    }

    // ↓ remove sockets recording in the traversal loop above
    for (auto fd : sockets_to_be_remove) {
      sockets.remove_if(
          [fd](pair<SOCKET, sockaddr_in>& x) { return x.first == fd; });
    }
  }

/* ↓ close server socket ↓ */
#ifdef PROGRAMMING_ON_WINDOWS
  closesocket(server_socket);
#endif
#ifdef PROGRAMMING_ON_LINUX
  closesocket(server_socket);
#endif
  /* Main Body End */

  /* Winsock Cleanup Begin */
#ifdef PROGRAMMING_ON_WINDOWS
  WSACleanup();
/* Winsock Cleanup End */
#endif

  return 0;
}
