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
            if ((rhs.Real != 0) && (rhs.Imagn != 0)) {
                double re = (Real * rhs.Real + Imagn * rhs.Imagn) /
                            (rhs.Real * rhs.Real + rhs.Imagn * rhs.Imagn);
                double im = (Imagn * rhs.Real - Real * rhs.Imagn) /
                            (rhs.Real * rhs.Real + rhs.Imagn * rhs.Imagn);
                return TComplex(re, im);
            }
            return TComplex(0, 0);
        }

        TComplex operator-() const {
            return TComplex(-Real, -Imagn);
        }

        TComplex operator!() const {
            if ((Real != 0) && (Imagn != 0)) {
                double re = Real / (Real * Real + Imagn * Imagn);
                double im = Imagn / (Real * Real + Imagn * Imagn);
                return TComplex(re, im);
            }
            return TComplex(0, 0);
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

        double Magnitude() const {
            return sqrt(Real * Real + Imagn * Imagn);
        }

        TComplex Sqr() {
            double re = Real * Real - Imagn * Imagn;
            double im = Real * Imagn + Imagn * Real;
            return TComplex(re, im);
        }

        TComplex Pow(int n) const {
            double fi = AngleRad();
            double mg = Magnitude();
            double re = pow(mg, n) * (cos(n * fi));
            double im = pow(mg, n) * (sin(n * fi));
            return TComplex(re, im);
        }

        TComplex Root(int n, int i) const {
            double fi = AngleRad();
            double mg = Magnitude();
            double re = pow(mg, 1 / n) * (cos((fi + 2 * (i - 1) * M_PI) / n));
            double im = pow(mg, 1 / n) * (sin((fi + 2 * (i - 1) * M_PI) / n));
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
using namespace std;

//
// Constructors
//
void test_complex_constructor() {
    using namespace NComplex;
    TEST_CASE("ConstructorString");
    {
        string cStr = "10.1+i*2.3";
        TComplex c = TComplex(cStr);
        TEST_CHECK(c.Real == 10.1);
        TEST_CHECK(c.Imagn == 2.3);
        c.AngleRad();
    }
    {
        string bad_r = "bla+i*2.3";
        TEST_EXCEPTION(new TComplex(bad_r), invalid_number);
        string bad_i = "10+i*bla";
        TEST_EXCEPTION(new TComplex(bad_i), invalid_number);
    }
    TEST_CASE("CopyConstructor");
    {
        TComplex a = TComplex(10, 1);
        TComplex b(a);
        TEST_CHECK(&a.Real != &b.Real && a.Real == b.Real);
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
}
#endif // #ifdef RUN_TESTS
#endif // #ifdef COMPLEX_CC
