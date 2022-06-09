#ifndef _FUNCTIONS_SERVER_
#define _FUNCTIONS_SERVER_
#include "global_definition.h"

#include <string>
#include <utility>
#include <vector>
#include "data_package.hpp"

namespace sheer_rey
{
	using std::pair;
	using std::string;
	using std::vector;

	int GetPackageHeader(SOCKET &_handled_socket, PackageHeader &_package_header);
	int EchoMessageServer(SOCKET &_handled_socket, PackageHeader &_package_header);
	int CalculatorServer(SOCKET &_handled_socket, PackageHeader &_package_header);
}

#endif
