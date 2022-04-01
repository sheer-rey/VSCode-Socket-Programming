#ifndef _FUNCTIONS_CLIENT_
#define _FUNCTIONS_CLIENT_

#include <iostream>
#include <cstring>
#include <winsock2.h>
#include <utility>

#include "data_package.hpp"

namespace sheer_rey
{
  int EchoMessage(SOCKET &client_socket);
  int SendInfixExpression(SOCKET &client_socket);
  int ShowCalculationResult(SOCKET &client_socket);
}

#endif
