#include <iostream>
#include <string>
#include <vector>
#include <utility>
#include <cctype>
#include <stack>

#include "functions_server.h"
#include "data_package.hpp"

namespace sheer_rey
{
  using std::cerr, std::endl;
  using std::isdigit;
  using std::pair;
  using std::stack;
  using std::string, std::stod;
  using std::vector;

  /*****************************************************************************
   * @name IsInfixValid
   * @param infix infix expression string to be checked
   * @return true : input \p infix expression is valid
   * @return false: input \p infix expression is not valid
   * @brief check if the infix expression is valid
   * @note this function will remove spaces and replace brackets of input infix expression
   * **************************************************************************/
  bool IsInfixValid(string &infix)
  {
    /* ↓ remove spaces in infix ↓ */
    int slow = 0, fast = slow;
    while (fast < int(infix.size()))
    {
      if (infix[fast] == ' ')
        fast++;
      else
      {
        if (slow < fast)
          infix[slow++] = infix[fast++];
        else
        {
          slow++;
          fast++;
        }
      }
    }
    infix.resize(slow);

    /* ↓ replace '[]' and '{}' with '()' ↓ */
    for (char &c : infix)
      switch (c)
      {
      case '[':
      case '{':
        c = '(';
        break;

      case ']':
      case '}':
        c = ')';
        break;

      default:
        break;
      }

    /* ↓ check infix program validity ↓ */

    // the first character must be digit or left bracket
    // the last character must be digit or right bracket
    if ((!isdigit(*infix.begin()) && *infix.begin() != '(') ||
        (!isdigit(*infix.rbegin()) && *infix.rbegin() != ')'))
      return false;
    stack<char> symbol_stack;
    for (int i = 0; i < int(infix.size()); i++)
    {
      if (isdigit(infix.at(i)))
        continue;
      else
        switch (infix.at(i))
        {
        case '.':
          // there must be digit at both sides of decimal point
          if (!isdigit(infix.at(i - 1)) || !isdigit(infix.at(i + 1)))
            return false;
          break;

        case '(':
          symbol_stack.push(')'); // used to match brackets
          // the left side of outermost left bracket must be operator
          if (i > 0)
            if (infix.at(i - 1) != '(') // it means '(' at outermost
              if (infix.at(i - 1) != '+' && infix.at(i - 1) != '-' &&
                  infix.at(i - 1) != '*' && infix.at(i - 1) != '/')
                return false;
          // the right side of innermost left bracket must be digit
          if (infix.at(i + 1) != '(') // it means '(' at innermost
            if (!isdigit(infix.at(i + 1)))
              return false;
          // the medial left bracket between outermost and innermost
          // used to match quantity
          break;

        case ')':
          if (symbol_stack.empty())
            // it means the number of brackets don't match
            return false;
          else
            symbol_stack.pop(); // used to match brackets
          // the left side of innermost right bracket must be digit
          if (infix.at(i - 1) != ')') // it means ')' at innermost
            if (!isdigit(infix.at(i - 1)))
              return false;
          // the right side of outermost right bracket must be operator
          if (i < int(infix.size() - 1))
            if (infix.at(i + 1) != ')') // it means ')' at outermost
              if (infix.at(i + 1) != '+' && infix.at(i + 1) != '-' &&
                  infix.at(i + 1) != '*' && infix.at(i + 1) != '/')
                return false;
          break;

        case '+':
        case '-':
        case '*':
        case '/':
          // left hand side
          if (!isdigit(infix.at(i - 1)))
            if (infix.at(i - 1) != ')')
              return false;

          // right hand side
          if (!isdigit(infix.at(i + 1)))
            if (infix.at(i + 1) != '(')
              return false;
          break;

        default:
          // infix[i] is a digit
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
   * @param infix_expression valid infix expression string without spaces and only have parentheses
   * @return a vector<string> object contains every operator and operand of suffix expression
   * @brief translate infix expression to suffix expression for calculate
   * @note if there's any error during runtime, the function will return a vector
   *       with only one element string of "error"
   * **************************************************************************/
  vector<string> Infix2Suffix(const string &infix_expression)
  {
    stack<char> symbol_stack;
    vector<string> suffix_expression;
    size_t i, j;
    for (i = 0, j = 0; i < infix_expression.size(); i++)
    {
      if (isdigit(infix_expression[i]))
        continue;
      else
      {
        if (i - j)
        {
          suffix_expression.push_back(infix_expression.substr(j, i - j));
        }
        if (symbol_stack.empty())
          symbol_stack.push(infix_expression[i]);
        else
        {
          char stack_top = symbol_stack.top();
          switch (infix_expression[i])
          {
          case '*':
          case '/':
            while (stack_top == '*' || stack_top == '/')
            {
              suffix_expression.push_back(string(1, stack_top));
              symbol_stack.pop();
              if (!symbol_stack.empty())
                stack_top = symbol_stack.top();
              else
                break;
            }
            symbol_stack.push(infix_expression[i]);
            break;
          case '+':
          case '-':
            while (stack_top != '(')
            {
              suffix_expression.push_back(string(1, stack_top));
              symbol_stack.pop();
              if (!symbol_stack.empty())
                stack_top = symbol_stack.top();
              else
                break;
            }
            symbol_stack.push(infix_expression[i]);
            break;
          case '(':
            symbol_stack.push(infix_expression[i]);
            break;
          case ')':
            while (stack_top != '(')
            {
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
            suffix_expression.assign(1, "error");
            return suffix_expression;
          }
        }
        j = i + 1;
      }
    }
    if (i - j)
      suffix_expression.push_back(infix_expression.substr(j, i - j));
    while (!symbol_stack.empty())
    {
      suffix_expression.push_back(string(1, symbol_stack.top()));
      symbol_stack.pop();
    }
    return suffix_expression;
  }

  pair<CalculateStatus, double> Calculator(const string &infix_expression)
  {
    vector<string> suffix_expression = Infix2Suffix(infix_expression);
    stack<double> calculation_stack;
    for (const string &str : suffix_expression)
    {
      if (isdigit(str.at(0)) || (str.at(1) == '-' && str.size() > 1))
        calculation_stack.push(stod(str));
      else
      {
        double rhs = calculation_stack.top();
        calculation_stack.pop();
        double lhs = calculation_stack.top();
        calculation_stack.pop();
        switch (str.at(0))
        {
        case '+':
          calculation_stack.push(lhs + rhs);
          break;
        case '-':
          calculation_stack.push(lhs - rhs);
          break;
        case '*':
          calculation_stack.push(lhs * rhs);
          break;
        case '/':
          if (rhs == 0)
          {
            cerr << "Error! The divisor cannot be zero!" << endl;
            return pair<CalculateStatus, double>(cDevidedBy0, 0);
          }
          else
            calculation_stack.push(lhs / rhs);
          break;

        default:
          break;
        }
      }
    }
    if (calculation_stack.size() == 1)
      return pair<CalculateStatus, double>(cSuccess, calculation_stack.top());
    else
      return pair<CalculateStatus, double>(cUnknownError, 0);
  }
}
