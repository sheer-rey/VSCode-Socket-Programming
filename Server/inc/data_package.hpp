#ifndef _DATA_PACKAGE_HPP_
#define _DATA_PACKAGE_HPP_

#include <cstdint>
#include <cstring>

namespace sheer_rey
{
  using std::int32_t;
  using std::strcpy, std::memset;

  const int cMaxBufferSize = 128;

  enum CMD
  {
    CMD_Hello,
    CMD_Calculator
  };

  struct PackageHeader
  {
    int32_t package_lenth;
    CMD command;
    PackageHeader() = default;
    PackageHeader(int32_t _lenth, CMD _cmd)
        : package_lenth(_lenth), command(_cmd) {}
    PackageHeader(const PackageHeader &) = default;
    ~PackageHeader() = default;
    PackageHeader &operator=(const PackageHeader &) = default;

    PackageHeader(PackageHeader &&) = delete;
    PackageHeader &operator=(PackageHeader &&) = delete;
  };

  struct PackageHello : public PackageHeader
  {
    char message[cMaxBufferSize] = {};
    PackageHello() : PackageHeader(sizeof(PackageHello), CMD_Hello) {}
    PackageHello(const char *_message)
        : PackageHeader(sizeof(PackageHello), CMD_Hello)
    {
      strcpy(message, _message);
    }
    PackageHello(const PackageHeader &_package_header)
        : PackageHeader(_package_header) {}
    ~PackageHello() = default;
    PackageHello &operator=(const PackageHeader &_package_header)
    {
      if (&_package_header != this)
      {
        PackageHeader::operator=(_package_header);
        memset(message, 0, cMaxBufferSize);
      }
      return *this;
    }

    PackageHello(const PackageHello &) = delete;
    PackageHello(PackageHello &&) = delete;
    PackageHello &operator=(const PackageHello &) = delete;
    PackageHello &operator=(PackageHello &&) = delete;
  };

  struct PackageCalculator : public PackageHeader
  {
    char infix_expression[cMaxBufferSize] = {};
    bool is_result;
    double result;
    PackageCalculator() : PackageHeader(sizeof(PackageCalculator), CMD_Calculator) {}
    PackageCalculator(const char *_infix, bool _is, double _result = -1)
        : PackageHeader(sizeof(PackageCalculator), CMD_Calculator),
          is_result(_is), result(_result)
    {
      strcpy(infix_expression, _infix);
    }
    PackageCalculator(const PackageHeader &_package_header)
        : PackageHeader(_package_header) {}
    ~PackageCalculator() = default;
    PackageCalculator &operator=(const PackageHeader &_package_header)
    {
      if (&_package_header != this)
      {
        PackageHeader::operator=(_package_header);
        memset(infix_expression, 0, cMaxBufferSize);
      }
      return *this;
    }

    PackageCalculator(const PackageCalculator &) = delete;
    PackageCalculator(PackageCalculator &&) = delete;
    PackageCalculator &operator=(const PackageCalculator &) = delete;
    PackageCalculator &operator=(PackageCalculator &&) = delete;
  };
}

#endif