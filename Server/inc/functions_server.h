#ifndef _FUNCTIONS_SERVER_
#define _FUNCTIONS_SERVER_
#include "global_definition.h"

#include <string>
#include <utility>
#include <vector>
#include "data_package.hpp"

namespace sheer_rey {
  using std::pair;
  using std::string;
  using std::vector;

  int GetPackageHeader(const SOCKET& _handled_socket, PackageHeader& _package_header);
  int EchoMessageServer(const SOCKET& _handled_socket,
                        PackageHeader& _package_header);
  int CalculatorServer(const SOCKET& _handled_socket, PackageHeader& _package_header);
}  // namespace sheer_rey

#endif
