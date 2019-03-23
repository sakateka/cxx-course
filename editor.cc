#ifndef TEDITOR_CC
#define TEDITOR_CC
#include <string>
#include <stdexcept>

namespace NEditor {

    using namespace std;
    const string ZERO = "0";
    const string DOT = ".";

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
            static const char alphabet[] = "0123456789ABCDEF";
            if (symbol < 0 or symbol > 15) {
                throw invalid_digit("Invalid p number " + to_string(symbol));
            }
            str += alphabet[symbol];
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

        string ToString() const {
            return str;
        }

        string SetString(string s) {
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
