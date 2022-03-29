#include "functions_client.h"
#include "data_package.hpp"
#include <iostream>
#include <cstring>

namespace sheer_rey
{
  using std::cin, std::cout, std::cerr, std::endl;
  using std::strcpy;
  using std::string, std::getline;

  int EchoMessage(SOCKET &client_socket)
  {
    cout << "Please enter the message send to server ("
         << cMaxBufferSize - 1 << " characters max, press enter to send): ";

    string str;
    getline(cin, str);
    while (!str.size())
    {
      cerr << "Error, empty line! Renter the message send to server ("
           << cMaxBufferSize - 1 << " characters max, press enter to send): ";
      getline(cin, str);
    }

    PackageHello message;
    if (str.size() > cMaxBufferSize - 1)
    {
      cerr << "Warning: the input greater than 127 characters, it will be trimmed." << endl;
      strcpy(message.message, str.substr(0, 127).c_str());
    }
    else
      strcpy(message.message, str.c_str());

    if (send(client_socket, (char *)&message, sizeof(message), 0) == SOCKET_ERROR)
    {
      cerr << "Send data error!" << endl;
      return -1;
    }
    else
    {
      cout << "Send to server successful!" << endl;
      PackageHeader package_header;
      int receive_lenth = 0;
      int receive_status = 0;
      while (receive_lenth < int(sizeof(package_header)))
      {
        receive_status = recv(client_socket, (char *)&package_header + receive_lenth,
                              sizeof(package_header) - receive_lenth, 0);
        if (receive_status == SOCKET_ERROR)
        {
          cerr << "Receive data error!" << endl;
          return -1;
        }
        else
          receive_lenth += receive_status;
      }

      switch (package_header.command)
      {
      case CMD_Hello:
        receive_lenth = receive_status = 0;
        message = package_header;
        while (receive_lenth < int(sizeof(message) - sizeof(package_header)))
        {
          receive_status = recv(client_socket,
                                (char *)&message + sizeof(package_header) + receive_lenth,
                                sizeof(message) - sizeof(package_header) - receive_lenth, 0);
          if (receive_status == SOCKET_ERROR)
          {
            cerr << "Receive data error!" << endl;
            return -1;
          }
          else
            receive_lenth += receive_status;
        }
        cout << "Receive from server successful!" << endl;
        cout << "Received data: " << message.message << endl;
        return 0;
        break;

      default:
        cerr << "Data Package Error!" << endl;
        return -1;
        break;
      }
    }
  }

  int Calculator(SOCKET &client_socket)
  {
    return -1;
  }
}
