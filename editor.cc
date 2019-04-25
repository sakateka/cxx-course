#ifndef TEDITOR_CC
#define TEDITOR_CC
#include <string>
#include <stdexcept>
#include "const.cc"

namespace NEditor {

    using NConst::ALPHABET;
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
            return str == "0";
        }

        string AddSign() {
            if (str.size() > 0 and str[0] == '-') {
                str = str.substr(1);
            } else {
                str = "-" + str;
            }
            return str;
        }

        string AddSymbol(int symbol) {
            if (symbol < 0 or symbol >= RADIX_MAX) {
                throw invalid_digit("Invalid p number " + to_string(symbol));
            }
            str += ALPHABET[symbol];
            return str;
        }

        string AddDot() {
            if (str.find(DOT) == string::npos) {
                throw invalid_digit("Dot alredy set: " + str);
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

        string Set(string s) {
            str = s;
            return str;
        }

    private:
        string str = "";
    };
}; // namespace NEditor

#ifdef RUN_TESTS
// tests
#endif // #ifdef RUN_TESTS
#endif //#ifndef TEDITOR_CC
