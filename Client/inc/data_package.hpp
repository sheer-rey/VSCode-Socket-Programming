#ifndef _DATA_PACKAGE_HPP_
#define _DATA_PACKAGE_HPP_

#include <cstdint>
#include <cstring>

namespace sheer_rey {
  using std::int32_t;
  using std::strcpy, std::memset;

  const int cMaxBufferSize = 128;

  /*****************************************************************************
   * @brief contents of \p PackageHeader
   * **************************************************************************/
  enum CMD { CMD_Hello, CMD_Calculator };

  struct PackageHeader {
    /* ↓ data member ↓ */
    int32_t package_lenth;
    CMD command;

    /* ↓ function member ↓ */

    // constructor
    PackageHeader() = default;

    PackageHeader(int32_t _lenth, CMD _cmd)
        : package_lenth(_lenth), command(_cmd) {}

    PackageHeader(const PackageHeader&) = default;

    // destructor
    ~PackageHeader() = default;

    // operator
    PackageHeader& operator=(const PackageHeader&) = default;

    // deleted functions
    PackageHeader(PackageHeader&&) = delete;
    PackageHeader& operator=(PackageHeader&&) = delete;
  };

  /*****************************************************************************
   * @brief contents of \p PackageHello
   * **************************************************************************/
  struct PackageHello : public PackageHeader {
    /* ↓ data member ↓ */
    char message[cMaxBufferSize] = {};

    /* ↓ function member ↓ */

    // constructor
    PackageHello() : PackageHeader(sizeof(PackageHello), CMD_Hello) {}

    PackageHello(const char* _message)
        : PackageHeader(sizeof(PackageHello), CMD_Hello) {
      strcpy(message, _message);
    }

    PackageHello(const PackageHeader& _package_header)
        : PackageHeader(_package_header) {}

    // destructor
    ~PackageHello() = default;

    // operator
    PackageHello& operator=(const PackageHeader& _package_header) {
      if (&_package_header != this) {
        PackageHeader::operator=(_package_header);
        memset(message, 0, cMaxBufferSize);
      }
      return *this;
    }

    // deleted functions
    PackageHello(const PackageHello&) = delete;
    PackageHello(PackageHello&&) = delete;
    PackageHello& operator=(const PackageHello&) = delete;
    PackageHello& operator=(PackageHello&&) = delete;
  };

  /*****************************************************************************
   * @brief contents of \p PackageCalculator
   * **************************************************************************/
  enum CalculateStatus {
    cSuccess,
    cInfixEmpty,
    cInfixError,
    cDevidedBy0,
    cUnknownError,
    cUnCalculate,
  };

  struct PackageCalculator : public PackageHeader {
    /* ↓ data member ↓ */
    char infix_expression[cMaxBufferSize] = {};
    CalculateStatus calculation_status = cUnCalculate;
    bool is_result = false;
    double result = -1;

    /* ↓ function member ↓ */

    // constructor
    PackageCalculator()
        : PackageHeader(sizeof(PackageCalculator), CMD_Calculator) {}

    PackageCalculator(const char* _infix,
                      CalculateStatus _status = cUnCalculate,
                      bool _is_result = false,
                      double _result = -1)
        : PackageHeader(sizeof(PackageCalculator), CMD_Calculator),
          calculation_status(_status),
          is_result(_is_result),
          result(_result) {
      strcpy(infix_expression, _infix);
    }

    PackageCalculator(const PackageHeader& _package_header)
        : PackageHeader(_package_header) {}

    // destructor
    ~PackageCalculator() = default;

    // operator
    PackageCalculator& operator=(const PackageHeader& _package_header) {
      if (&_package_header != this) {
        PackageHeader::operator=(_package_header);
        memset(infix_expression, 0, cMaxBufferSize);
      }
      return *this;
    }

    // deleted functions
    PackageCalculator(const PackageCalculator&) = delete;
    PackageCalculator(PackageCalculator&&) = delete;
    PackageCalculator& operator=(const PackageCalculator&) = delete;
    PackageCalculator& operator=(PackageCalculator&&) = delete;
  };
}  // namespace sheer_rey

#endif
