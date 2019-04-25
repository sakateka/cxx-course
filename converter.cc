#ifndef CONVERTER_CC
#define CONVERTER_CC

#include <string>
#include <iostream>
#include "pnumber.cc"
#include <stdexcept>

namespace NConverter {
    using NPNumber::ALPHABET;
    using NPNumber::TPNumber;
    class Conver_10_P {
    public:
        //Преобразовать целое в символ.
        static char int_to_Char(int n) {
            if (0 > n || n > 15) {
                throw std::out_of_range("Value out of range: " + std::to_string(n));
            }
            return ALPHABET[n];
        }
        //Преобразовать десятичное целое в с.сч. с основанием р.
        static std::string int_to_P(int n, int p) {
            return Do(double(n), p, 0);
        }
        //Преобразовать десятичную дробь в с.сч. с основанием р.
        static std::string flt_to_P(double n, int p, int c) {
            return Do(n, p, c);
        }
        //Преобразовать десятичное действительное число в с.сч. с осн. р.
        static std::string Do(double n, int p = 10, int c = 3) {
            TPNumber pn(n, p, c);
            return pn.ToString();
        }
    }; // class Conver_10_P
    class Conver_P_10 {
    public:
        //Преобразовать цифру в число.
        static double char_To_num(char ch) {
            return double(TPNumber::CharToIdx(ch));
        }
        //Преобразовать строку в число
        static double convert(const std::string& pNum, int p, double weight = 0) {
            return TPNumber(pNum, p, weight).GetNumber();
        }
        //Преобразовать из с.сч. с основанием р в с.сч. с основанием 10.
        static double dval(const std::string& pNum, int p) {
            return convert(pNum, p);
        }
    }; // class Conver_P_10

} // namespace NConverter

#ifdef RUN_TESTS
#include "acutest.h"
namespace TestNConverter {
    using NConverter::Conver_10_P;
    using NConverter::Conver_P_10;
    using namespace std;
    struct Case {
        double num;
        int radix;
        int prec;
        string expect;
    };
    const auto cases = vector<Case>{
        {0.0, 2, 3, "0.000"},
        // Тестовый набор сгенерирован из случайных чисел.
        // За эталон числа в которое конвертируется исходный double num
        // взят результат с сервиса https://baseconvert.com
        // Исходный код сервиса https://github.com/Skalman/baseconvert
        {-163.22679204619095, 2, 2, "-10100011.00"},
        {416.1800109856468, 15, 3, "1CB.2A7"},
        {490.0739797855565, 7, 6, "1300.034242"},
        {-552.0771122797344, 13, 6, "-336.100553"},
        {252.181873553691, 3, 1, "100100.0"},
        {-340.45905953130796, 8, 5, "-524.35302"},
        {119.00757024950497, 4, 4, "1313.0001"},
        {-63.890613894975104, 8, 4, "-77.7077"},
        {156.35195858631167, 12, 0, "110"},
        {154.6689070247263, 8, 6, "232.526365"},
        {220.4001378622886, 7, 0, "433"},
        {-729.5593774727158, 2, 4, "-1011011001.1000"},
        {26.585956326744338, 5, 7, "101.2431102"},
        {572.3150164251238, 9, 1, "705.2"},
        {81.02981244324104, 15, 4, "56.06A9"},
        {439.07301416705525, 9, 3, "537.058"},
        {-48.972657941123316, 2, 6, "-110000.111110"},
        {-1.7036002125668972, 12, 3, "-1.853"},
        {620.5654185208238, 3, 5, "211222.12002"},
        {-525.141364218048, 14, 3, "-297.1D9"},
        {-657.6612950971388, 14, 1, "-34D.9"},
        {434.64015556189736, 10, 0, "435"},
        {-213.2380562215185, 13, 2, "-135.31"},
        {-280.43040425050515, 10, 6, "-280.430404"},
        {-339.2466884857781, 14, 7, "-1A3.364CADB"},
        {-375.32895756965036, 6, 0, "-1423"},
        {737.9808597436097, 11, 6, "610.A87584"},
        {-204.24645684326447, 8, 4, "-314.1761"},
        {-317.80496611796457, 9, 0, "-382"},
        {104.0674077099527, 12, 5, "88.09859"},
        {-440.4266174317555, 4, 3, "-12320.123"},
        {637.2726303970761, 3, 2, "212121.02"},
    };

    struct IntCase {
        int num;
        int prec;
        string expect;
    };
    const auto intCases = vector<IntCase>{
        {0, 2, "0"},
        {1, 2, "1"},
        {2, 2, "10"},
        {3, 2, "11"},
        {255, 2, "11111111"},
        {-10, 8, "-12"},
        {-10, 16, "-A"},
        {-15, 16, "-F"},
        {-16, 16, "-10"},
        {16, 16, "10"},
        {15, 16, "F"},
        {11415, 16, "2C97"},
    };
    void test_converter_10_p_operations() {
        Conver_10_P conv;
        TEST_CASE("int2char");
        {
            TEST_CHECK(conv.int_to_Char(0) == '0');
            TEST_CHECK(conv.int_to_Char(1) == '1');
            // ...
            for (int i = 0; i < 16; i++) {
                char expect = NPNumber::ALPHABET[i];
                char c = conv.int_to_Char(i);
                TEST_CHECK_(c == expect, "%c == %c", c, expect);
            }
            // ...
            TEST_CHECK(conv.int_to_Char(15) == 'F');
            TEST_EXCEPTION(conv.int_to_Char(16), out_of_range);
            TEST_EXCEPTION(conv.int_to_Char(-1), out_of_range);
        }
        TEST_CASE("int2str");
        {
            for (auto& c : intCases) {
                string result = conv.int_to_P(c.num, c.prec);
                TEST_CHECK_(result == c.expect, "%s == %s", result.c_str(), c.expect.c_str());
            }
        }

        TEST_CASE("flt2p");
        {
            for (auto& c : cases) {
                string result = conv.flt_to_P(c.num, c.radix, c.prec);
                TEST_CHECK_(result == c.expect, "%s == %s", result.c_str(), c.expect.c_str());
            }
        }
    }
    void test_converter_p_10_operations() {
        using NPNumber::TPNumber;
        Conver_P_10 conv;
        TEST_CASE("char_To_num");
        {
            TEST_CHECK(conv.char_To_num('A') == 10);
        }
        TEST_CASE("convert");
        {
            for (auto& c : cases) {
                double result = conv.convert(c.expect, c.radix);
                TPNumber pn(result, c.radix, c.prec);
                if (pn.GetNumber() >= 0) {
                    pn.SetDoCarry();
                } else {
                    pn.SetDoCarry(false);
                }
                string backToStr = pn.ToString();
                TEST_CHECK_(backToStr == c.expect, "%s == %s", backToStr.c_str(), c.expect.c_str());
                //TEST_CHECK_(result == c.num, "%lf == %lf", result, c.num);
            }
        }
    }
} // namespace TestNConverter

#endif // #ifdef RUN_TESTS
#endif // #ifndef CONVERTER_CC
