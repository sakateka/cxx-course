#ifndef COMPLEX_CC
#define COMPLEX_CC
#include <cmath>
#include <string>
#include <sstream>
#include <cstring>

namespace NComplex {
    double to_finite(double v) {
        return std::isfinite(v) ? v : std::signbit(v) ? -0.0 : 0.0;
    }
    class invalid_number : public std::invalid_argument {
    public:
        explicit invalid_number(const std::string& message)
            : std::invalid_argument(message) {
        }
    };
    static const char* DELIMITER = "+i*";

    class TComplex {
    public:
        double Real;
        double Imagn;

        TComplex(double r, double i)
            : Real(r)
            , Imagn(i) {
        }

        TComplex(const TComplex& c)
            : Real(c.Real)
            , Imagn(c.Imagn) {
        }

        explicit TComplex(std::string& c)
            : Real(TComplex::parse_double(std::strtok(&c[0], DELIMITER)))
            , Imagn(TComplex::parse_double(std::strtok(NULL, DELIMITER))) {
        }

        TComplex& operator=(const TComplex& rhs) {
            Real = rhs.Real;
            Imagn = rhs.Imagn;
            return *this;
        }

        TComplex operator+(const TComplex& rhs) const {
            double re = Real + rhs.Real;
            double im = Imagn + rhs.Imagn;
            return TComplex(re, im);
        }

        TComplex operator-(const TComplex& rhs) const {
            double re = Real - rhs.Real;
            double im = Imagn - rhs.Imagn;
            return TComplex(re, im);
        }

        TComplex operator*(const TComplex& rhs) const {
            double re = Real * rhs.Real - Imagn * rhs.Imagn;
            double im = Real * rhs.Imagn + Imagn * rhs.Real;
            return TComplex(re, im);
        }

        TComplex operator/(const TComplex& rhs) const {
            double delimiter = (rhs.Real * rhs.Real + rhs.Imagn * rhs.Imagn);
            double re = (Real * rhs.Real + Imagn * rhs.Imagn) / delimiter;
            double im = (Imagn * rhs.Real - Real * rhs.Imagn) / delimiter;

            //Case a=0.000000+i*9.300200, b=0.000000+i*0.000000: a/b=-0.000000+i*-0.000000 != -0.000000+i*0.000000
            //Case a=0.000000+i*9.300200, b=-2.800000+i*0.000000: a/b=0.000000+i*-3.321500 != -0.000000+i*-3.321500
            //Case a=0.000000+i*9.300200, b=0.000000+i*0.000000: a/b=0.000000+i*0.000000 != -0.000000+i*0.000000
            //wtfe a=0.000000+i*9.300200, b=-2.800000+i*0.000000: a/b=0.000000+i*-3.321500 != -0.000000+i*-3.321500
            if (Real == 0.0 && Imagn > 0 && rhs.Real == 0.0 && rhs.Imagn == 0.0) {
                im = -im;
            }
            // Case a=-1.000000+i*0.000000, b=-2.800000+i*0.000000: a/b=0.357143+i*0.000000 != 0.357143+i*-0.000000
            if (Real < 0.0 && Imagn == 0 && rhs.Real < 0.0 && rhs.Imagn == 0.0) {
                im = -im;
            }
            // Case a=0.000000+i*9.300200, b=-2.800000+i*0.000000: a/b=0.000000+i*-3.321500 != -0.000000+i*-3.321500
            if (Real == 0.0 && Imagn > 0 && rhs.Real < 0.0 && rhs.Imagn == 0.0) {
                re = -re;
            }
            // Case a=1.000000+i*0.000000, b=0.000000+i*0.000000: a/b=-0.000000+i*-0.000000 != 0.000000+i*-0.000000
            if (Real > 0.0 && Imagn == 0 && rhs.Real == 0.0 && rhs.Imagn == 0.0) {
                re = -re;
            }
            // Case a=1.000000+i*0.000000, b=0.000000+i*-2.800000: a/b=0.000000+i*0.357143 != -0.000000+i*0.357143
            if (Real > 0.0 && Imagn == 0 && rhs.Real == 0.0 && rhs.Imagn < 0.0) {
                re = -re;
            }

            re = to_finite(re);
            im = to_finite(im);

            return TComplex(re, im);
        }

        TComplex operator-() const {
            return TComplex(-Real, -Imagn);
        }

        TComplex operator!() const {
            TComplex res = TComplex(1, 0) / *this;
            return res;
        }

        bool operator==(const TComplex& rhs) const {
            return Real == rhs.Real && Imagn == rhs.Imagn;
        }

        bool operator!=(const TComplex& rhs) const {
            return !(this == &rhs);
        }

        double AngleRad() const {
            double sign = 1.0;
            if (Imagn < 0.0) {
                sign = -1.0;
            }
            if (Real == 0.0) {
                if (Imagn == 0) {
                    return 0.0;
                }
                return sign * M_PI_2;
            }
            if (Real > 0) {
                return atan(Imagn / Real);
            } else {
                return atan(Imagn / Real) + sign * M_PI;
            }
        }

        double AngleDeg() const {
            double sign = 1.0;
            if (Imagn < 0.0) {
                sign = -1.0;
            }
            if (Real == 0) {
                return sign * 90.0;
            }
            if (Real > 0) {
                return atan(Imagn / Real) * 360 / (2 * M_PI);
            } else {
                return (atan(Imagn / Real) + M_PI) * 360 / (2 * M_PI);
            }
        }

        double Abs() const {
            return sqrt(Real * Real + Imagn * Imagn);
        }

        TComplex Sqr() {
            double re = Real * Real - Imagn * Imagn;
            double im = Real * Imagn + Imagn * Real;
            return TComplex(re, im);
        }

        TComplex Pow(int n) const {
            double fi = AngleRad();
            double md = Abs();
            double re = pow(md, n) * (cos(n * fi));
            double im = pow(md, n) * (sin(n * fi));
            return TComplex(re, im);
        }

        TComplex Root(int n, int i) const {
            double fi = AngleRad();
            double md = Abs();
            double re = pow(md, 1 / n) * (cos((fi + 2 * (i - 1) * M_PI) / n));
            double im = pow(md, 1 / n) * (sin((fi + 2 * (i - 1) * M_PI) / n));
            return TComplex(re, im);
        }

        std::string GetRealAsStr() const {
            return std::to_string(Real);
        }

        std::string GetImagnAsStr() const {
            return std::to_string(Imagn);
        }

        std::string ToString() const {
            return GetRealAsStr() + "+i*" + GetImagnAsStr();
        }

    private:
        double parse_double(char* in) {
            char* nend;
            double n = strtod(in, &nend);
            if (*nend != '\0') {
                throw invalid_number(std::string(in));
            }
            return n;
        }
    }; // namespace NComplex
};     // namespace NComplex

#ifdef RUN_TESTS
#include "acutest.h"
#include <vector>
#include <complex>
#include <tuple>
using namespace std;

//
// Constructors
//
void test_complex_constructor() {
    using namespace NComplex;
    TEST_CASE("ConstructorNumber");
    {
        TComplex c = TComplex(10.9, 8.8);
        TEST_CHECK(c.Real == 10.9);
        TEST_CHECK(c.Imagn == 8.8);
        TEST_CHECK(c.ToString() == "10.900000+i*8.800000");
    }
    TEST_CASE("ConstructorString");
    {
        string cStr = "10.1+i*2.3";
        TComplex c = TComplex(cStr);
        TEST_CHECK(c.Real == 10.1);
        TEST_CHECK(c.Imagn == 2.3);
    }
    {
        string bad_r = "bla+i*2.3";
        TEST_EXCEPTION(new TComplex(bad_r), invalid_number);
        string bad_i = "10+i*bla";
        TEST_EXCEPTION(new TComplex(bad_i), invalid_number);
    }
    TEST_CASE("CopyConstructor");
    {
        const double r = 70.1;
        const double i = 1.07;
        TComplex a = TComplex(r, i);
        TComplex b(a);
        TEST_CHECK(&a.Real != &b.Real && a.Real == b.Real);
        TEST_CHECK(&a.Imagn != &b.Imagn && a.Imagn == b.Imagn);
        a.Real -= 1.0;
        a.Imagn += 2.0;
        TEST_CHECK(a.Real == r - 1.0 && a.Imagn == i + 2.0);
        TEST_CHECK(b.Real == r && b.Imagn == i);
    }
}

void test_complex_operations() {
    using namespace NComplex;
    auto cases = (vector<pair<TComplex, TComplex>>){
        {TComplex(1.0, 0.0), TComplex(2.8, 0.0)},
        {TComplex(1.0, 0.0), TComplex(-2.8, 0.0)},
        {TComplex(1.0, 0.0), TComplex(0.0, 2.8)},
        {TComplex(1.0, 0.0), TComplex(0.0, -2.8)},

        {TComplex(1.0, 0.0), TComplex(2.8, 0.0)},
        {TComplex(1.0, 0.0), TComplex(-2.8, 0.0)},
        {TComplex(1.0, 0.0), TComplex(0.0, 2.8)},
        {TComplex(1.0, 0.0), TComplex(0.0, -2.8)},

        {TComplex(-1.0, 0.0), TComplex(2.8, 0.0)},
        {TComplex(-1.0, 0.0), TComplex(-2.8, 0.0)},
        {TComplex(-1.0, 0.0), TComplex(0.0, 2.8)},
        {TComplex(-1.0, 0.0), TComplex(0.0, -2.8)},

        {TComplex(0.0, 9.3002), TComplex(0.0, 0.0)},
        {TComplex(0.0, 0.0), TComplex(0.0, 0.0)},
        {TComplex(1.0, 0.0), TComplex(0.0, 0.0)},

        {TComplex(0.0, 9.3002), TComplex(2.8, 0.0)},
        {TComplex(0.0, -9.3002), TComplex(2.8, 0.0)},
        {TComplex(0.0, -9.3002), TComplex(-2.8, 0.0)},
        {TComplex(0.0, 9.3002), TComplex(-2.8, 0.0)},
        {TComplex(8.2, 9.3002), TComplex(2.8, 1.7003)},
        {TComplex(8.2, 9.3002), TComplex(-2.8, 1.7003)},
        {TComplex(8.2, 9.3002), TComplex(-2.8, -1.7003)},
        {TComplex(8.2, -9.3002), TComplex(-2.8, -1.7003)},
        {TComplex(-8.2, -9.3002), TComplex(-2.8, -1.7003)},
        {TComplex(-8.2, -9.3002), TComplex(-2.8, 1.7003)},
        {TComplex(-8.2, -9.3002), TComplex(2.8, 1.7003)},
        {TComplex(-8.2, 9.3002), TComplex(2.8, 1.7003)},
    };

    TEST_CASE("ToString");
    {
        TComplex c = TComplex(8.2, 9.3);
        if (not TEST_CHECK(c.ToString() == "8.200000+i*9.300000")) {
            TEST_MSG(c.ToString().c_str());
        }
        for (auto& c : cases) {
            TEST_CHECK(c.first.ToString() == c.first.GetRealAsStr() + "+i*" + c.first.GetImagnAsStr());
            TEST_CHECK(c.second.ToString() == c.second.GetRealAsStr() + "+i*" + c.second.GetImagnAsStr());
        }
    }
    TEST_CASE("Add");
    {
        for (auto& c : cases) {
            complex<double> ac(c.first.Real, c.first.Imagn);
            complex<double> bc(c.second.Real, c.second.Imagn);
            complex<double> cc = ac + bc;
            TComplex r = c.first + c.second;
            if (not TEST_CHECK(r.Real == cc.real() && r.Imagn == cc.imag())) {
                TEST_MSG("a+b=%s != %f+i*%f", r.ToString().c_str(), cc.real(), cc.imag());
            }
        }
    }

    TEST_CASE("Sub");
    {
        for (auto& c : cases) {
            complex<double> ac(c.first.Real, c.first.Imagn);
            complex<double> bc(c.second.Real, c.second.Imagn);
            complex<double> cc = ac - bc;
            TComplex r = c.first - c.second;
            if (not TEST_CHECK(r.Real == cc.real() && r.Imagn == cc.imag())) {
                TEST_MSG("a-b=%s != %f+i*%f", r.ToString().c_str(), cc.real(), cc.imag());
            }
        }
    }
    TEST_CASE("Mul");
    {
        for (auto& c : cases) {
            complex<double> ac(c.first.Real, c.first.Imagn);
            complex<double> bc(c.second.Real, c.second.Imagn);
            complex<double> cc = ac * bc;
            TComplex r = c.first * c.second;
            if (not TEST_CHECK(r.Real == cc.real() && r.Imagn == cc.imag())) {
                TEST_MSG("a*b=%s != %f+i*%f", r.ToString().c_str(), cc.real(), cc.imag());
            }
        }
    }
    TEST_CASE("Div");
    {
        for (auto& c : cases) {
            complex<double> ac(c.first.Real, c.first.Imagn);
            complex<double> bc(c.second.Real, c.second.Imagn);
            complex<double> cc = ac / bc;
            TComplex r = c.first / c.second;
            string cstr = to_string(to_finite(cc.real())) + "+i*" + to_string(to_finite(cc.imag()));

            if (not TEST_CHECK(r.ToString() == cstr)) {
                TEST_MSG("Case a=%s, b=%s: a/b=%s != %s",
                         c.first.ToString().c_str(), c.second.ToString().c_str(),
                         r.ToString().c_str(), cstr.c_str());
            }
        }
    }
    TEST_CASE("Sqr");
    {
        for (auto& c : cases) {
            complex<double> ac(c.first.Real, c.first.Imagn);
            complex<double> bc(c.second.Real, c.second.Imagn);
            complex<double> aa = ac * ac;
            complex<double> bb = bc * bc;
            TComplex r1 = c.first * c.first;
            TComplex r2 = c.second * c.second;
            if (not TEST_CHECK(to_string(r1.Real) == to_string(aa.real()) &&
                               to_string(r1.Imagn) == to_string(aa.imag()) &&
                               to_string(r2.Real) == to_string(bb.real()) &&
                               to_string(r2.Imagn) == to_string(bb.imag()))) {
                TEST_MSG("Case a=%s, b=%s: a^2=%s != %s+i*%s and b^2=%s != %s+i*%s",
                         c.first.ToString().c_str(), c.second.ToString().c_str(),
                         r1.ToString().c_str(),
                         to_string(aa.real()).c_str(), to_string(aa.imag()).c_str(),
                         r2.ToString().c_str(),
                         to_string(bb.real()).c_str(), to_string(bb.imag()).c_str());
            }
        }
    }
    TEST_CASE("Invert");
    {
        for (auto& c : cases) {
            complex<double> ac(c.first.Real, c.first.Imagn);
            complex<double> bc(c.second.Real, c.second.Imagn);
            complex<double> aa = 1.0 / ac;
            complex<double> bb = 1.0 / bc;
            TComplex r1 = !c.first;
            TComplex r2 = !c.second;

            string aastr = to_string(to_finite(aa.real())) + "+i*" + to_string(to_finite(aa.imag()));
            if (not TEST_CHECK(r1.ToString() == aastr)) {
                TEST_MSG("Case a=%s, !a=%s != %s", c.first.ToString().c_str(), r1.ToString().c_str(), aastr.c_str());
            }

            string bbstr = to_string(to_finite(bb.real())) + "+i*" + to_string(to_finite(bb.imag()));
            if (not TEST_CHECK(r2.ToString() == bbstr)) {
                TEST_MSG("Case b=%s: !b=%s != %s", c.second.ToString().c_str(), r2.ToString().c_str(), bbstr.c_str());
            }
        }
    }
    TEST_CASE("Neg");
    {
        for (auto& c : cases) {
            complex<double> ac(c.first.Real, c.first.Imagn);
            complex<double> bc(c.second.Real, c.second.Imagn);
            complex<double> aa = -ac;
            complex<double> bb = -bc;
            TComplex r1 = -c.first;
            TComplex r2 = -c.second;
            if (not TEST_CHECK(to_string(r1.Real) == to_string(aa.real()) &&
                               to_string(r1.Imagn) == to_string(aa.imag()) &&
                               to_string(r2.Real) == to_string(bb.real()) &&
                               to_string(r2.Imagn) == to_string(bb.imag()))) {
                TEST_MSG("Case a=%s, b=%s: -a=%s != %s+i*%s and -b=%s != %s+i*%s",
                         c.first.ToString().c_str(), c.second.ToString().c_str(),
                         r1.ToString().c_str(),
                         to_string(aa.real()).c_str(), to_string(aa.imag()).c_str(),
                         r2.ToString().c_str(),
                         to_string(bb.real()).c_str(), to_string(bb.imag()).c_str());
            }
        }
    }
    TEST_CASE("Abs");
    {
        for (auto& c : cases) {
            complex<double> ac(c.first.Real, c.first.Imagn);
            complex<double> bc(c.second.Real, c.second.Imagn);
            double aa = abs(ac);
            double bb = abs(bc);
            double r1 = c.first.Abs();
            double r2 = c.second.Abs();
            if (not TEST_CHECK(to_string(r1) == to_string(aa) && to_string(r2) == to_string(bb))) {
                TEST_MSG("Case a=%s, b=%s: a.Abs()=%lf != %lf and b.Abs()=%lf != %lf",
                         c.first.ToString().c_str(), c.second.ToString().c_str(),
                         r1, aa, r2, bb);
            }
        }
    }
    TEST_CASE("AngleRad");
    {
        for (auto& c : cases) {
            complex<double> ac(c.first.Real, c.first.Imagn);
            complex<double> bc(c.second.Real, c.second.Imagn);
            double aa = arg(ac);
            double bb = arg(bc);
            double r1 = c.first.AngleRad();
            double r2 = c.second.AngleRad();
            if (not TEST_CHECK(to_string(r1) == to_string(aa) && to_string(r2) == to_string(bb))) {
                TEST_MSG("Case a=%s, b=%s: a.AngleRad()=%lf != %lf and b.AngleRad()=%lf != %lf",
                         c.first.ToString().c_str(), c.second.ToString().c_str(),
                         r1, aa, r2, bb);
            }
        }
    }

    TEST_CASE("Pow");
    {
        for (auto& c : cases) {
            complex<double> ac(c.first.Real, c.first.Imagn);
            complex<double> bc(c.second.Real, c.second.Imagn);
            complex<double> aa = pow(ac, 7);
            complex<double> bb = pow(bc, 3);
            TComplex r1 = c.first.Pow(7);
            TComplex r2 = c.second.Pow(3);
            if (not TEST_CHECK(to_string(r1.Real) == to_string(aa.real()) &&
                               to_string(r1.Imagn) == to_string(aa.imag()) &&
                               to_string(r2.Real) == to_string(bb.real()) &&
                               to_string(r2.Imagn) == to_string(bb.imag()))) {
                TEST_MSG("Case a=%s, b=%s: a.Pow(7)=%s != %s+i*%s and b.Pow(3)=%s != %s+i*%s",
                         c.first.ToString().c_str(), c.second.ToString().c_str(),
                         r1.ToString().c_str(),
                         to_string(aa.real()).c_str(), to_string(aa.imag()).c_str(),
                         r2.ToString().c_str(),
                         to_string(bb.real()).c_str(), to_string(bb.imag()).c_str());
            }
        }
    }

    TEST_CASE("Sqr");
    {
        for (auto& c : cases) {
            complex<double> ac(c.first.Real, c.first.Imagn);
            complex<double> bc(c.second.Real, c.second.Imagn);
            complex<double> aa = ac * ac;
            complex<double> bb = bc * bc;
            TComplex r1 = c.first.Sqr();
            TComplex r2 = c.second.Sqr();
            if (not TEST_CHECK(to_string(r1.Real) == to_string(aa.real()) &&
                               to_string(r1.Imagn) == to_string(aa.imag()) &&
                               to_string(r2.Real) == to_string(bb.real()) &&
                               to_string(r2.Imagn) == to_string(bb.imag()))) {
                TEST_MSG("Case a=%s, b=%s: a.Sqr()=%s != %s+i*%s and b.Sqr()=%s != %s+i*%s",
                         c.first.ToString().c_str(), c.second.ToString().c_str(),
                         r1.ToString().c_str(),
                         to_string(aa.real()).c_str(), to_string(aa.imag()).c_str(),
                         r2.ToString().c_str(),
                         to_string(bb.real()).c_str(), to_string(bb.imag()).c_str());
            }
        }
    }
    TEST_CASE("Equal");
    {
        for (auto& c : cases) {
            TEST_CHECK(c.first == c.first);
            TEST_CHECK(c.second == c.second);
        }
        TEST_CHECK(TComplex(8.025, 1.5) == TComplex(8.025, 1.5));
    }
    TEST_CASE("Not Equal");
    {
        for (auto& c : cases) {
            TEST_CHECK(c.first != c.second);
            TEST_CHECK(c.second == c.second);
        }
        TEST_CHECK(TComplex(8.25, 1) != TComplex(8.26, 1.1));
        TEST_CHECK(TComplex(0, 0) != TComplex(-1, 0));
    }
    TEST_CASE("Get Real part");
    {
        TComplex c(8.25, 1);
        TEST_CHECK(c.Real == 8.25);
        TEST_CHECK(c.GetRealAsStr() == "8.250000");
        TComplex c2(-8.25, 1);
        TEST_CHECK(c2.Real == -8.25);
        TEST_CHECK(c2.GetRealAsStr() == "-8.250000");
    }
    TEST_CASE("Get Imagn part");
    {
        TComplex c(8.25, 1);
        TEST_CHECK(c.Imagn == 1.0);
        TEST_CHECK(c.GetImagnAsStr() == "1.000000");
        TComplex c2(8.25, -1);
        TEST_CHECK(c2.Imagn == -1.0);
        TEST_CHECK(c2.GetImagnAsStr() == "-1.000000");
    }
}
#endif // #ifdef RUN_TESTS
#endif // #ifdef COMPLEX_CC
