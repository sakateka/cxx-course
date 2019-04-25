#ifndef CONST_CC
#define CONST_CC

#include <string>

namespace NConst {
    static const std::string ZERO = "0";
    static const std::string DOT = ".";
    static const char ALPHABET[] = "0123456789ABCDEF";
    // double guaranteed precision is 15 digits
    static const size_t DOUBLE_PRECISION = 15;
    static const int RADIX_MIN = 2;
    static const int RADIX_MAX = 16;
} // namespace NConst
#endif // #ifndef CONST_CC
