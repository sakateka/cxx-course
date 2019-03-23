#ifndef PMEMORY_CC
#define PMEMORY_CC
#define TEST_NO_MAIN

#include "pnumber.cc"
#include <algorithm>
#include <iostream>
#include <map>
#include <set>
#include <string>
#include <vector>

namespace NMemory {
    using TPNumber = NPNumber::TPNumber;

    class TMemory {
    public:
        TMemory(int radix, int precision) {
            number = TPNumber(0, radix, precision);
        }

        void Store(const TPNumber p) {
            number = p;
            state = true;
        }
        TPNumber Get() {
            return number;
        }
        std::string GetAsStr() const {
            return number.ToString();
        }

        void Add(const TPNumber& p) {
            number += p;
            state = true;
        }
        void Clear(int radix, int precision) {
            number = TPNumber(0, radix, precision);
            state = false;
        }
        bool GetState() {
            return state;
        }
        std::string GetStateAsStr() {
            return state ? "_On" : "_Off";
        }

    private:
        TPNumber number = TPNumber::default_();
        bool state = false;
    };
}; // namespace NMemory

#ifdef RUN_TESTS
#include "acutest.h"
using namespace std;

void test_pmemory_constructor() {
    using namespace NMemory;

    TMemory m = TMemory(2, 0);
    TEST_CHECK(m.Get().GetNumber() == 0);
    TEST_CHECK(m.GetStateAsStr() == "_Off");
    TEST_EXCEPTION(TMemory(0, 0), NPNumber::invalid_radix);
}

void test_pmemory_operations() {
    using namespace NMemory;
    using TPNumber = NPNumber::TPNumber;

    TEST_CASE("Store");
    {
        TMemory m = TMemory(2, 0);
        m.Store(TPNumber("F", "16", "0"));
        TEST_CHECK(m.GetStateAsStr() == "_On");
        string ns = m.Get().ToString();
        TEST_CHECK_(ns == "F", "'F' == %s", ns.c_str());
        m.Store(TPNumber(10, 11, 3));
        TEST_CHECK(m.Get().ToString() == "A.000");
    }

    TEST_CASE("Get");
    {
        TMemory m = TMemory(2, 0);

        m.Store(TPNumber(10, 11, 3));
        TPNumber got = m.Get();
        TEST_CHECK(got.GetNumber() == m.Get().GetNumber() and
                   got.GetRadix() == m.Get().GetRadix() and
                   got.GetPrecision() == m.Get().GetPrecision());
        TEST_CASE("Get return Copy");
        got.SetPrecision(0);
        TEST_CHECK(got.GetPrecision() != m.Get().GetPrecision());
    }

    TEST_CASE("Get not modify State");
    {
        TMemory m = TMemory(2, 1);
        TEST_CHECK(m.GetState() == false);
        TEST_CHECK_(m.Get().ToString() == "0.0", "'%s'",
                    m.Get().ToString().c_str());
        TEST_CHECK(m.GetState() == false);
    }
    TEST_CASE("Add");
    {
        TMemory m = TMemory(2, 0);
        TEST_CHECK(m.GetState() == false);
        m.Add(TPNumber(10, 2, 0));
        TEST_CHECK(m.GetState() == true);
        TEST_CHECK(m.Get().GetNumber() == 10);
        m.Add(TPNumber(10, 2, 0));
        TEST_CHECK(m.Get().GetNumber() == 20 && m.Get().GetRadix() == 2);

        // NOTE
        TEST_CASE("Clear");
        TEST_CHECK(m.GetState() == true);
        m.Clear(3, 3);
        TEST_CHECK(m.GetState() == false);
        TEST_CHECK(m.Get().GetNumber() == 0 and m.Get().GetRadix() == 3 and
                   m.Get().GetPrecision() == 3);
    }

    TEST_CASE("GetStateAsStr");
    {
        TMemory m = TMemory(2, 1);
        TEST_CHECK(m.GetStateAsStr() == "_Off");
        m.Add(TPNumber(1, 2, 0));
        TEST_CHECK(m.GetStateAsStr() == "_On");
    }

    TEST_CASE("GetAsStr");
    {
        TMemory m = TMemory(2, 4);
        TEST_CHECK(m.GetAsStr() == "0.0000");
        TEST_CHECK(m.GetStateAsStr() == "_Off");
        m.Add(TPNumber(2, 2, 0));
        if (not TEST_CHECK(m.GetAsStr() == "10.0000")) {
            TEST_MSG("m.GetAsStr() == %s", m.GetAsStr().c_str());
        }
    }
}

#endif // #ifdef RUN_TESTS
#endif // #ifndef PMEMORY_CC
