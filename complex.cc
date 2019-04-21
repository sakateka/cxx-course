#ifndef COMPLEX_CC
#define COMPLEX_CC
#include <cmath>
#include <string>
#include <sstream>
#include <cstdlib>
#include <cstring>

namespace NComplex {
    class invalid_number : public std::invalid_argument {
    public:
        explicit invalid_number(const std::string& message)
            : std::invalid_argument(message) {
        }
    };
    static const char* Delimiter = "+i*";

    class TComplex {
    public:
        double Real;
        double Imagn;

        TComplex(double r = 0, double i = 0)
            : Real(r)
            , Imagn(i) {
        }

        TComplex(const TComplex& c)
            : Real(c.Real)
            , Imagn(c.Imagn) {
        }

        explicit TComplex(std::string& c)
            : Real(TComplex::parse_double(std::strtok(&c[0], Delimiter)))
            , Imagn(TComplex::parse_double(std::strtok(NULL, Delimiter))) {
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
            double re = (Real * rhs.Real + Imagn * rhs.Imagn) /
                        (rhs.Real * rhs.Real + rhs.Imagn * rhs.Imagn);
            double im = (Imagn * rhs.Real - Real * rhs.Imagn) /
                        (rhs.Real * rhs.Real + rhs.Imagn * rhs.Imagn);
            return TComplex(re, im);
        }

        TComplex operator-() const {
            return TComplex(-Real, -Imagn);
        }

        TComplex operator!() const {
            double re = Real / (Real * Real + Imagn * Imagn);
            double im = -Imagn / (Real * Real + Imagn * Imagn);
            return TComplex(re, im);
        }

        bool operator==(const TComplex& rhs) const {
            return Real == rhs.Real && Imagn == rhs.Imagn;
        }

        bool operator!=(const TComplex& rhs) const {
            return !(this == &rhs);
        }

        double AngleRad() const {
            if (Real == 0) {
                if (Imagn > 0) {
                    return M_PI_2;
                }
                if (Imagn < 0) {
                    return -M_PI_2;
                }
            }
            if (Real > 0) {
                return atan(Imagn / Real);
            }
            if (Real < 0) {
                return atan(Imagn / Real) + M_PI;
            }
            return 0.0;
        }

        double AngleDeg() const {
            if (Real == 0) {
                if (Imagn > 0) {
                    return 90.0;
                }
                if (Imagn < 0) {
                    return -90.0;
                }
            }
            if (Real > 0) {
                return (atan(Imagn / Real) * 360) / (2 * M_PI);
            }
            if (Real < 0) {
                return ((atan(Imagn / Real) + M_PI) * 360) / (2 * M_PI);
            }
            return 0.0;
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
    };
}; // namespace NComplex

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
        TComplex a = TComplex(70.1, 1.07);
        TComplex b(a);
        TEST_CHECK(&a.Real != &b.Real && a.Real == b.Real);
        TEST_CHECK(&a.Imagn != &b.Imagn && a.Imagn == b.Imagn);
    }
}

void test_complex_operations() {
    using namespace NComplex;
    TEST_CASE("ToString");
    {
        TComplex c = TComplex(8.2, 9.3);
        if (not TEST_CHECK(c.ToString() == "8.200000+i*9.300000")) {
            TEST_MSG(c.ToString().c_str());
        }
    }
    auto cases = (vector<pair<TComplex, TComplex>>){
        {TComplex(8.2, 9.3002), TComplex(2.8, 1.7003)},
        {TComplex(0.0, 9.3002), TComplex(2.8, 1.7003)},
        {TComplex(8.2, 0.0), TComplex(2.8, 0.0)},
        {TComplex(0.0, 0.0), TComplex(0.0, -1.7003)},
        {TComplex(0.0, 0.0), TComplex(0.0, -1.7003)},
        {TComplex(0.0, 0.0), TComplex(0.0, 0.0)},
        {TComplex(-8.2, 9.3002), TComplex(2.8, 1.7003)},
        {TComplex(8.2, 9.3002), TComplex(-2.8, 1.7003)},
        {TComplex(8.2, 9.3002), TComplex(-2.8, -1.7003)},
        {TComplex(8.2, -9.3002), TComplex(-2.8, -1.7003)},
        {TComplex(-8.2, -9.3002), TComplex(-2.8, -1.7003)},
    };
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
            if (not TEST_CHECK(to_string(r.Real) == to_string(cc.real()) &&
                               to_string(r.Imagn) == to_string(cc.imag()))) {
                TEST_MSG("Case a=%s, b=%s: a/b=%s != %s+i*%s",
                         c.first.ToString().c_str(), c.second.ToString().c_str(),
                         r.ToString().c_str(),
                         to_string(cc.real()).c_str(), to_string(cc.imag()).c_str());
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
            if (not TEST_CHECK(to_string(r1.Real) == to_string(aa.real()) &&
                               to_string(r1.Imagn) == to_string(aa.imag()) &&
                               to_string(r2.Real) == to_string(bb.real()) &&
                               to_string(r2.Imagn) == to_string(bb.imag()))) {
                TEST_MSG("Case a=%s, b=%s: !a=%s != %s+i*%s and !b=%s != %s+i*%s",
                         c.first.ToString().c_str(), c.second.ToString().c_str(),
                         r1.ToString().c_str(),
                         to_string(aa.real()).c_str(), to_string(aa.imag()).c_str(),
                         r2.ToString().c_str(),
                         to_string(bb.real()).c_str(), to_string(bb.imag()).c_str());
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
            complex<double> aa = abs(ac);
            complex<double> bb = abs(bc);
            TComplex r1 = c.first.Abs();
            TComplex r2 = c.second.Abs();
            if (not TEST_CHECK(to_string(r1.Real) == to_string(aa.real()) &&
                               to_string(r1.Imagn) == to_string(aa.imag()) &&
                               to_string(r2.Real) == to_string(bb.real()) &&
                               to_string(r2.Imagn) == to_string(bb.imag()))) {
                TEST_MSG("Case a=%s, b=%s: a.Abs()=%s != %s+i*%s and b.Abs()=%s != %s+i*%s",
                         c.first.ToString().c_str(), c.second.ToString().c_str(),
                         r1.ToString().c_str(),
                         to_string(aa.real()).c_str(), to_string(aa.imag()).c_str(),
                         r2.ToString().c_str(),
                         to_string(bb.real()).c_str(), to_string(bb.imag()).c_str());
            }
        }
    }
    TEST_CASE("AngleRad");
    {
        for (auto& c : cases) {
            complex<double> ac(c.first.Real, c.first.Imagn);
            complex<double> bc(c.second.Real, c.second.Imagn);
            complex<double> aa = arg(ac);
            complex<double> bb = arg(bc);
            TComplex r1 = c.first.AngleRad();
            TComplex r2 = c.second.AngleRad();
            if (not TEST_CHECK(to_string(r1.Real) == to_string(aa.real()) &&
                               to_string(r1.Imagn) == to_string(aa.imag()) &&
                               to_string(r2.Real) == to_string(bb.real()) &&
                               to_string(r2.Imagn) == to_string(bb.imag()))) {
                TEST_MSG("Case a=%s, b=%s: a.AngleRad()=%s != %s+i*%s and b.AngleRad()=%s != %s+i*%s",
                         c.first.ToString().c_str(), c.second.ToString().c_str(),
                         r1.ToString().c_str(),
                         to_string(aa.real()).c_str(), to_string(aa.imag()).c_str(),
                         r2.ToString().c_str(),
                         to_string(bb.real()).c_str(), to_string(bb.imag()).c_str());
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
}
#endif // #ifdef RUN_TESTS
#endif // #ifdef COMPLEX_CC
