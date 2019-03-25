#ifndef PNUMBER_CC
#define PNUMBER_CC
#define TEST_NO_MAIN

#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

namespace NPNumber {

    class invalid_pnumber : public std::invalid_argument {
    public:
        explicit invalid_pnumber(const std::string& message)
            : std::invalid_argument(message) {
        }
    };
    class invalid_radix : public std::invalid_argument {
    public:
        explicit invalid_radix(const std::string& message)
            : std::invalid_argument(message) {
        }
    };
    class invalid_precision : public std::invalid_argument {
    public:
        explicit invalid_precision(const std::string& message)
            : std::invalid_argument(message) {
        }
    };

    class TPNumber {
    public:
        TPNumber(double n, int b, int c) {
            number = n;
            radix = validate_radix(b);
            precision = validate_precision(c);
        }
        static TPNumber default_() {
            return TPNumber(0, 10, 0);
        };

        TPNumber(const std::string& n, const std::string& b, const std::string& c) {
            SetRadix(parse_radix(b));
            precision = parse_precision(c);
            number = parse_number(n, radix);
        }
        friend std::ostream& operator<<(std::ostream& out, const TPNumber& p) {
            out << p.ToString();
            return out;
        }
        TPNumber operator+(const TPNumber& rhs) const {
            return {number + rhs.number, radix, precision};
        }
        void operator+=(const TPNumber& rhs) {
            number += rhs.number;
        }
        TPNumber operator-(const TPNumber& rhs) const {
            return {number - rhs.number, radix, precision};
        }
        void operator-=(const TPNumber& rhs) {
            number -= rhs.number;
        }
        TPNumber operator*(const TPNumber& rhs) const {
            return {number * rhs.number, radix, precision};
        }
        void operator*=(const TPNumber& rhs) {
            number *= rhs.number;
        }
        TPNumber operator/(const TPNumber& rhs) const {
            return {number / rhs.number, radix, precision};
        }
        void operator/=(const TPNumber& rhs) {
            number /= rhs.number;
        }
        bool operator==(const TPNumber& rhs) const {
            return number == rhs.number;
        }
        bool operator!=(const TPNumber& rhs) const {
            return number != rhs.number;
        }
        TPNumber operator!() const {
            if (number == 0) {
                throw std::runtime_error("Division by zero");
            }
            return {1 / number, radix, precision};
        }

        TPNumber Sqr() const {
            return {number * number, radix, precision};
        }

        double GetNumber() const {
            return number;
        }
        int GetRadix() const {
            return radix;
        }
        std::string GetRadixAsStr() const {
            return std::to_string(radix);
        }
        int GetPrecision() const {
            return precision;
        }
        std::string GetPrecisionAsStr() const {
            return std::to_string(precision);
        }

        std::string ToString() const {
            std::stringstream sresult;
            sresult << std::fixed << std::setprecision(precision) << number;
            std::string result = sresult.str();
            if (radix == 10 || std::string(result).find("inf") != std::string::npos) {
                return result;
            }
            result.clear();

            double positive_number = number < 0 ? -number : number;

            static const char alphabet[] = "0123456789ABCDEF";
            int tmpN = (int)positive_number;
            double fdouble = (positive_number - (double)tmpN);
            char fstring[17]; // 0.<double guaranteed precision 15 digits>
            sprintf(fstring, "%.15f", fdouble);
            std::string fs(fstring);
            std::vector<int> fracVec;
            transform(fs.begin() + 2 /*skip 0.*/, fs.end(),
                      std::back_inserter(fracVec),
                      [](char c) -> int { return (int)(c - '0'); });

            fs.clear();
            fs.resize(15, '0');
            for (int i = 0;
                 std::any_of(begin(fracVec), end(fracVec),
                             [](int c) { return c != 0; }) &&
                 i < 15;
                 i++) {
                int carry = 0;

                for (int j = fracVec.size() - 1; j >= 0; j--) {
                    int digit = fracVec[j] * radix + carry;
                    carry = digit / 10;
                    fracVec[j] = digit % 10;
                }
                fs[i] = alphabet[carry];
            }
            double carry = 0;
            for (int i = 14; i >= precision; i--) {
                carry = (double)(fs[i] - '0' + carry) >= (double)radix / 2.0 ? 1 : 0;
            }
            if (carry) {
                for (int i = precision - 1; i >= 0; i--) {
                    if (fs[i] == alphabet[radix - 1]) {
                        fs[i] = '0';
                    } else {
                        fs[i] = alphabet[fs[i] - '0' + 1];
                        carry = 0;
                        break;
                    }
                }
            }
            tmpN += carry;
            do {
                result += alphabet[tmpN % radix];
                tmpN /= radix;
            } while (tmpN);
            std::reverse(result.begin(), result.end());

            if (number < 0) {
                result = "-" + result;
            }
            if (precision > 0) {
                fs.resize(precision, '0');
                result += "." + fs;
            }
            return result;
        }
        std::string Repr() const {
            std::stringstream sresult;
            sresult << "TPNumber("
                    << std::fixed << std::setprecision(precision) << number
                    << ", " << radix << ", " << precision << ")";
            return sresult.str();
        }

        void SetRadix(int r) {
            radix = validate_radix(r);
        }
        void SetRadixAsStr(const std::string& rs) {
            SetRadix(parse_radix(rs));
        }
        void SetPrecision(int p) {
            precision = validate_precision(p);
        }
        void SetPrecisionAsStr(const std::string& ps) {
            precision = parse_precision(ps);
        }

    private:
        static int validate_radix(int r) {
            if (r < 2 or r > 16) {
                throw invalid_radix(std::to_string(r));
            }
            return r;
        }
        static int validate_precision(int p) {
            if (p < 0) {
                throw invalid_precision(std::to_string(p));
            }
            return p;
        }
        static int parse_radix(const std::string& rs) {
            if (rs.size() > 0 && rs[0] != '-') {
                char* nend;
                int r = strtol(rs.c_str(), &nend, 10);
                if (*nend == '\0') {
                    return validate_radix(r);
                }
            }
            // failed to parse
            throw invalid_radix(rs);
        };
        static int parse_precision(const std::string& ps) {
            if (ps.size() > 0 && ps[0] != '-') {
                char* nend;
                int p = strtol(ps.c_str(), &nend, 10);
                if (*nend == '\0') {
                    return validate_precision(p);
                }
            }
            // failed to parse
            throw invalid_precision(ps);
        }
        static double parse_number(const std::string& ns, int base) {
            char* nend;
            double n = strtol(ns.c_str(), &nend, base);
            if (*nend == '.' && *(++nend) != '\0') {
                char* dot = nend;
                double fractional = strtol(nend, &nend, base);
                if (*nend == '\0') {
                    n += fractional / (double)pow(base, nend - dot);
                }
            }
            if (*nend != '\0') {
                throw invalid_pnumber(ns);
            }
            return n;
        }

        double number;
        int radix;
        int precision;
    };
}; // namespace NPNumber

#ifdef RUN_TESTS
#include "acutest.h"
using namespace std;

//
// Constructors
//
void test_pnumber_constructor() {
    using namespace NPNumber;
    {
        TEST_CASE("ConstructorNumber");
        TPNumber p = TPNumber(11.3, 16, 1);
        TEST_CHECK(p.GetNumber() == 11.3);
        TEST_CHECK(p.GetRadix() == 16);
        TEST_CHECK(p.GetPrecision() == 1);
        TEST_CASE("Negative ConstructorNumber");
        p = TPNumber(-11.3, 16, 1);
        TEST_CHECK(p.GetNumber() == -11.3);
    }

    {
        TEST_CASE("ConstructorString");
        TPNumber p = TPNumber("11", "2", "3");
        if (not TEST_CHECK(p.GetNumber() == 3.0)) {
            TEST_MSG("GetNumber() == %.1lf", p.GetNumber());
        }
        TEST_CHECK(p.GetRadixAsStr() == "2");
        TEST_CHECK(p.GetPrecisionAsStr() == "3");
    }

    {
        TEST_CASE("ConstructorString with fractional part");
        TPNumber p = TPNumber("11.11", "2", "3");
        if (not TEST_CHECK(p.GetNumber() == 3.75)) {
            TEST_MSG("GetNumber() == %lf", p.GetNumber());
        }
    }
    {
        TEST_CASE("ConstructorString number ends with dot");
        TPNumber p = TPNumber("100.", "2", "3");
        if (not TEST_CHECK(p.GetNumber() == 4.0)) {
            TEST_MSG("GetNumber() == %.1lf", p.GetNumber());
        }
    }
    {
        TEST_CASE("ConstructorString negative number");
        TPNumber p = TPNumber("-f", "16", "3");
        if (not TEST_CHECK(p.GetNumber() == -15.0)) {
            TEST_MSG("GetNumber() == %.1lf", p.GetNumber());
        }
    }
}

void test_pnumber_constructor_exception() {
    using namespace NPNumber;
    TEST_CASE("ConstructorString mailformed string");
    TEST_EXCEPTION(TPNumber(0, 1, 3), invalid_radix);
    TEST_EXCEPTION(TPNumber(".0", "1", "3"), invalid_radix);
    TEST_EXCEPTION(TPNumber("1y1", "3", "3"), invalid_pnumber);
    TEST_EXCEPTION(TPNumber("13", "3y", "3"), invalid_radix);
    TEST_EXCEPTION(TPNumber("13", "3", "3y"), invalid_precision);
    TEST_CASE("ConstructorString missmatched radix");
    TEST_EXCEPTION(TPNumber("1F", "2", "3"), invalid_pnumber);
    TEST_CASE("ConstructorString negative radix and precision");
    TEST_EXCEPTION(TPNumber(1, -2, 3), invalid_radix);
    TEST_EXCEPTION(TPNumber(1, 2, -3), invalid_precision);
    TEST_EXCEPTION(TPNumber("1", "-2", "3"), invalid_radix);
    TEST_EXCEPTION(TPNumber("1", "2", "-3"), invalid_precision);
}

//
// Methods
//
void test_pnumber_setters() {
    using namespace NPNumber;
    TEST_CASE("Radix");
    {
        TPNumber p = TPNumber("100.", "2", "3");
        p.SetRadix(16);
        TEST_CHECK(p.GetRadix() == 16);
        TEST_CHECK(p.GetRadixAsStr() == "16");

        TEST_EXCEPTION(p.SetRadix(1), invalid_radix);
        TEST_EXCEPTION(p.SetRadix(17), invalid_radix);
        TEST_EXCEPTION(p.SetRadix(-1), invalid_radix);
        TEST_EXCEPTION(p.SetRadixAsStr("1"), invalid_radix);
        TEST_EXCEPTION(p.SetRadixAsStr("17"), invalid_radix);
        TEST_EXCEPTION(p.SetRadixAsStr("-1"), invalid_radix);

        p.SetRadixAsStr("8");
        TEST_CHECK(p.GetRadix() == 8);
    }

    TEST_CASE("Precision");
    {
        TPNumber p = TPNumber("100.", "2", "3");
        p.SetPrecision(0);
        TEST_CHECK(p.GetPrecision() == 0);
        TEST_CHECK(p.GetPrecisionAsStr() == "0");
        p.SetPrecisionAsStr("42");
        TEST_CHECK(p.GetPrecision() == 42);
        TEST_CHECK(p.GetPrecisionAsStr() == "42");
        TEST_EXCEPTION(p.SetPrecision(-1), invalid_precision);
        TEST_EXCEPTION(p.SetPrecisionAsStr("-2"), invalid_precision);
    }
}

void test_pnumber_to_string() {
    using namespace NPNumber;
    TEST_CASE("TPNumber::default_()");
    {
        TPNumber p = TPNumber::default_();
        TEST_CHECK(p.ToString() == "0");
        TEST_CHECK(p.GetRadix() == 10);
    }
    TEST_CASE("Zero");
    {
        for (int i = 2; i <= 16; i++) {
            TEST_CASE_("TPNumber(0, %i, 0)", i);
            TPNumber tmp = TPNumber(0, i, 0);
            TEST_CHECK_(tmp.ToString() == "0", "'%s' == '0'", tmp.ToString().c_str());
            TEST_CASE_("stringstream TPNumber(0, %i, 0)", i);
            stringstream ss;
            ss << tmp;
            TEST_CHECK_(ss.str() == "0", "'%s' == '0'", ss.str().c_str());
        }
        for (int i = 2; i <= 16; i++) {
            TEST_CASE_("TPNumber(0, %i, 1)", i);
            TPNumber tmp = TPNumber(0, i, 1);
            TEST_CHECK_(tmp.ToString() == "0.0", "'%s' == '0.0'",
                        tmp.ToString().c_str());
            TEST_CASE_("stringstream TPNumber(0, %i, 1)", i);
            stringstream ss;
            ss << tmp;
            TEST_CHECK_(ss.str() == "0.0", "'%s' == '0.0'", ss.str().c_str());
        }
    }

    TEST_CASE("Positive");
    {
        TPNumber p = TPNumber("FF.", "16", "3");
        p.SetRadix(15);
        string s = p.ToString();
        TEST_CHECK_(s == "120.000", "ToString == '%s'", s.c_str());
        stringstream ss("");
        ss << p;
        TEST_CHECK(ss.str() == "120.000");

        p.SetRadix(8);
        s = p.ToString();
        TEST_CHECK_(s == "377.000", "ToString == '%s'", s.c_str());
        p.SetRadix(10);
        s = p.ToString();
        TEST_CHECK_(s == "255.000", "ToString == '%s'", s.c_str());
        p.SetRadix(2);
        s = p.ToString();
        TEST_CHECK_(s == "11111111.000", "ToString == '%s'", s.c_str());
    }
    TEST_CASE("With fraction");
    {
        struct Case {
            double n;
            int r;
            int b;
            string expect;
        };
        vector<Case> cases = {
            {15.9375, 16, 4, "F.F000"},
            {15.9375, 15, 4, "10.E0E1"},
            {15.9375, 14, 4, "11.D1A7"},
            {15.9375, 13, 4, "12.C259"},
            {15.9375, 12, 4, "13.B300"},
            {15.9375, 11, 4, "14.A349"},
            {15.9375, 10, 4, "15.9375"},
            {15.9375, 9, 4, "16.8384"},
            {15.9375, 8, 4, "17.7400"},
            {15.9375, 7, 4, "21.6364"},
            {15.9375, 6, 4, "23.5343"},
            {15.9375, 5, 4, "30.4321"},
            {15.9375, 4, 4, "33.3300"},
            {15.9375, 3, 4, "120.2211"},
            {15.9375, 2, 4, "1111.1111"},

            {42.55, 14, 8, "30.79B2B2B3"},
            {42.55, 8, 8, "52.43146315"},
            {42.47, 7, 8, "60.32013201"},
            {11.3, 16, 1, "B.5"},
            {63.984375, 8, 0, "100"},
            {63.0, 8, 0, "77"},
        };

        TEST_CASE("Positive")
        for (auto c : cases) {
            TPNumber p = TPNumber(c.n, c.r, c.b);
            if (not TEST_CHECK(p.ToString() == c.expect)) {
                TEST_MSG("%s -> %s != %s (expect)",
                         p.Repr().c_str(),
                         p.ToString().c_str(),
                         c.expect.c_str());
            }
        }
        TEST_CASE("Negative")
        for (auto c : cases) {
            double n = -c.n;
            string expect = "-" + c.expect;
            TPNumber p = TPNumber(n, c.r, c.b);
            if (not TEST_CHECK(p.ToString() == expect)) {
                TEST_MSG("%s-> %s != %s (expect)",
                         p.Repr().c_str(),
                         p.ToString().c_str(),
                         expect.c_str());
            }
        }
    }
    TEST_CASE("Infinity");
    {
        TPNumber n = TPNumber(numeric_limits<double>::infinity(), 2, 2);
        TEST_CHECK(n.ToString() == "inf"); // inf^2
        n = TPNumber(-numeric_limits<double>::infinity(), 2, 2);
        TEST_CHECK(n.ToString() == "-inf"); // inf^2
    }
}

void test_pnumber_operations() {
    using namespace NPNumber;
    TEST_CASE("operator== && operator!=");
    {
        TPNumber a = TPNumber("F", "16", "3");
        TPNumber b = TPNumber("15", "10", "3");
        TEST_CHECK(a == b and a.GetNumber() == b.GetNumber());

        b.SetRadix(16);
        TEST_CHECK(a == b and a.GetNumber() == b.GetNumber());
        a = TPNumber(1, 10, 3);
        TEST_CHECK(a != b and a.GetNumber() != b.GetNumber());
    }

    {
        TEST_CASE("operator+=");
        TPNumber a = TPNumber("F", "16", "3");
        a += TPNumber("1", "10", "3");
        if (not TEST_CHECK(a == TPNumber("10", "16", "3"))) {
            TEST_MSG("a = %s", a.ToString().c_str());
        }
    }

    TEST_CASE("operator+");
    TEST_CHECK(TPNumber("F", "16", "3") + TPNumber("1", "10", "3") ==
               TPNumber("10", "16", "3"));
    // + negative
    TEST_CHECK(TPNumber("F", "16", "3") + TPNumber("-1", "10", "3") ==
               TPNumber("E", "16", "3"));

    TEST_CASE("operator-");
    TEST_CHECK(TPNumber("F", "16", "3") - TPNumber("1", "10", "3") ==
               TPNumber("E", "16", "3"));
    // - negative
    TEST_CHECK(TPNumber("E", "16", "3") - TPNumber("-1", "10", "3") ==
               TPNumber("F", "16", "3"));

    TEST_CASE("operator*");
    TEST_CHECK(TPNumber("2", "10", "3") * TPNumber("2", "10", "3") ==
               TPNumber("4", "10", "3"));
    // mul negative
    TEST_CHECK(
        (TPNumber("-2", "10", "3") * TPNumber("-2", "10", "3")).GetNumber() == 4);
    // mul negative first
    TEST_CHECK(
        (TPNumber("-2", "10", "3") * TPNumber("2", "10", "3")).GetNumber() == -4);

    TEST_CASE("operator/");
    TEST_CHECK(TPNumber("4", "10", "3") / TPNumber("2", "10", "3") ==
               TPNumber("2", "10", "3"));
    // div negative
    TEST_CHECK(
        (TPNumber("-4", "10", "3") / TPNumber("-2", "10", "3")).GetNumber() == 2);
    // div negative first
    TEST_CHECK(
        (TPNumber("-4", "10", "3") / TPNumber("2", "10", "3")).GetNumber() == -2);

    TEST_CASE("operator!");
    TEST_CHECK((!TPNumber("-4", "10", "3")).GetNumber() == 1.0 / -4);
    TPNumber n = TPNumber("-4", "10", "3");
    TEST_CHECK((!!n) == n);

    TEST_CASE("Sqr");
    TEST_CHECK(TPNumber("-4", "10", "3").Sqr().GetNumber() == 16);
}

#endif // #ifdef RUN_TESTS
#endif // #ifndef PNUMBER_CC
