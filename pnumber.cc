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
  PNumber(std::string n, std::string b, std::string c) {
    char *nend;
    radix = strtoul(b.c_str(), &nend, 10);
    if (*nend != '\0' or radix < 2) {
      throw invalid_radix(b);
    }
    precision = strtoul(c.c_str(), &nend, 10);
    if (*nend != '\0') {
      throw invalid_precision(c);
    }
    number = strtol(n.c_str(), &nend, radix);
    if (*nend == '.' and nend > n.c_str() and nend < (n.c_str() + n.size())) {
      double fractional = strtol(++nend, &nend, radix);
      if (*nend == '\0') {
        int flog = log2(fractional);
        while (flog-- > 0) {
          fractional /= 10;
        }
        number += fractional;
      }
    }
    if (*nend != '\0') {
      throw invalid_pnumber(n);
    }
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

  PNumber pow(const PNumber &rhs) const {
    return {number * rhs.number, radix, precision};
  }
  PNumber pow() const { return {number * number, radix, precision}; }
  double GetNumber() const { return number; }
  std::string ToString() const { return "todo"; }
  unsigned GetRadix() const { return radix; }
  std::string GetRadixAsStr() const { return std::to_string(radix); }
  unsigned GetPrecision() const { return precision; }
  std::string GetPrecisionAsStr() const { return std::to_string(precision); }

private:
  double number;
  unsigned radix;
  unsigned precision;
};

#ifdef RUN_TESTS
#include "acutest.h"
void test_pnumber_constructor() {
  TEST_CHECK(PNumber(1, 2, 3).GetNumber() == 1);
  TEST_CHECK(PNumber(1, 2, 3).GetRadix() == 2);
  TEST_CHECK(PNumber(1, 2, 3).GetPrecision() == 3);

  // TEST_CHECK_(PNumber(1, 2, 3).ToString() == "1", "to_string todo");
  TEST_CHECK(PNumber(1, 2, 3).GetRadixAsStr() == "2");
  TEST_CHECK(PNumber(1, 2, 3).GetPrecisionAsStr() == "3");

  {
    PNumber p = PNumber("11", "2", "3");
    TEST_CHECK_(p.GetNumber() == 3, "p.GetNumber == %.1lf", p.GetNumber());
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
