#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

class invalid_pnumber : public std::invalid_argument {
public:
  explicit invalid_pnumber(const std::string &message)
      : std::invalid_argument(message) {}
};
class invalid_radix : public std::invalid_argument {
public:
  explicit invalid_radix(const std::string &message)
      : std::invalid_argument(message) {}
};
class invalid_precision : public std::invalid_argument {
public:
  explicit invalid_precision(const std::string &message)
      : std::invalid_argument(message) {}
};

class PNumber {
public:
  PNumber(double n, unsigned b, unsigned c) {
    number = n;
    radix = b;
    precision = c;
  }
  PNumber(const std::string &n, const std::string &b, const std::string &c) {
    SetRadix(parse_radix(b));
    precision = parse_precision(c);
    number = parse_number(n, radix);
  }
  friend std::ostream &operator<<(std::ostream &out, const PNumber &p) {
    out << p.ToString();
    return out;
  }
  PNumber operator+(const PNumber &rhs) const {
    return {number + rhs.number, radix, precision};
  }
  PNumber operator-(const PNumber &rhs) const {
    return {number - rhs.number, radix, precision};
  }
  PNumber operator*(const PNumber &rhs) const {
    return {number * rhs.number, radix, precision};
  }
  PNumber operator/(const PNumber &rhs) const {
    return {number / rhs.number, radix, precision};
  }
  PNumber operator!() const { return {1.0 / number, radix, precision}; }

  PNumber pow2() const { return {number * number, radix, precision}; }

  double GetNumber() const { return number; }
  unsigned GetRadix() const { return radix; }
  std::string GetRadixAsStr() const { return std::to_string(radix); }
  unsigned GetPrecision() const { return precision; }
  std::string GetPrecisionAsStr() const { return std::to_string(precision); }

  std::string ToString() const {
    std::string result;
    if (radix == 10) {
      std::stringstream sresult(result);
      sresult << std::fixed << std::setprecision(precision) << number;
      return sresult.str();
    }
    static const char alphabet[] = "0123456789ABCDEF";
    unsigned n = (unsigned)number;
    double fdouble = (number - n);
    while (n) {
      result += alphabet[n % radix];
      n /= radix;
    }
    std::reverse(result.begin(), result.end());

    if (precision > 0) {
      char fstring[17]; // 0.<double guaranteed precision 15 digits>
      sprintf(fstring, "%.15f", fdouble);
      std::string fs(fstring);
      std::vector<int> fracVec;
      transform(fs.begin() + 2 /*skip 0.*/, fs.end(),
                std::back_inserter(fracVec),
                [](unsigned char c) -> int { return c - '0'; });

      fs.clear();
      fs.resize(precision, '0');
      for (unsigned i = 0;
           std::count(fracVec.begin(), fracVec.end(), 0) && i < precision;
           i++) {
        int carry = 0;

        for (int j = fracVec.size() - 1; j >= 0; j--) {
          int digit = fracVec[j] * radix + carry;

          if (digit > 9) {
            carry = digit / 10;
            digit %= 10;
          } else {
            carry = 0;
          }

          fracVec[j] = digit;
        }
        fs[i] = alphabet[carry];
      }
      result += '.' + fs;
    }
    return result;
  }

  void SetRadix(unsigned r) {
    if (r < 2 or r > 16) {
      throw invalid_radix(std::to_string(r));
    }
    radix = r;
  }
  void SetRadixAsStr(const std::string &rs) { SetRadix(parse_radix(rs)); }
  void SetPrecision(unsigned p) { precision = p; }
  void SetPrecisionAsStr(const std::string &ps) {
    precision = parse_precision(ps);
  }

private:
  double number;
  unsigned radix;
  unsigned precision;
  unsigned parse_radix(const std::string &rs) const {
    char *nend;
    unsigned r = strtoul(rs.c_str(), &nend, 10);
    if (*nend != '\0') {
      throw invalid_radix(rs);
    }
    return r;
  };
  unsigned parse_precision(const std::string &ps) const {
    char *nend;
    unsigned p = strtoul(ps.c_str(), &nend, 10);
    if (*nend != '\0') {
      throw invalid_precision(ps);
    }
    return p;
  }
  double parse_number(const std::string &ns, unsigned base) const {
    char *nend;
    double n = strtol(ns.c_str(), &nend, base);
    if (*nend == '.' && *(++nend) != '\0') {
      char *dot = nend;
      double fractional = strtol(nend, &nend, radix);
      if (*nend == '\0') {
        n += fractional / (double)pow(base, nend - dot);
      }
    }
    if (*nend != '\0') {
      throw invalid_pnumber(ns);
    }
    return n;
  }
};

#ifdef RUN_TESTS
#include "acutest.h"
void test_pnumber_constructor() {
  {
    PNumber p = PNumber(11.3, 16, 1);
    TEST_CHECK(p.GetNumber() == 11.3);
    TEST_CHECK(p.GetRadix() == 16);
    TEST_CHECK(p.GetPrecision() == 1);
  }

  {
    PNumber p = PNumber("11", "2", "3");
    if (not TEST_CHECK(p.GetNumber() == 3.0)) {
      TEST_MSG("GetNumber() == %.1lf", p.GetNumber());
    }
    TEST_CHECK(p.GetRadixAsStr() == "2");
    TEST_CHECK(p.GetPrecisionAsStr() == "3");
  }

  {
    PNumber p = PNumber("11.11", "2", "3");
    if (not TEST_CHECK(p.GetNumber() == 3.75)) {
      TEST_MSG("GetNumber() == %lf", p.GetNumber());
    }
  }
  {
    PNumber p = PNumber("100.", "2", "3");
    if (not TEST_CHECK(p.GetNumber() == 4.0)) {
      TEST_MSG("GetNumber() == %.1lf", p.GetNumber());
    }
  }
}

void test_pnumber_constructor_exception() {
  TEST_EXCEPTION(PNumber(".0", "1", "3"), invalid_radix);
  TEST_EXCEPTION(PNumber("1y1", "3", "3"), invalid_pnumber);
  TEST_EXCEPTION(PNumber("13", "3y", "3"), invalid_radix);
  TEST_EXCEPTION(PNumber("13", "3", "3y"), invalid_precision);
}
void test_pnumber_setters() {
  {
    PNumber p = PNumber("100.", "2", "3");
    p.SetRadix(16);
    TEST_CHECK(p.GetRadix() == 16);
    TEST_CHECK(p.GetRadixAsStr() == "16");

    p.SetRadixAsStr("8");
    TEST_CHECK(p.GetRadix() == 8);

    // exceptions
    TEST_EXCEPTION(p.SetRadix(1), invalid_radix);
    TEST_EXCEPTION(p.SetRadix(17), invalid_radix);
  }

  {
    PNumber p = PNumber("100.", "2", "3");
    p.SetPrecision(0);
    TEST_CHECK(p.GetPrecision() == 0);
    TEST_CHECK(p.GetPrecisionAsStr() == "0");
  }
}

void test_pnumber_to_string() {
  std::string s;
  {
    PNumber p = PNumber("FF.", "16", "3");
    p.SetRadix(15);
    s = p.ToString();
    TEST_CHECK_(s == "120.000", "ToString == '%s'", s.c_str());
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
  {
    PNumber p = PNumber("F.F", "16", "3");
    TEST_CHECK_(p.GetNumber() == 15.9375, "GetNumber == %.3f", p.GetNumber());
    s = p.ToString();
    TEST_CHECK_(s == "F.F00", "ToString == '%s'", s.c_str());
    p.SetRadix(15);
    s = p.ToString();
    TEST_CHECK_(s == "10.E0E", "ToString == '%s'", s.c_str());
    p.SetRadix(8);
    s = p.ToString();
    TEST_CHECK_(s == "17.740", "ToString == '%s'", s.c_str());
    p.SetRadix(2);
    s = p.ToString();
    TEST_CHECK_(s == "1111.111", "ToString == '%s'", s.c_str());
  }
}

TEST_LIST = {
    {"pnumber_constructor", test_pnumber_constructor},
    {"pnumber_constructor-exception", test_pnumber_constructor_exception},
    {"pnumber_setters", test_pnumber_setters},
    {"pnumber_to_string", test_pnumber_to_string},
    {NULL, NULL}};
#endif
