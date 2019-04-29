#ifndef UI_UI_CC
#define UI_UI_CC

#include "converter.h"
#include "../control.cc"
#include <wx/log.h>
#include <wx/msgdlg.h>
#include <vector>
#include <iostream>

class Ui : public ConverterFrame {
public:
    Ui()
        : ConverterFrame(NULL) {
    }

    void OnSliderSourceRadix(wxCommandEvent& WXUNUSED(event)) {
        UpdateControls();
    }

    void UpdateControls() {
        int sourceRadix = m_sourceRadix->GetValue();
        Control.SetSourceRadix(sourceRadix);
        EnableButtons(sourceRadix);
    }

    void EnableButtons(int sourceRadix) {
        wxLogDebug("EnableButtons for sourceRadix = %d", Control.GetSourceRadix());
        for (int idx = 0; idx < sourceRadix; idx++) {
            numbers[idx]->Enable();
        }
        for (int idx = sourceRadix; idx < numbers.size(); idx++) {
            numbers[idx]->Disable();
        }
    }

    void OnNumberClick(wxCommandEvent& event) {
        int id = event.GetId();
        wxButton* n = (wxButton*)wxWindow::FindWindowById(id);
        wxLogDebug("Click on %s", n->GetLabel());
    }

    void OnExit(wxCommandEvent& event) {
        Close(true);
    }
    void OnAbout(wxCommandEvent& event) {
        wxMessageBox("This is a wxWidgets Hello World example",
                     "About Hello World", wxOK | wxICON_INFORMATION);
    }

private:
    NCtrl::TCtrl Control;
    std::vector<wxButton*> numbers = {
        m_button0,
        m_button1,
        m_button2,
        m_button3,
        m_button4,
        m_button5,
        m_button6,
        m_button7,
        m_button8,
        m_button9,
        m_buttonA,
        m_buttonB,
        m_buttonC,
        m_buttonD,
        m_buttonE,
        m_buttonF,
    };
};

#endif // #ifndef UI_UI_CC
