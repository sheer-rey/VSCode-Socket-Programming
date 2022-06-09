#ifndef _FUNCTIONS_CLIENT_
#define _FUNCTIONS_CLIENT_
#include "global_definition.h"

#include <cstring>
#include <iostream>
#include <utility>

#include "data_package.hpp"

namespace sheer_rey {
  int EchoMessageClient(SOCKET& _client_socket);
  int CalculatorClient(SOCKET& _client_socket);
}  // namespace sheer_rey

#endif
