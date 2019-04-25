#ifndef FRACTIONAL_CC
#define FRACTIONAL_CC
#include <algorithm>
#include <iostream>
#include <exception>
#include <string>
#include <sstream>
#include <assert.h>

namespace NFrac {
    class TFrac {
    public:
        TFrac() {
        }

        TFrac(int new_numerator, int new_denominator) {
            if (new_denominator == 0) {
                throw std::invalid_argument("denominator == 0");
            }
            int sign = 1;
            if (new_denominator < 0) {
                new_denominator = abs(new_denominator);
                sign = -1;
            }
            int rgcd = gcd(abs(new_numerator), abs(new_denominator));
            assert(rgcd != 0);
            numerator = new_numerator / rgcd * sign;
            denominator = new_denominator / rgcd;
        }

        explicit TFrac(std::string frac) {
            std::istringstream s(frac);
            int n = 0, d = 1;
            s >> n;
            if (!s.eof()) {
                char sym = s.peek();
                if (sym == '/' && s.ignore(1) && !s.eof()) {
                    s >> d;
                }
            }
            if (!s.eof()) {
                throw std::invalid_argument(frac);
            }
            *this = TFrac(n, d);
        }

        TFrac Copy() const {
            return TFrac(GetNumerator(), GetDenominator());
        }

        int GetNumerator() const {
            return numerator;
        }
        int GetDenominator() const {
            return denominator;
        }
        std::string GetNumeratorAsStr() const {
            return std::to_string(numerator);
        }
        std::string GetDenominatorAsStr() const {
            return std::to_string(denominator);
        }
        void SetNumerator(int n) {
            TFrac tmp = {n, GetDenominator()}; // simplify and check sign
            numerator = tmp.GetNumerator();
            denominator = tmp.GetDenominator();
        }
        void SetDenominator(int d) {
            TFrac tmp = {GetNumerator(), d}; // simplify and check sign
            numerator = tmp.GetNumerator();
            denominator = tmp.GetDenominator();
        }

        bool operator==(const TFrac& rhs) const {
            return GetNumerator() == rhs.GetNumerator() and
                   GetDenominator() == rhs.GetDenominator();
        }
        TFrac operator+(const TFrac& rhs) const {
            return TFrac(GetNumerator() * rhs.GetDenominator() + rhs.GetNumerator() * GetDenominator(),
                         GetDenominator() * rhs.GetDenominator());
        }
        TFrac operator-(const TFrac& rhs) const {
            return TFrac(GetNumerator() * rhs.GetDenominator() - rhs.GetNumerator() * GetDenominator(),
                         GetDenominator() * rhs.GetDenominator());
        }
        TFrac operator-() const {
            return TFrac(0, 1) - *this;
        }
        TFrac operator*(const TFrac& rhs) const {
            return TFrac(GetNumerator() * rhs.GetNumerator(),
                         GetDenominator() * rhs.GetDenominator());
        }
        TFrac operator/(const TFrac& rhs) const {
            if (rhs.GetNumerator() == 0) {
                throw std::domain_error("result to denominator == 0");
            }
            return TFrac(GetNumerator() * rhs.GetDenominator(),
                         GetDenominator() * rhs.GetNumerator());
        }
        TFrac operator!() const {
            return TFrac(GetDenominator(), GetNumerator());
        }
        TFrac Sqr() const {
            return *this * *this;
        }

        bool operator<(const TFrac& rhs) const {
            return GetNumerator() * rhs.GetDenominator() < rhs.GetNumerator() * GetDenominator();
        }
        bool operator>(const TFrac& rhs) const {
            return !(*this < rhs) && !(*this == rhs);
        }
        std::string ToString() const {
            return GetNumeratorAsStr() + "/" + GetDenominatorAsStr();
        }

    private:
        int numerator = 0;
        int denominator = 1;
        int gcd(long int a, long int b) {
            long int rem = std::min(a, b);
            b = std::max(a, b);
            if (rem == 0) {
                return b;
            }

            do {
                a = rem;
                rem = b % a;
                b = a;
            } while (rem != 0);
            return a;
        }
    }; // class TFrac

    std::ostream& operator<<(std::ostream& output, const TFrac& r) {
        output << r.ToString();
        return output;
    }
    std::istream& operator>>(std::istream& input, TFrac& r) {
        std::string tmp;
        input >> tmp;
        r = TFrac(tmp);
        return input;
    }
} // namespace NFrac

#ifdef RUN_TESTS
#include "acutest.h"
#include <sstream>
#include <set>
#include <vector>
#include <map>
using NFrac::TFrac;
using namespace std;
void test_fractional_construction() {
    TEST_CASE("Constructor");
    {
        const TFrac r(3, 10);
        TEST_CHECK(r.GetNumerator() == 3 && r.GetDenominator() == 10);
    }
    TEST_CASE("Constructor simplify");
    {
        const TFrac r(8, 12);
        TEST_CHECK(r.GetNumerator() == 2 && r.GetDenominator() == 3);
    }
    {
        const TFrac r(-4, 6);
        TEST_CHECK(r.GetNumerator() == -2 && r.GetDenominator() == 3);
    }
    {
        const TFrac r(4, -6);
        TEST_CHECK(r.GetNumerator() == -2 && r.GetDenominator() == 3);
    }
    {
        const TFrac r(0, 15);
        TEST_CHECK(r.GetNumerator() == 0 && r.GetDenominator() == 1);
    }
    TEST_CASE("Default Constructor");
    {
        const TFrac r;
        TEST_CHECK(r.GetNumerator() == 0 && r.GetDenominator() == 1);
    }
    TEST_CASE("Illegal arguments");
    TEST_EXCEPTION(TFrac r(1, 0), invalid_argument);
}

void test_fractional_operations() {
    TEST_CASE("Equal");
    {
        TFrac r1(4, 6);
        TFrac r2(2, 3);
        TEST_CHECK(r1 == r2);
    }

    TEST_CASE("Add");
    {
        TFrac a(2, 3);
        TFrac b(4, 3);
        TEST_CHECK(a + b == TFrac(2, 1));

        TFrac c(1, 2);
        TFrac d(1, 2);
        TEST_CHECK(c + d == TFrac(1, 1));
    }

    TEST_CASE("Sub");
    {
        TFrac a(5, 7);
        TFrac b(2, 9);
        TFrac c = a - b;
        TEST_CHECK(c == TFrac(31, 63));
    }
    TEST_CASE("Mul");
    {
        TFrac a(2, 3);
        TFrac b(4, 3);
        TFrac c = a * b;
        TEST_CHECK(c == TFrac(8, 9));
    }
    TEST_CASE("Div");
    {
        TFrac a(5, 4);
        TFrac b(15, 8);
        TFrac c = a / b;
        TEST_CHECK(c == TFrac(2, 3));
    }
    TEST_CASE("Invert");
    {
        TFrac a(5, 4);
        TFrac b = !a;
        TEST_CHECK(b == TFrac(4, 5));
        TEST_EXCEPTION(!TFrac(0, 1), invalid_argument);
    }
    TEST_CASE("Neg");
    {
        TFrac a(5, 4);
        TFrac b = -a;
        TFrac c = TFrac(0, 1) - TFrac(5, 4);
        if (not TEST_CHECK(b == c)) {
            TEST_MSG("%s != %s", b.ToString().c_str(), c.ToString().c_str());
        }
    }
    TEST_CASE("Less / Great");
    {
        TFrac a(3, 4);
        TFrac b(5, 4);
        TEST_CHECK(a < b);
        TEST_CHECK(b > a);
    }
    TEST_CASE("Sqr");
    {
        TFrac a(5, 4);
        TFrac b = a.Sqr();
        TEST_CHECK(a == TFrac(5, 4));
        TEST_CHECK(b == TFrac(25, 16));
    }
    TEST_CASE("ToString");
    {
        TFrac a(5, 4);
        TEST_CHECK(a.GetNumeratorAsStr() == "5");
        TEST_CHECK(a.GetDenominatorAsStr() == "4");
        TEST_CHECK(a.ToString() == "5/4");
        TFrac b(-5, -4);
        TEST_CHECK(b.GetNumeratorAsStr() == "5");
        TEST_CHECK(b.GetDenominatorAsStr() == "4");
        TEST_CHECK(b.ToString() == "5/4");
        TFrac c(-5, 4);
        TEST_CHECK(c.GetNumeratorAsStr() == "-5");
        TEST_CHECK(c.GetDenominatorAsStr() == "4");
        TEST_CHECK(c.ToString() == "-5/4");
        TFrac d(5, -4);
        TEST_CHECK(d.GetNumeratorAsStr() == "-5");
        TEST_CHECK(d.GetDenominatorAsStr() == "4");
        TEST_CHECK(d.ToString() == "-5/4");
    }
    TEST_CASE("ostream out");
    {
        ostringstream output;
        output << TFrac(-6, 8);
        TEST_CHECK(output.str() == "-3/4");
    }
    TEST_CASE("istream in");
    {
        istringstream input("5/7");
        TFrac r;
        input >> r;
        TEST_CHECK(r == TFrac(5, 7));
    }
    {
        istringstream input("5/7 10/8");
        TFrac r1, r2;
        TEST_CHECK(r1 == TFrac(0, 1) && r2 == TFrac(0, 1));
        input >> r1 >> r2;
        TEST_CHECK(r1 == TFrac(5, 7) && r2 == TFrac(5, 4));

        // Reset if input eof
        input >> r1;
        input >> r2;
        TEST_CHECK(r1 == TFrac(0, 1) && r2 == TFrac(0, 1));

        input = istringstream("9/7 12/4");
        input >> r1;
        input >> r2;
        TEST_CHECK(r1 == TFrac(9, 7) && r2 == TFrac(3, 1));
    }
    TEST_CASE("Set element");
    {
        const set<TFrac> rs = {{1, 2}, {1, 25}, {3, 4}, {3, 4}, {1, 2}};
        TEST_CHECK(rs.size() == 3);
        vector<TFrac> v;
        for (auto x : rs) {
            v.push_back(x);
        }
        TEST_CHECK((v == vector<TFrac>{{1, 25}, {1, 2}, {3, 4}}));
    }
    TEST_CASE("Map key");
    {
        map<TFrac, int> count;
        ++count[{1, 2}];
        ++count[{1, 2}];
        ++count[{2, 3}];
        TEST_CHECK(count.size() == 2);
    }
    TEST_CASE("Zero division");
    TEST_EXCEPTION(TFrac(1, 2) / TFrac(0, 1), domain_error);

    TEST_CASE("SetNumerator and SetDenominator");
    {
        TFrac a(5, 4);
        a.SetNumerator(10);
        TEST_CHECK(a == TFrac(5, 2));
        a.SetDenominator(15);
        TEST_CHECK(a == TFrac(1, 3));
        TEST_EXCEPTION(a.SetDenominator(0), invalid_argument);
    }
    TEST_CASE("Copy");
    {
        TFrac a(5, 4);
        TFrac b = a.Copy();
        a.SetNumerator(1);
        TEST_CHECK(a.GetNumerator() != b.GetNumerator());
    }
}
#endif // #ifdef RUN_TESTS
#endif // #ifndef FRACTIONAL_CC
