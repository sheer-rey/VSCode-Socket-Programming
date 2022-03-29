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

  pair<bool, double> Calculator(const vector<string> &suffix_expression)
  {
    stack<double> calculation_stack;
    for (const string &str : suffix_expression)
    {
      if (isdigit(str.at(0)))
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
            return pair<bool, double>(false, 0);
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
      return pair<bool, double>(true, calculation_stack.top());
    else
      return pair<bool, double>(false, 0);
  }
}
