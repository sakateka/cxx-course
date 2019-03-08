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

class TPNumber {
public:
  TPNumber(double n, unsigned b, unsigned c) {
    number = n;
    radix = validate_radix(b);
    precision = c;
  }
  static TPNumber default_() { return TPNumber(0, 10, 0); };

  TPNumber(const std::string &n, const std::string &b, const std::string &c) {
    SetRadix(parse_radix(b));
    precision = parse_precision(c);
    number = parse_number(n, radix);
  }
  friend std::ostream &operator<<(std::ostream &out, const TPNumber &p) {
    out << p.ToString();
    return out;
  }
  TPNumber operator+(const TPNumber &rhs) const {
    return {number + rhs.number, radix, precision};
  }
  void operator+=(const TPNumber &rhs) { number += rhs.number; }
  TPNumber operator-(const TPNumber &rhs) const {
    return {number - rhs.number, radix, precision};
  }
  void operator-=(const TPNumber &rhs) { number -= rhs.number; }
  TPNumber operator*(const TPNumber &rhs) const {
    return {number * rhs.number, radix, precision};
  }
  void operator*=(const TPNumber &rhs) { number *= rhs.number; }
  TPNumber operator/(const TPNumber &rhs) const {
    return {number / rhs.number, radix, precision};
  }
  void operator/=(const TPNumber &rhs) { number /= rhs.number; }
  bool operator==(const TPNumber &rhs) const { return number == rhs.number; }
  bool operator!=(const TPNumber &rhs) const { return number != rhs.number; }
  TPNumber operator!() const { return {1.0 / number, radix, precision}; }

  TPNumber pow2() const { return {number * number, radix, precision}; }

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
    do {
      result += alphabet[n % radix];
      n /= radix;
    } while (n);
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

  void SetRadix(unsigned r) { radix = validate_radix(r); }
  void SetRadixAsStr(const std::string &rs) { SetRadix(parse_radix(rs)); }
  void SetPrecision(unsigned p) { precision = p; }
  void SetPrecisionAsStr(const std::string &ps) {
    precision = parse_precision(ps);
  }

  static unsigned validate_radix(unsigned r) {
    if (r < 2 or r > 16) {
      throw invalid_radix(std::to_string(r));
    }
    return r;
  }
  static unsigned parse_radix(const std::string &rs) {
    if (rs.size() > 0 && rs[0] != '-') {
      char *nend;
      unsigned r = strtoul(rs.c_str(), &nend, 10);
      if (*nend == '\0') {
        return r;
      }
    }
    // negative or not fully parsed
    throw invalid_radix(rs);
  };
  static unsigned parse_precision(const std::string &ps) {
    if (ps.size() > 0 && ps[0] != '-') {
      char *nend;
      unsigned p = strtoul(ps.c_str(), &nend, 10);
      if (*nend == '\0') {
        return p;
      }
    }
    // negative or not fully parsed
    throw invalid_precision(ps);
  }
  static double parse_number(const std::string &ns, unsigned base) {
    char *nend;
    double n = strtol(ns.c_str(), &nend, base);
    if (*nend == '.' && *(++nend) != '\0') {
      char *dot = nend;
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

private:
  double number;
  unsigned radix;
  unsigned precision;
};

#ifdef RUN_TESTS
#include "acutest.h"
using namespace std;

//
// Constructors
//
void test_pnumber_constructor() {
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
  TEST_CASE("ConstructorString mailformed string");
  TEST_EXCEPTION(TPNumber(0, 1, 3), invalid_radix);
  TEST_EXCEPTION(TPNumber(".0", "1", "3"), invalid_radix);
  TEST_EXCEPTION(TPNumber("1y1", "3", "3"), invalid_pnumber);
  TEST_EXCEPTION(TPNumber("13", "3y", "3"), invalid_radix);
  TEST_EXCEPTION(TPNumber("13", "3", "3y"), invalid_precision);
  TEST_CASE("ConstructorString missmatched radix");
  TEST_EXCEPTION(TPNumber("1F", "2", "3"), invalid_pnumber);
  TEST_CASE("ConstructorString negative radix and precision");
  TEST_EXCEPTION(TPNumber("1", "-2", "3"), invalid_radix);
  TEST_EXCEPTION(TPNumber("1", "2", "-3"), invalid_precision);
}

//
// Methods
//
void test_pnumber_setters() {
  TEST_CASE("Radix");
  {
    TPNumber p = TPNumber("100.", "2", "3");
    p.SetRadix(16);
    TEST_CHECK(p.GetRadix() == 16);
    TEST_CHECK(p.GetRadixAsStr() == "16");

    TEST_EXCEPTION(p.SetRadix(1), invalid_radix);
    TEST_EXCEPTION(p.SetRadix(17), invalid_radix);
    TEST_EXCEPTION(p.SetRadix(-1), invalid_radix);

    p.SetRadixAsStr("8");
    TEST_CHECK(p.GetRadix() == 8);
  }

  TEST_CASE("Precision");
  {
    TPNumber p = TPNumber("100.", "2", "3");
    p.SetPrecision(0);
    TEST_CHECK(p.GetPrecision() == 0);
    TEST_CHECK(p.GetPrecisionAsStr() == "0");
  }
}

void test_pnumber_to_string() {
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
  {
    string s;
    TPNumber p = TPNumber("F.F", "16", "3");
    TEST_CHECK_(p.GetNumber() == 15.9375, "GetNumber == %.3f", p.GetNumber());
    stringstream ss("");
    p.SetRadix(10);
    ss << p;
    TEST_CHECK(ss.str() == "15.938"); // NOTE precision 3

    p.SetRadix(16);
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

void test_pnumber_operations() {
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

  TEST_CASE("pow2");
  TEST_CHECK(TPNumber("-4", "10", "3").pow2().GetNumber() == 16);
}

#endif // #ifdef RUN_TESTS
#endif // #ifndef PNUMBER_CC
