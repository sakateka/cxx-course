#include "acutest.h"
#include "pnumber.cc"
#include <sstream>
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
  {
    string s;
    TPNumber p = TPNumber("FF.", "16", "3");
    p.SetRadix(15);
    s = p.ToString();
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

TEST_LIST = {
    {"pnumber_constructor", test_pnumber_constructor},
    {"pnumber_constructor-exception", test_pnumber_constructor_exception},
    {"pnumber_setters", test_pnumber_setters},
    {"pnumber_to_string", test_pnumber_to_string},
    {"pnumber_operations", test_pnumber_operations},
    {NULL, NULL}};
