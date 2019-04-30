#ifndef TEDITOR_CC
#define TEDITOR_CC
#include <string>
#include <stdexcept>

#include "const.cc"

namespace NEditor {

    using NConst::DOT;
    using NConst::MINUS;
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

        bool IsEmpty() const {
            return str.empty() || str == "-";
        }

        string AddSign() {
            if (!str.empty() && str[0] == MINUS) {
                str = str.substr(1);
            } else {
                str = MINUS + str;
            }
            return str;
        }

        string AddDigit(char symbol, int base = 16) {
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

        bool HasDot() {
            return dotSeen;
        }

        string AddDot() {
            if (dotSeen) {
                throw invalid_digit("Dot alredy present in: '" + str + "'");
            }
            dotSeen = true;
            str += DOT;
            return str;
        }

        string AddZero() {
            str += ZERO;
            return str;
        }

        string Backspace() {
            size_t size = str.size();
            if (size > 0) {
                if (str[size - 1] == DOT) {
                    dotSeen = false;
                }
                str.resize(size - 1);
            }
            return str;
        }

        string Clear() {
            str.clear();
            str += ZERO;
            dotSeen = false;
            return str;
        }

        string Set(std::string in, int base) {
            TEditor new_;
            size_t inSize = in.size();
            size_t i = 0;
            if (inSize > 0 && in[i] == MINUS) {
                i++;
                new_.AddSign();
            }
            for (; i < in.size(); i++) {
                char c = in[i];
                switch (c) {
                    case DOT:
                        new_.AddDot();
                        break;
                    default:
                        new_.AddDigit(c, base);
                }
            }
            dotSeen = new_.dotSeen;
            str = new_.Get();
            return str;
        }

        string Get() const {
            return str;
        }

    private:
        bool dotSeen = false;
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
        e.AddDigit('A');
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
        e.AddDigit('A');
        TEST_CHECK(!e.IsZero());
        TEST_CHECK(e.Get() == "-A");
    }

    TEST_CASE("AddDigit / AddDot");
    {
        TEditor e;
        TEST_EXCEPTION(e.AddDigit('A', 10), NEditor::invalid_digit);
        e.AddDigit('A');
        TEST_CHECK(e.Get() == "A");
        e.AddDot();
        TEST_CHECK(e.Get() == "A.");
        TEST_EXCEPTION(e.AddDot(), NEditor::invalid_digit);
        TEST_CHECK(e.Get() == "A.");
        e.AddDigit('E');
        TEST_CHECK(e.Get() == "A.E");
    }
    TEST_CASE("AddSign;AddDot");
    {
        TEditor e;
        e.AddSign();
        TEST_CHECK(e.Get() == "-");
        e.AddDot();
        TEST_CHECK(e.Get() == "-.");
    }
    TEST_CASE("Backspace");
    {
        TEditor e;
        e.AddDigit('A');
        e.AddDot();
        e.AddDigit('E');
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
