#ifndef TCTRL_CC
#define TCTRL_CC
#include "editor.cc"
#include "proc.cc"
#include "pmemory.cc"
#include "pnumber.cc"

namespace NCtrl {
    enum struct TCtrlState { CStart,
                             CEditing,
                             FunDone,
                             CValDone,
                             CExpDone,
                             COpChange,
                             CError };

    class TCtrl {
    private:
        NEditor::TEditor editor;
        NProc::TProc processor;
        TMemory memory;
        TCtrlState state;
        TPNumber number;
    };
}; // namespace NCtrl

#ifdef RUN_TESTS
// tests
#endif // #ifdef RUN_TESTS
#endif //#ifndef TCTRL_CC
