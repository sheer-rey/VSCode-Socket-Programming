#include "functions_client.h"

namespace sheer_rey
{
  using std::cin, std::cout, std::cerr, std::endl;
  using std::pair;
  using std::strcpy;
  using std::string, std::getline;

  /*****************************************************************************
   * @name EchoMessage
   * @param client_socket client's windows socket handle
   * @retval -1: echo error
   * @retval 0 : echo success
   * @brief Send message to server and get the echo
   * @note Get a line of input from keyboard (max number of input defined by
   *       cMaxBufferSize), then send to server specified by \p client_socket.
   *       Finally get the echo message from server and output.
   * **************************************************************************/
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

    /* pack the input line and sent to server */
    PackageHello message;
    if (str.size() > cMaxBufferSize - 1)
    {
      cerr << "Warning: the input is greater than 127 characters, it will be trimmed." << endl;
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

      /* Get server's echo */
      PackageHeader package_header;
      int receive_lenth = 0;
      int receive_status = 0;

      // get package head first
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

      // check package type received from server
      if (package_header.command == CMD_Hello)
      {
        // get package content except header
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
      }
      else
      {
        cerr << "Data Package Error!" << endl;
        return -1;
      }
    }
  }

  /*****************************************************************************
   * @name SendInfixExpression
   * @param client_socket client's windows socket handle
   * @retval -1: send error
   * @retval 0 : send success
   * @brief Send infix expression to server for calculate
   * @note Send the infix expression to server without validity checking
   * **************************************************************************/
  int SendInfixExpression(SOCKET &client_socket)
  {
    cout << "Please input the 'infix expression' send to server ("
         << cMaxBufferSize - 1 << " characters max, press enter to send, q to quit): ";

    /* get the input line */
    string str;
    getline(cin, str);
    while (true)
    {
      // check empty line input
      if (!str.size())
        cerr << "Error, empty line! Reinput the 'infix expression' send to server ("
             << cMaxBufferSize - 1 << " characters max, press enter to send, q to quit): ";
      // check if input size greater than cMaxBufferSize - 1
      else if (str.size() >= cMaxBufferSize)
        cerr << "Error, the input is greater than 127 characters! "
                "Reinput the 'infix expression' send to server ("
             << cMaxBufferSize - 1 << " characters max, press enter to send, q to quit): ";
      // check for quit
      else if (str.size() == 1 && str[0] == 'q')
        return -1;
      else
        break;
      getline(cin, str);
    }

    /* pack the input line and sent to server */
    PackageCalculator infix(str.c_str());

    if (send(client_socket, (char *)&infix, sizeof(infix), 0) == SOCKET_ERROR)
    {
      cerr << "Send data error!" << endl;
      return -1;
    }
    else
    {
      cout << "Send to server successful!" << endl;
      return 0;
    }
  }

  /*****************************************************************************
   * @name GetCalculationResult
   * @param client_socket client's windows socket handle
   * @retval -1: get result from server error
   * @retval 0 : get result from server success
   * @brief Get calculation result from server after send infix successful and show
   * **************************************************************************/
  int ShowCalculationResult(SOCKET &client_socket)
  {
    PackageHeader package_header;
    int receive_lenth = 0;
    int receive_status = 0;

    /* ↓ get package head first ↓ */
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

    PackageCalculator calculator(package_header);

    /* ↓ check package type received from server ↓ */
    if (package_header.command == CMD_Calculator)
    {
      // get package content except header
      receive_lenth = receive_status = 0;
      while (receive_lenth < int(sizeof(calculator) - sizeof(package_header)))
      {
        receive_status = recv(client_socket,
                              (char *)&calculator + sizeof(package_header) + receive_lenth,
                              sizeof(calculator) - sizeof(package_header) - receive_lenth, 0);
        if (receive_status == SOCKET_ERROR)
        {
          cerr << "Receive data error!" << endl;
          return -1;
        }
        else
          receive_lenth += receive_status;
      }

      /* ↓ check received package ↓ */
      switch (calculator.calculation_status)
      {
      case cSuccess:
        // server calculate successful, show the infix expression and result
        cout << "Get calculation result successful." << endl;
        if (calculator.is_result == true)
        {
          cout << calculator.infix_expression << '=' << calculator.result << endl;
          return 0;
        }
        else
          cerr << "Error! Unknown error." << endl;
        break;

      case cInfixEmpty:
        cerr << "Error! Infix expression is empty." << endl;
        break;

      case cInfixError:
        cerr << "Error! Invalid infix expression." << endl;
        break;

      case cDevidedBy0:
        cerr << "Error! Invalid infix expression. Devided by zero." << endl;
        break;

      case cUnknownError:
        cerr << "Error! Unknown error." << endl;
        break;

      case cUnCalculate:
        cerr << "Error! Server uncalculated the infix expression." << endl;
        break;

      default:
        break;
      }
      return -1;
    }
    else
    {
      cerr << "Data Package Error!" << endl;
      return -1;
    }
  }

}
