#ifndef TCTRL_CC
#define TCTRL_CC
#include "editor.cc"
#include "proc.cc"
#include "pmemory.cc"
#include "pnumber.cc"
#include "history.cc"

namespace NCtrl {

    enum struct TState { Editing,
                         Converted,
                         Error };

    class TCtrl {
    public:
        explicit TCtrl() {
        }

        void DoCmd(int cmd) {
        }

    private:
        TState state = TState::Editing;
        int radixIn = 10;
        int radixOut = 10;

        NHistory::THistory history;
        NPNumber::TPNumber number;
        NMemory::TMemory memory;
        NEditor::TEditor editor;
        NProc::TProc proc;
    };
}; // namespace NCtrl

#ifdef RUN_TESTS
// tests
#endif // #ifdef RUN_TESTS
#endif //#ifndef TCTRL_CC
