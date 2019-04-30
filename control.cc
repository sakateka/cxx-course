#ifndef TCTRL_CC
#define TCTRL_CC
#include "editor.cc"
#include "proc.cc"
#include "pmemory.cc"
#include "pnumber.cc"
#include "history.cc"
#include "const.cc"

namespace NCtrl {
    using NEditor::invalid_digit;
    using NPNumber::TPNumber;

    class TCtrl {
    public:
        explicit TCtrl() {
        }

        void SetOutputPrecision(int p) {
            number.SetPrecision(p);
        }
        int GetOutputPrecision() {
            return number.GetPrecision();
        }

        void SetOutputRadix(int radix) {
            number.SetRadix(radix);
        }
        int GetOutputRadix() {
            return number.GetRadix();
        }

        void SetSourceRadix(int radix) {
            radixIn = radix;
        }
        int GetSourceRadix() {
            return radixIn;
        }
        std::string GetSourceNumberAsStr() {
            return editor.Get();
        }

        void Clear() {
            editor.Clear();
            number.SetNumber(0);
        }

        void Backspace() {
            editor.Backspace();
            ReSetNumber(editor.Get());
        }

        void AddSign() {
            editor.AddSign();
            number.SetNumber(-number.GetNumber());
        }

        void AddDot() {
            editor.AddDot();
        }

        void AddDigit(char n) {
            editor.AddDigit(n, radixIn);
            ReSetNumber(editor.Get());
        }

        std::string ReSetNumber(std::string n) {
            int radixOut = number.GetRadix();
            n = editor.Set(n, radixIn);
            number.SetRadix(radixIn);
            number.SetNumberAsStr(n);
            number.SetRadix(radixOut);
            return n;
        }

        std::string SetToSource() {
            TPNumber n(number);
            n.SetPrecision(15);
            n.SetDoCarry();
            n.SetRadix(radixIn);
            std::string s = n.ToString();
            size_t end = s.size();
            size_t dot = s.find(NConst::DOT);
            if (dot != std::string::npos) {
                if (editor.HasDot()) {
                    dot += 1 + s.substr(dot).find_last_not_of(NConst::ZERO);
                }
                end = dot;
            }
            s = s.substr(0, end);
            editor.Set(s, radixIn);
            return editor.Get();
        }

        std::string Convert() {
            return number.ToString();
        }

        void AddToHistory() {
            history.AddRecord(radixIn, number.GetRadix(), editor.Get(), number.ToString());
        }

        NHistory::Record SetFromHistory(int idx) {
            auto r = history[idx];
            SetSourceRadix(r.radix1);
            SetOutputRadix(r.radix2);
            ReSetNumber(r.number1);
            return r;
        }

        std::vector<std::string> GetHistory() {
            return history.Get();
        }

    private:
        int radixIn = 10;

        TPNumber number;
        NEditor::TEditor editor;
        NHistory::THistory history;
    };
}; // namespace NCtrl

#ifdef RUN_TESTS
#include "acutest.h"
void test_control_operations() {
    using namespace std;
    TEST_CASE("SetOutputPrecision");
    {
        NCtrl::TCtrl c;
        int expect = 13;
        TEST_CHECK(c.GetOutputPrecision() != expect); // default 3 or 0
        c.SetOutputPrecision(expect);
        TEST_CHECK(c.GetOutputPrecision() == expect);
    }
    TEST_CASE("SetOutputRadix");
    {
        NCtrl::TCtrl c;
        int expect = 14;
        TEST_CHECK(c.GetOutputRadix() != expect); // default 10
        c.SetOutputRadix(expect);
        TEST_CHECK(c.GetOutputRadix() == expect);
    }
    TEST_CASE("Set Source Radix");
    {
        NCtrl::TCtrl c;
        int expect = 2;
        TEST_CHECK(c.GetSourceRadix() != expect); // default 10
        c.SetSourceRadix(expect);
        TEST_CHECK(c.GetSourceRadix() == expect);
    }
    TEST_CASE("GetSourceNumberAsStr");
    {
        NCtrl::TCtrl c;
        string expect = "";
        TEST_CHECK(c.GetSourceNumberAsStr() == expect);
        expect = "F.2";
        c.SetSourceRadix(16);
        c.AddDigit('F');
        c.AddDot();
        c.AddDigit('2');
        TEST_CHECK(c.GetSourceNumberAsStr() == expect);
        TEST_CASE("Clear");
        c.Clear();
        TEST_CHECK(c.GetSourceNumberAsStr() == "0");
        TEST_CASE("Backspace");
        c.Backspace();
        TEST_CHECK(c.GetSourceNumberAsStr() == "");
        c.AddDigit('F');
        c.AddDot();
        c.AddDigit('2');
        TEST_CHECK(c.GetSourceNumberAsStr() == expect);
        c.Backspace();
        c.Backspace();
        TEST_CHECK(c.GetSourceNumberAsStr() == "F");
    }
    TEST_CASE("AddDigit / AddDot");
    {
        NCtrl::TCtrl c;
        c.SetSourceRadix(NConst::RADIX_MIN);
        for (size_t i = NConst::RADIX_MIN; i < NConst::RADIX_MAX; i++) {
            char outside_radix = NConst::ALPHABET[i];
            TEST_EXCEPTION(c.AddDigit(outside_radix), NEditor::invalid_digit);
        }
        string expect = "."; // dot to preserve leading zero
        c.AddDot();
        c.SetSourceRadix(NConst::RADIX_MAX);
        for (size_t i = 0; i < NConst::RADIX_MAX; i++) {
            char valid_digit = NConst::ALPHABET[i];
            expect += valid_digit;
            c.AddDigit(valid_digit);
            TEST_CHECK(c.GetSourceNumberAsStr() == expect && expect.size() == i + 2);
        }
        TEST_EXCEPTION(c.AddDot(), NEditor::invalid_digit);
    }
    /*
        string ReSetNumber(string n) {
            int radixOut = number.GetRadix();
            n = editor.Set(n, radixIn);
            number.SetRadix(radixIn);
            number.SetNumberAsStr(n);
            number.SetRadix(radixOut);
            return n;
        }
        */
    TEST_CASE("ReSetNumber");
    {
        NCtrl::TCtrl c;
        c.SetSourceRadix(10);
        c.SetOutputRadix(10);
        c.SetOutputPrecision(3);

        string g = c.ReSetNumber("-10.981");
        string e = c.Convert();
        c.SetSourceRadix(16);

        c.ReSetNumber("-DEF.FED");
        c.SetOutputPrecision(4);
        g = c.Convert();
        e = "-3567.9953";
        TEST_CHECK_(g == e, "%s == %s", g.c_str(), e.c_str());
    }
    TEST_CASE("SetToSource");
    {
        NCtrl::TCtrl c;
        c.SetSourceRadix(10);
        c.SetOutputRadix(16);
        c.SetOutputPrecision(3);
        string in = "15.9375";
        c.ReSetNumber(in);
        string g = c.Convert();
        string e = "F.F00";
        string s = c.GetSourceNumberAsStr();
        TEST_CHECK_(g == e, "%s == %s", g.c_str(), e.c_str());
        TEST_CHECK_(s == in, "%s == %s", s.c_str(), in.c_str());

        c.SetSourceRadix(12);
        g = c.SetToSource();
        e = "13.B3";
        TEST_CHECK_(g == e, "%s == %s", g.c_str(), e.c_str());
        g = c.Convert();
        e = "F.F00";
        TEST_CHECK_(g == e, "%s == %s", g.c_str(), e.c_str());
        c.SetSourceRadix(12);
        TEST_EXCEPTION(c.ReSetNumber("F.F"), NEditor::invalid_digit);
    }
    TEST_CASE("SetToSource && History ");
    {
        NCtrl::TCtrl c;
        c.SetSourceRadix(10);
        c.SetOutputRadix(16);
        c.SetOutputPrecision(1);
        string in = "15.9375";
        c.ReSetNumber(in);
        c.AddToHistory();

        TEST_CHECK_(c.Convert() == "F.F", "%s == F.F", c.Convert().c_str());
        c.SetOutputPrecision(3);
        c.SetOutputRadix(15);
        c.AddToHistory();
        string g = c.Convert();
        string e = "10.E0E";
        TEST_CHECK_(g == e, "%s == %s", g.c_str(), e.c_str());

        TEST_CHECK(c.GetHistory().size() == 2);
        NHistory::Record r1 = c.SetFromHistory(0);
        NHistory::Record r2 = c.SetFromHistory(1);

        // r 1
        e = "{{15.9375, 10} => {F.F, 16}}";
        TEST_CHECK_(r1.ToString() == e, "%s == %s", r1.ToString().c_str(), e.c_str());
        e = "{{15.9375, 10} => {10.E0E, 15}}";
        TEST_CHECK_(r2.ToString() == e, "%s == %s", r2.ToString().c_str(), e.c_str());
    }
}
#endif // #ifdef RUN_TESTS
#endif //#ifndef TCTRL_CC
