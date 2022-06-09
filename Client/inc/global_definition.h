#ifndef _GLOBAL_DEFINITION_H_
#define _GLOBAL_DEFINITION_H_
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

/*******************************************************************************
 * @note Programming Under Windows Operating System
 * ****************************************************************************/
#ifdef PROGRAMMING_ON_WINDOWS
#include <windows.h>
#include <winsock2.h>
typedef int socklen_t;
#endif

/*******************************************************************************
 * @note Programming Under Linux Operating System
 * ****************************************************************************/
#ifdef PROGRAMMING_ON_LINUX
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>

#define closesocket close
#define SOCKET_ERROR -1
#define INVALID_SOCKET -1
typedef int SOCKET;
#endif

#endif
