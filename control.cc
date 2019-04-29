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

    enum struct TState { Editing,
                         Converted,
                         Error };

    class TCtrl {
    public:
        explicit TCtrl() {
        }

        void SetOutputPrecision(int p) {
            number.SetPrecision(p);
        }

        void SetOutputRadix(int radix) {
            number.SetRadix(radix);
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
            std::cout << "result s=" << s << std::endl;
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

    private:
        //TState state = TState::Editing;
        int radixIn = 10;

        TPNumber number;
        NEditor::TEditor editor;
        NHistory::THistory history;
        NMemory::TMemory memory;
        NProc::TProc proc;
    };
}; // namespace NCtrl

#ifdef RUN_TESTS
// tests
#endif // #ifdef RUN_TESTS
#endif //#ifndef TCTRL_CC
