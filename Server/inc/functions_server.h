#ifndef _FUNCTIONS_SERVER_
#define _FUNCTIONS_SERVER_

#include <string>
#include <vector>
#include <winsock2.h>

namespace shher_rey
{
  using std::string;
  using std::vector;

  int EchoMessage(SOCKET server_socket);

  vector<string> Infix2Suffix(const string &infix_expression);
  double CalculatorOfSuffix(const vector<string> &suffix_expression);
}

#endif
