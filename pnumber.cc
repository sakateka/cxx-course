#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <map>
#include <set>
#include <string>
#include <vector>

struct invalid_pnumber : public std::invalid_argument {
public:
  explicit invalid_pnumber(const std::string &message)
      : std::invalid_argument(message) {}
};
struct invalid_radix : public std::invalid_argument {
public:
  explicit invalid_radix(const std::string &message)
      : std::invalid_argument(message) {}
};
struct invalid_precision : public std::invalid_argument {
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
    number = parse_number(n);
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
  std::string ToString() const {
    // XXX TODO
    return "TODO";
  }
  unsigned GetRadix() const { return radix; }
  std::string GetRadixAsStr() const { return std::to_string(radix); }
  unsigned GetPrecision() const { return precision; }
  std::string GetPrecisionAsStr() const { return std::to_string(precision); }

  void SetRadix(unsigned r) {
    if (r < 2) {
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
  double parse_number(const std::string &ns) const {
    char *nend;
    double n = strtol(ns.c_str(), &nend, radix);
    if (*nend == '.' and nend > ns.c_str() and
        nend < (ns.c_str() + ns.size())) {
      double fractional = strtol(++nend, &nend, radix);
      if (*nend == '\0') {
        int flog = log2(fractional);
        while (flog-- > 0) {
          fractional /= 10;
        }
        n += fractional;
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
    if (not TEST_CHECK(p.GetNumber() == 3.3)) {
      TEST_MSG("GetNumber() == %.1lf", p.GetNumber());
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
  TEST_CATCH(PNumber(".0", "2", "3"), invalid_pnumber);
  TEST_CATCH(PNumber(".0", "1", "3"), invalid_radix);
  TEST_CATCH(PNumber("1y1", "3", "3"), invalid_pnumber);
  TEST_CATCH(PNumber("13", "3y", "3"), invalid_radix);
  TEST_CATCH(PNumber("13", "3", "3y"), invalid_precision);
}

TEST_LIST = {
    {"pnumber_constructor", test_pnumber_constructor},
    {"pnumber_constructor-exception", test_pnumber_constructor_exception},
    {NULL, NULL}};
#endif
