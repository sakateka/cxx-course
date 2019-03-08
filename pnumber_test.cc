#include "acutest.h"
#include "pnumber.cc"

void test_pnumber_constructor() {
  {
    TPNumber p = TPNumber(11.3, 16, 1);
    TEST_CHECK(p.GetNumber() == 11.3);
    TEST_CHECK(p.GetRadix() == 16);
    TEST_CHECK(p.GetPrecision() == 1);
  }

  {
    TPNumber p = TPNumber("11", "2", "3");
    if (not TEST_CHECK(p.GetNumber() == 3.0)) {
      TEST_MSG("GetNumber() == %.1lf", p.GetNumber());
    }
    TEST_CHECK(p.GetRadixAsStr() == "2");
    TEST_CHECK(p.GetPrecisionAsStr() == "3");
  }

  {
    TPNumber p = TPNumber("11.11", "2", "3");
    if (not TEST_CHECK(p.GetNumber() == 3.75)) {
      TEST_MSG("GetNumber() == %lf", p.GetNumber());
    }
  }
  {
    TPNumber p = TPNumber("100.", "2", "3");
    if (not TEST_CHECK(p.GetNumber() == 4.0)) {
      TEST_MSG("GetNumber() == %.1lf", p.GetNumber());
    }
  }
}

void test_pnumber_constructor_exception() {
  TEST_EXCEPTION(TPNumber(".0", "1", "3"), invalid_radix);
  TEST_EXCEPTION(TPNumber("1y1", "3", "3"), invalid_pnumber);
  TEST_EXCEPTION(TPNumber("13", "3y", "3"), invalid_radix);
  TEST_EXCEPTION(TPNumber("13", "3", "3y"), invalid_precision);
}
void test_pnumber_setters() {
  {
    TPNumber p = TPNumber("100.", "2", "3");
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
    TPNumber p = TPNumber("100.", "2", "3");
    p.SetPrecision(0);
    TEST_CHECK(p.GetPrecision() == 0);
    TEST_CHECK(p.GetPrecisionAsStr() == "0");
  }
}

void test_pnumber_to_string() {
  std::string s;
  {
    TPNumber p = TPNumber("FF.", "16", "3");
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
    TPNumber p = TPNumber("F.F", "16", "3");
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
