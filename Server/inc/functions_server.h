#ifndef _FUNCTIONS_SERVER_
#define _FUNCTIONS_SERVER_

#include <string>
#include <utility>
#include <vector>
#include <winsock2.h>

#include "data_package.hpp"

namespace sheer_rey
{
  using std::pair;
  using std::string;
  using std::vector;

  int EchoMessage(SOCKET server_socket);
  pair<CalculateStatus, double> Calculator(string &infix_expression);
}

#endif
