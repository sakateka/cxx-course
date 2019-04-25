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
        static double convert(const std::string& pNum, int base, double weight = 0) {
            return TPNumber(pNum, base, weight).GetNumber();
        }
        //Преобразовать из с.сч. с основанием р в с.сч. с основанием 10.
        static double dval(const std::string& pNum, int base) {
            return convert(pNum, base);
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
        {-1.7036002125668972, 12, 3, "-1.853"},
        {-10.51467116681556, 14, 3, "-A.72C"},
        {-163.22679204619095, 2, 2, "-10100011.00"},
        {-189.41276270058626, 2, 7, "-10111101.0110100"},
        {-204.24645684326447, 8, 4, "-314.1761"},
        {-213.2380562215185, 13, 2, "-135.31"},
        {-231.43178606809232, 6, 4, "-1023.2331"},
        {-234.22891053650284, 4, 0, "-3222"},
        {-253.4954538298093, 9, 2, "-311.44"},
        {-280.43040425050515, 10, 6, "-280.430404"},
        {-317.80496611796457, 9, 0, "-382"},
        {-324.28290361618315, 4, 3, "-11010.102"},
        {-339.2466884857781, 14, 7, "-1A3.364CADB"},
        {-340.45905953130796, 8, 5, "-524.35302"},
        {-362.0180854238283, 3, 6, "-111102.000111"},
        {-365.8262975435402, 11, 4, "-302.90A8"},
        {-375.32895756965036, 6, 0, "-1423"},
        {-402.7956514936853, 9, 0, "-486"},
        {-434.64015556189736, 10, 4, "-434.6401"},
        {-440.4266174317555, 4, 3, "-12320.123"},
        {-48.972657941123316, 2, 6, "-110000.111110"},
        {-491.5631286407264, 9, 4, "-605.5054"},
        {-525.141364218048, 14, 3, "-297.1D9"},
        {-552.0771122797344, 13, 6, "-336.100553"},
        {-617.9333420929238, 14, 7, "-321.D0D13AC"},
        {-626.9608046088618, 14, 3, "-32A.D64"},
        {-63.890613894975104, 8, 4, "-77.7077"},
        {-636.3308309583647, 9, 5, "-776.28715"},
        {-657.6612950971388, 14, 1, "-34D.9"},
        {-72.22009453975133, 6, 5, "-200.11531"},
        {-729.5593774727158, 2, 4, "-1011011001.1000"},
        {104.0674077099527, 12, 5, "88.09859"},
        {119.00757024950497, 4, 4, "1313.0001"},
        {12.01160668633031, 9, 4, "13.0084"},
        {149.70041054572857, 6, 4, "405.4111"},
        {154.6689070247263, 8, 6, "232.526365"},
        {155.73457810033642, 10, 0, "155"},
        {156.35195858631167, 12, 0, "110"},
        {157.5800800245345, 13, 0, "C1"},
        {164.0947971477068, 13, 3, "C8.130"},
        {17.002835622278, 7, 0, "23"},
        {220.4001378622886, 7, 0, "433"},
        {232.34448003116398, 7, 5, "451.22610"},
        {252.181873553691, 3, 1, "100100.0"},
        {26.585956326744338, 5, 7, "101.2431102"},
        {292.7517125091312, 3, 6, "101211.202021"},
        {310.4466230222396, 7, 4, "622.3061"},
        {36.88883248081177, 6, 2, "100.51"},
        {401.2318800375758, 15, 1, "1BB.3"},
        {416.1800109856468, 15, 3, "1CB.2A7"},
        {434.64015556189736, 10, 0, "434"},
        {434.64015556189736, 10, 4, "434.6401"},
        {439.07301416705525, 9, 3, "537.058"},
        {490.0739797855565, 7, 6, "1300.034242"},
        {542.3095434918035, 6, 1, "2302.1"},
        {548.7695612424577, 9, 1, "668.6"},
        {561.66091911797, 4, 3, "20301.222"},
        {572.3150164251238, 9, 1, "705.2"},
        {620.5654185208238, 3, 5, "211222.12002"},
        {627.2881340029084, 3, 7, "212020.0212100"},
        {637.2726303970761, 3, 2, "212121.02"},
        {64.79926926822037, 5, 1, "224.3"},
        {737.9808597436097, 11, 6, "610.A87584"},
        {768.124841520417, 7, 1, "2145.0"},
        {81.02981244324104, 15, 4, "56.06A9"},
        {856.8062082784829, 9, 3, "1151.722"},
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
                TEST_CHECK_(result == c.expect, "Case TPNumber(%lf, %d, %d): %s == %s",
                            c.num, c.radix, c.prec, result.c_str(), c.expect.c_str());
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
                pn.SetDoCarry();
                string backToStr = pn.ToString();
                TEST_CHECK_(backToStr == c.expect,
                            "Case TPNumber(%f, %d, %d) -> parsed from '%s' (expect) "
                            "-> TPNumber(%f, %d, %d).ToString() -> '%s' (result)",
                            c.num, c.radix, c.prec, c.expect.c_str(),
                            result, pn.GetRadix(), pn.GetPrecision(), backToStr.c_str());
            }
        }
    }
} // namespace TestNConverter

#endif // #ifdef RUN_TESTS
#endif // #ifndef CONVERTER_CC
