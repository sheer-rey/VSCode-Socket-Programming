#include "functions_server.h"

#include <cctype>
#include <iostream>
#include <stack>
#include <string>
#include <utility>
#include <vector>

#include "data_package.hpp"

namespace sheer_rey {
  using std::cerr, std::endl;
  using std::isdigit;
  using std::pair;
  using std::stack;
  using std::string, std::stod;
  using std::vector;

  /*****************************************************************************
   * @name IsInfixValid
   * @param _infix infix expression string to be checked
   * @retval true : input \p _infix expression is valid
   * @retval false: input \p _infix expression is not valid
   * @brief Check if the infix expression is valid
   * @note This function will remove spaces and replace brackets of input infix
   * expression
   * @note This function only used in current source file.
   * @warning This function may modify the real parameter \p _infix
   * **************************************************************************/
  bool IsInfixValid(string& _infix) {
    /* ↓ remove spaces in _infix ↓ */
    int slow = 0, fast = slow;
    while (fast < int(_infix.size())) {
      if (_infix[fast] == ' ')
        fast++;
      else {
        if (slow < fast)
          _infix[slow++] = _infix[fast++];
        else {
          slow++;
          fast++;
        }
      }
    }
    _infix.resize(slow);

    /* ↓ replace '[]' and '{}' with '()' ↓ */
    for (char& c : _infix)
      switch (c) {
        case '[':
        case '{': c = '('; break;

        case ']':
        case '}': c = ')'; break;

        default: break;
      }

    /* ↓ check infix program validity ↓ */

    // the first character must be digit or left bracket
    // the last character must be digit or right bracket
    if ((!isdigit(*_infix.begin()) && *_infix.begin() != '(') ||
        (!isdigit(*_infix.rbegin()) && *_infix.rbegin() != ')'))
      return false;
    stack<char> symbol_stack;
    for (int i = 0; i < int(_infix.size()); i++) {
      if (isdigit(_infix.at(i)))
        continue;
      else
        switch (_infix.at(i)) {
          case '.':
            // there must be digit at both sides of decimal point
            if (!isdigit(_infix.at(i - 1)) || !isdigit(_infix.at(i + 1)))
              return false;
            break;

          case '(':
            symbol_stack.push(')');  // used to match brackets
            // the left side of outermost left bracket must be operator
            if (i > 0)
              if (_infix.at(i - 1) != '(')  // it means '(' at outermost
                if (_infix.at(i - 1) != '+' && _infix.at(i - 1) != '-' &&
                    _infix.at(i - 1) != '*' && _infix.at(i - 1) != '/')
                  return false;
            // the right side of innermost left bracket must be digit
            if (_infix.at(i + 1) != '(')  // it means '(' at innermost
              if (!isdigit(_infix.at(i + 1)))
                return false;
            // the medial left bracket between outermost and innermost
            // used to match quantity
            break;

          case ')':
            if (symbol_stack.empty())
              // it means the number of brackets don't match
              return false;
            else
              symbol_stack.pop();  // used to match brackets
            // the left side of innermost right bracket must be digit
            if (_infix.at(i - 1) != ')')  // it means ')' at innermost
              if (!isdigit(_infix.at(i - 1)))
                return false;
            // the right side of outermost right bracket must be operator
            if (i < int(_infix.size() - 1))
              if (_infix.at(i + 1) != ')')  // it means ')' at outermost
                if (_infix.at(i + 1) != '+' && _infix.at(i + 1) != '-' &&
                    _infix.at(i + 1) != '*' && _infix.at(i + 1) != '/')
                  return false;
            break;

          case '+':
          case '-':
          case '*':
          case '/':
            // left hand side
            if (!isdigit(_infix.at(i - 1)))
              if (_infix.at(i - 1) != ')')
                return false;

            // right hand side
            if (!isdigit(_infix.at(i + 1)))
              if (_infix.at(i + 1) != '(')
                return false;
            break;

          default:
            // _infix[i] is neither a digit nor any symbols above, that means
            // _infix is invalid
            return false;
            break;
        }
    }

    // check if the number of left and right brackets match
    if (symbol_stack.empty())
      return true;
    else
      return false;
  }

  /*****************************************************************************
   * @name Infix2Suffix
   * @param _infix_expression valid infix expression string without spaces and
   * only have parentheses
   * @return a vector<string> object contains every operator and operand of
   * suffix expression
   * @brief Translate infix expression to suffix expression for calculate
   * @note If there's any error during runtime, the function will return a
   * vector with only one element string of "error"
   * @note This function only used in current source file.
   * **************************************************************************/
  vector<string> Infix2Suffix(const string& _infix_expression) {
    stack<char> symbol_stack;
    vector<string> suffix_expression;
    size_t i, j;
    for (i = 0, j = 0; i < _infix_expression.size(); i++) {
      if (isdigit(_infix_expression[i]))
        continue;
      else {
        // i-j > 0, that means there's digits before _infix_expression[i]
        if (i - j)
          // output to suffix expression vector
          suffix_expression.push_back(_infix_expression.substr(j, i - j));

        // if symbol stack is empty, that means there's no need to compare
        // operator's priority
        if (symbol_stack.empty())
          symbol_stack.push(_infix_expression[i]);
        else {
          // symbol stack is not empty, need to compare operator's priority
          char stack_top = symbol_stack.top();
          switch (_infix_expression[i]) {
            case '*':
            case '/':
              // if current operator is '*' or '/', pop operators whose
              // priority is greater than or equal to current to suffix
              // expression's vector (also '*' and '/'), stop until '(',
              // then push current operator to symbol stack.
              while (stack_top == '*' || stack_top == '/') {
                suffix_expression.push_back(string(1, stack_top));
                symbol_stack.pop();
                if (!symbol_stack.empty())
                  stack_top = symbol_stack.top();
                else
                  break;
              }
              symbol_stack.push(_infix_expression[i]);
              break;

            case '+':
            case '-':
              // if current operator is '+' or '-', pop operators whose
              // priority is greater than or equal to current to suffix
              // expression's vector
              // ('+', '-', '*' and '/'), stop until '(', then push current
              // operator to symbol stack.
              while (stack_top != '(') {
                suffix_expression.push_back(string(1, stack_top));
                symbol_stack.pop();
                if (!symbol_stack.empty())
                  stack_top = symbol_stack.top();
                else
                  break;
              }
              symbol_stack.push(_infix_expression[i]);
              break;

            case '(':
              // if current symbol is '(', push to symbol stack directly
              // and wait for matched ')'
              symbol_stack.push(_infix_expression[i]);
              break;

            case ')':
              // if current symbol is ')', pop all operators in symbol
              // stack to suffix expression vector untill meet '(', then
              // disgard both '(' and ')'.
              while (stack_top != '(') {
                suffix_expression.push_back(string(1, stack_top));
                symbol_stack.pop();
                if (!symbol_stack.empty())
                  stack_top = symbol_stack.top();
                else
                  break;
              }
              symbol_stack.pop();
              break;

            default:
              // if there's any other symbols, that means infix expression
              // is invalid
              suffix_expression.assign(1, "error");
              return suffix_expression;
          }
        }
        // j used to indicate the symbol after current
        j = i + 1;
      }
    }

    // push the last segment of digits to suffix expression vector
    if (i - j)
      suffix_expression.push_back(_infix_expression.substr(j, i - j));

    // pop all remaining symbol in symbol stack to suffix expression vector
    while (!symbol_stack.empty()) {
      suffix_expression.push_back(string(1, symbol_stack.top()));
      symbol_stack.pop();
    }

    return suffix_expression;
  }

  /*****************************************************************************
   * @name Calculator
   * @param _infix_expression infix expression string
   * @return a pair object contains calculation status and calculation result
   * @brief Calculate a infix expression and return calculation status and
   * result
   * @note Internally, this function call \p IsInfixValid to check infix
   * expression's validity and call \p Infix2Suffix to convert infix to suffix,
   * then use the suffix expression to calculate result.
   * @warning This function may modify the real parameter \p _infix_expression
   * **************************************************************************/
  pair<CalculateStatus, double> Calculator(string& _infix_expression) {
    /* ↓ check if infix expression is empty ↓ */
    if (_infix_expression.empty())
      return pair<CalculateStatus, double>(cInfixEmpty, -1);

    /* ↓ check validity of infix expression ↓ */
    if (IsInfixValid(_infix_expression) == false)
      return pair<CalculateStatus, double>(cInfixError, -1);

    /* ↓ convert infix to suffix expression ↓ */
    vector<string> suffix_expression = Infix2Suffix(_infix_expression);
    if (suffix_expression.size() && suffix_expression.at(0) == "error")
      return pair<CalculateStatus, double>(cInfixError, -1);

    /* ↓ calculate suffix expression ↓ */
    stack<double> calculation_stack;
    for (const string& str : suffix_expression) {
      /* ↓ check if str is an operand ↓ */
      if (isdigit(str.at(0)) || (str.size() > 1 && str.at(1) == '-'))
        // is an operand, push to calculation stack directly
        calculation_stack.push(stod(str));
      else {
        // is an operator, get and pop first two operand of calculation stack
        // top
        double rhs = calculation_stack.top();
        calculation_stack.pop();
        double lhs = calculation_stack.top();
        calculation_stack.pop();

        // check operator type
        switch (str.at(0)) {
          case '+': calculation_stack.push(lhs + rhs); break;

          case '-': calculation_stack.push(lhs - rhs); break;

          case '*': calculation_stack.push(lhs * rhs); break;

          case '/':
            // check is there a divided by zero error occur
            if (rhs == 0) {
              cerr << "Error! The divisor cannot be zero!" << endl;
              return pair<CalculateStatus, double>(cDevidedBy0, -1);
            } else
              calculation_stack.push(lhs / rhs);
            break;

          default: break;
        }
      }
    }

    /* ↓ check the correctness of calculation result ↓ */
    if (calculation_stack.size() == 1)
      return pair<CalculateStatus, double>(cSuccess, calculation_stack.top());
    else
      return pair<CalculateStatus, double>(cUnknownError, -1);
  }

  //==========================================================================//

  /*****************************************************************************
   * @name GetPackageHeader
   * @param _handled_socket server's socket
   * @param _package_header data package header to be fill
   * @retval  0 : get package header successful
   * @retval -1 : receive data error
   * @retval -2 : the client has performed an orderly shutdown
   * @brief Receive data from client and fill package header struct
   * **************************************************************************/
  int GetPackageHeader(const SOCKET& _handled_socket,
                       PackageHeader& _package_header) {
    int receive_status = 0;
    int receive_lenth = 0;

    // get package header first
    while (receive_lenth < int(sizeof(_package_header))) {
      receive_status =
          recv(_handled_socket, (char*)&_package_header + receive_lenth,
               int(sizeof(_package_header)) - receive_lenth, 0);
      if (receive_status < 0) {
        cerr << "Receive data error!" << endl;
        return -1;
      } else if (receive_status == 0 && receive_lenth == 0) {
        return -2;
      } else
        receive_lenth += receive_status;
    }

    return 0;
  }

  /*****************************************************************************
   * @name EchoMessageServer
   * @param _handled_socket server's socket
   * @param _package_header data package header to get the package head
   * infomation
   * @retval  0 : receive data and echo successful
   * @retval -1 : receive data error
   * @brief Receive data from client and echo
   * **************************************************************************/
  int EchoMessageServer(const SOCKET& _handled_socket,
                        PackageHeader& _package_header) {
    PackageHello message(_package_header);
    int receive_status = 0;
    int receive_lenth = 0;

    // get package content except header
    while (receive_lenth < int(sizeof(message) - sizeof(_package_header))) {
      receive_status = recv(
          _handled_socket,
          (char*)&message + int(sizeof(_package_header)) + receive_lenth,
          int(sizeof(message) - sizeof(_package_header)) - receive_lenth, 0);
      if (receive_status == SOCKET_ERROR) {
        cerr << "Receive data error!" << endl;
        return -1;
      } else
        receive_lenth += receive_status;
    }

    // echo received message
    send(_handled_socket, (char*)&message, sizeof(message), 0);
    return 0;
  }

  /*****************************************************************************
   * @name CalculatorServer
   * @param _handled_socket server's socket
   * @param _package_header data package header to get the package head
   * infomation
   * @retval  0 : receive data and sent result successful
   * @retval -1 : receive data error
   * @brief Receive data from client and sent back the calculation result
   * **************************************************************************/
  int CalculatorServer(const SOCKET& _handled_socket,
                       PackageHeader& _package_header) {
    PackageCalculator calculator(_package_header);
    int receive_status = 0;
    int receive_lenth = 0;

    // get package content except header
    while (receive_lenth < int(sizeof(calculator) - sizeof(_package_header))) {
      receive_status = recv(
          _handled_socket,
          (char*)&calculator + int(sizeof(_package_header)) + receive_lenth,
          int(sizeof(calculator) - sizeof(_package_header)) - receive_lenth, 0);
      if (receive_status == SOCKET_ERROR) {
        cerr << "Receive data error!" << endl;
        return -1;
      } else
        receive_lenth += receive_status;
    }

    /* ↓ check received package ↓ */
    if (calculator.calculation_status == cUnCalculate &&
        calculator.is_result == false) {
      string infix(calculator.infix_expression);
      pair<CalculateStatus, double> calculate_result = Calculator(infix);

      // pack calculation result
      memset(calculator.infix_expression, 0, cMaxBufferSize);
      strcpy(calculator.infix_expression, infix.c_str());
      calculator.is_result = true;
      calculator.calculation_status = calculate_result.first;
      calculator.result = calculate_result.second;

      // send calculation result
      send(_handled_socket, (char*)&calculator, sizeof(calculator), 0);
      return 0;
    } else {
      cerr << "Unidentified command." << endl;
      return -1;
    }
  }
}  // namespace sheer_rey
