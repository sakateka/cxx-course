#ifndef FRACTIONAL_CC
#define FRACTIONAL_CC
#include <algorithm>
#include <iostream>
#include <exception>
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

        int Numerator() const {
            return numerator;
        }
        int Denominator() const {
            return denominator;
        }

        bool operator==(const TFrac& rhs) const {
            return Numerator() == rhs.Numerator() and
                   Denominator() == rhs.Denominator();
        }
        TFrac operator+(const TFrac& rhs) const {
            return TFrac(Numerator() * rhs.Denominator() +
                             rhs.Numerator() * Denominator(),
                         Denominator() * rhs.Denominator());
        }
        TFrac operator-(const TFrac& rhs) const {
            return TFrac(Numerator() * rhs.Denominator() -
                             rhs.Numerator() * Denominator(),
                         Denominator() * rhs.Denominator());
        }
        TFrac operator*(const TFrac& rhs) {
            return TFrac(Numerator() * rhs.Numerator(),
                         Denominator() * rhs.Denominator());
        }
        TFrac operator/(const TFrac& rhs) {
            if (rhs.Numerator() == 0) {
                throw std::domain_error("result to denominator == 0");
            }
            return TFrac(Numerator() * rhs.Denominator(),
                         Denominator() * rhs.Numerator());
        }

        bool operator<(const TFrac& rhs) const {
            return Numerator() * rhs.Denominator() < rhs.Numerator() * Denominator();
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
    };

    std::ostream& operator<<(std::ostream& output, const TFrac& r) {
        output << r.Numerator() << "/" << r.Denominator();
        return output;
    }
    std::istream& operator>>(std::istream& input, TFrac& r) {
        int n = r.Numerator(), d = r.Denominator();
        input >> n;
        input.ignore(1);
        input >> d;
        r = TFrac(n, d);
        return input;
    }
} // namespace NFrac

#ifdef RUN_TESTS
using NFrac::TFrac;
#include <sstream>
#include <set>
#include <vector>
#include <map>
using namespace std;
int main() {
    {
        const TFrac r(3, 10);
        if (r.Numerator() != 3 || r.Denominator() != 10) {
            cout << "TFrac(3, 10) != 3/10" << endl;
            return 1;
        }
    }

    {
        const TFrac r(8, 12);
        if (r.Numerator() != 2 || r.Denominator() != 3) {
            cout << "TFrac(8, 12) != 2/3" << endl;
            return 2;
        }
    }

    {
        const TFrac r(-4, 6);
        if (r.Numerator() != -2 || r.Denominator() != 3) {
            cout << "TFrac(-4, 6) != -2/3" << endl;
            return 3;
        }
    }

    {
        const TFrac r(4, -6);
        if (r.Numerator() != -2 || r.Denominator() != 3) {
            cout << "TFrac(4, -6) != -2/3" << endl;
            return 3;
        }
    }

    {
        const TFrac r(0, 15);
        if (r.Numerator() != 0 || r.Denominator() != 1) {
            cout << "TFrac(0, 15) != 0/1" << endl;
            return 4;
        }
    }

    {
        const TFrac defaultConstructed;
        if (defaultConstructed.Numerator() != 0 ||
            defaultConstructed.Denominator() != 1) {
            cout << "TFrac() != 0/1" << endl;
            return 5;
        }
    }
    {
        TFrac r1(4, 6);
        TFrac r2(2, 3);
        bool equal = r1 == r2;
        if (!equal) {
            cout << "4/6 != 2/3" << endl;
            return 1;
        }
    }

    {
        TFrac a(2, 3);
        TFrac b(4, 3);
        TFrac c = a + b;
        bool equal = c == TFrac(2, 1);
        if (!equal) {
            cout << "2/3 + 4/3 != 2" << endl;
            return 2;
        }
    }

    {
        TFrac a(5, 7);
        TFrac b(2, 9);
        TFrac c = a - b;
        bool equal = c == TFrac(31, 63);
        if (!equal) {
            cout << "5/7 - 2/9 != 31/63" << endl;
            return 3;
        }
    }
    {
        TFrac a(2, 3);
        TFrac b(4, 3);
        TFrac c = a * b;
        bool equal = c == TFrac(8, 9);
        if (!equal) {
            cout << "2/3 * 4/3 != 8/9" << endl;
            return 1;
        }
    }

    {
        TFrac a(5, 4);
        TFrac b(15, 8);
        TFrac c = a / b;
        bool equal = c == TFrac(2, 3);
        if (!equal) {
            cout << "5/4 / 15/8 != 2/3" << endl;
            return 2;
        }
    }
    {
        ostringstream output;
        output << TFrac(-6, 8);
        if (output.str() != "-3/4") {
            cout << "TFrac(-6, 8) should be written as \"-3/4\"" << endl;
            return 1;
        }
    }

    {
        istringstream input("5/7");
        TFrac r;
        input >> r;
        bool equal = r == TFrac(5, 7);
        if (!equal) {
            cout << "5/7 is incorrectly read as " << r << endl;
            return 2;
        }
    }

    {
        istringstream input("5/7 10/8");
        TFrac r1, r2;
        input >> r1 >> r2;
        bool correct = r1 == TFrac(5, 7) && r2 == TFrac(5, 4);
        if (!correct) {
            cout << "Multiple values are read incorrectly: " << r1 << " " << r2
                 << endl;
            return 3;
        }

        input >> r1;
        input >> r2;
        correct = r1 == TFrac(5, 7) && r2 == TFrac(5, 4);
        if (!correct) {
            cout << "Read from empty stream shouldn't change arguments: " << r1 << " "
                 << r2 << endl;
            return 4;
        }
    }
    {
        const set<TFrac> rs = {{1, 2}, {1, 25}, {3, 4}, {3, 4}, {1, 2}};
        for (auto& r : rs) {
            cout << r << " ";
        }
        cout << endl;
        if (rs.size() != 3) {
            cout << "Wrong amount of items in the set" << endl;
            return 1;
        }

        vector<TFrac> v;
        for (auto x : rs) {
            v.push_back(x);
        }
        if (v != vector<TFrac>{{1, 25}, {1, 2}, {3, 4}}) {
            cout << "TFracs comparison works incorrectly" << endl;
            return 2;
        }
    }

    {
        map<TFrac, int> count;
        ++count[{1, 2}];
        ++count[{1, 2}];

        ++count[{2, 3}];

        if (count.size() != 2) {
            cout << "Wrong amount of items in the map" << endl;
            return 3;
        }
    }
    try {
        TFrac r(1, 0);
        cout << r << " Doesn't throw in case of zero denominator" << endl;
        return 1;
    } catch (invalid_argument&) {
    }

    try {
        auto x = TFrac(1, 2) / TFrac(0, 1);
        cout << x << " Doesn't throw in case of division by zero" << endl;
        return 2;
    } catch (domain_error&) {
    }

    cout << "OK" << endl;
    return 0;
}
#endif // #ifdef RUN_TESTS
#endif // #ifndef FRACTIONAL_CC
