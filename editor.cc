#ifndef TEDITOR_CC
#define TEDITOR_CC
#include <string>
#include <stdexcept>

#include "const.cc"

namespace NEditor {

    using NConst::DOT;
    using NConst::RADIX_MAX;
    using NConst::ZERO;
    using namespace std;

    class invalid_digit : public std::invalid_argument {
    public:
        explicit invalid_digit(const std::string& message)
            : std::invalid_argument(message) {
        }
    };

    class TEditor {
    public:
        TEditor(){};

        bool IsZero() const {
            return str == "0" || str == "-0";
        }

        string AddSign() {
            if (str.size() > 0 and str[0] == '-') {
                str = str.substr(1);
            } else {
                str = "-" + str;
            }
            return str;
        }

        string AddSymbol(char symbol, int base = 16) {
            if (!NConst::IsValidChar(symbol, base)) {
                throw invalid_digit("Invalid p number " + to_string(symbol));
            }
            if (IsZero()) {
                str[str.size() - 1] = symbol; // overwrite leading zero i.e. -0 -> -A
            } else {
                str += symbol;
            }
            return str;
        }

        string AddDot() {
            if (str.find(DOT) != string::npos) {
                throw invalid_digit("Dot alredy present in: '" + str + "'");
            }
            str += DOT;
            return str;
        }

        string AddZero() {
            str += ZERO;
            return str;
        }

        string Backspace() {
            if (str.size() > 0) {
                str.resize(str.size() - 1);
            }
            return str;
        }

        string Clear() {
            str.clear();
            str.append(ZERO);
            return str;
        }

        string Get() const {
            return str;
        }

    private:
        string str = "";
    };
}; // namespace NEditor

#ifdef RUN_TESTS
#include "acutest.h"
void test_editor_operations() {
    using NEditor::TEditor;
    TEST_CASE("Constructor");
    {
        TEditor e;
        TEST_CHECK(e.Get() == "");
    }
    TEST_CASE("Clear");
    {
        TEditor e;
        TEST_CHECK(e.Get() == "");
        e.Clear();
        TEST_CHECK(e.Get() == "0");
        e.AddSign();
        TEST_CHECK(e.Get() == "-0");
        e.Clear();
        TEST_CHECK(e.Get() == "0");
        e.AddSymbol('A');
        TEST_CHECK(e.Get() == "A");
        e.Clear();
        TEST_CHECK(e.Get() == "0");
    }
    TEST_CASE("AddSign / Get");
    {
        TEditor e;
        TEST_CHECK(e.Get() == "");
        e.AddSign();
        TEST_CHECK(e.Get() == "-");
        e.AddSign();
        TEST_CHECK(e.Get() == "");
        e.AddZero();
        TEST_CHECK(e.Get() == "0");
        e.AddSign();
        TEST_CHECK(e.Get() == "-0");
    }
    TEST_CASE("AddZero / IsZero");
    {
        TEditor e;
        TEST_CHECK(!e.IsZero());
        e.AddSign();
        TEST_CHECK(!e.IsZero());
        e.AddZero();
        TEST_CHECK(e.IsZero());
        e.AddSymbol('A');
        TEST_CHECK(!e.IsZero());
        TEST_CHECK(e.Get() == "-A");
    }

    TEST_CASE("AddSymbol / AddDot");
    {
        TEditor e;
        TEST_EXCEPTION(e.AddSymbol('A', 10), NEditor::invalid_digit);
        e.AddSymbol('A');
        TEST_CHECK(e.Get() == "A");
        e.AddDot();
        TEST_CHECK(e.Get() == "A.");
        TEST_EXCEPTION(e.AddDot(), NEditor::invalid_digit);
        TEST_CHECK(e.Get() == "A.");
        e.AddSymbol('E');
        TEST_CHECK(e.Get() == "A.E");
    }
    TEST_CASE("Backspace");
    {
        TEditor e;
        e.AddSymbol('A');
        e.AddDot();
        e.AddSymbol('E');
        TEST_CHECK(e.Get() == "A.E");
        TEST_CHECK(e.Backspace() == "A.");
        TEST_CHECK(e.Get() == "A.");
        TEST_CHECK(e.Backspace() == "A");
        TEST_CHECK(e.Get() == "A");
        e.AddSign();
        TEST_CHECK(e.Backspace() == "-");
        TEST_CHECK(e.Get() == "-");
        TEST_CHECK(e.Backspace() == "");
        TEST_CHECK(e.Get() == "");
        TEST_CHECK(e.Backspace() == "");
        TEST_CHECK(e.Get() == "");
    }
}

// tests
#endif // #ifdef RUN_TESTS
#endif //#ifndef TEDITOR_CC
