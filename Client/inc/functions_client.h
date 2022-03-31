#ifndef _FUNCTIONS_CLIENT_
#define _FUNCTIONS_CLIENT_

#include <string>
#include <winsock2.h>

namespace sheer_rey
{
  using std::string;

  int EchoMessage(SOCKET &client_socket);
  int SendInfixExpression(SOCKET &client_socket);
}
#endif
