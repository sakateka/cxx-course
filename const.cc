#ifndef CONST_CC
#define CONST_CC

#include <string>

namespace NConst {
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-const-variable"
    static const std::string ZERO = "0";
    static const std::string DOT = ".";
    static const char ALPHABET[] = "0123456789ABCDEF";
    // double guaranteed precision is 15 digits
    static const size_t DOUBLE_PRECISION = 15;
    static const int RADIX_MIN = 2;
    static const int RADIX_MAX = 16;
#pragma clang diagnostic pop

    static int CharToIdx(char c) {
        int charIdx = c - '0';
        if (charIdx > 9) {
            // ord('0') = 48
            // ord('0') - ord('0') = 0, ord('1') - ord('0') = 1 ...
            // ord('A') = 65 ...
            // ord('A') - ord('0') - 7 = 10, ord('B') - ord('0') -7 = 11 ...
            charIdx -= 7;
        }
        return charIdx;
    }

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-function"
    static bool IsValidChar(char i, int base) {
        int idx = CharToIdx(i);
        return idx < base && ALPHABET[idx] == i;
    };
#pragma clang diagnostic pop

} // namespace NConst
#endif // #ifndef CONST_CC
