#ifndef UI_UI_CC
#define UI_UI_CC

#include <vector>
#include <iostream>
#include <wx/log.h>
#include <wx/msgdlg.h>
#include "wx/choicdlg.h"

#include "converter.h"
#include "../control.cc"
#include "../const.cc"

class Ui : public ConverterFrame {
public:
    Ui()
        : ConverterFrame(NULL) {
    }

    void Init() {
        Control.SetSourceRadix(m_sourceRadix->GetValue());
        Control.SetOutputRadix(m_outputRadix->GetValue());
        Control.SetOutputPrecision(m_outputPrecision->GetSelection());
        UpdateControls(Control.GetSourceRadix());
    }

    void OnHistory(wxCommandEvent& event) {
        wxLogDebug("Click Menu History");
        wxArrayString choices;
        for (auto& i : Control.GetHistory()) {
            choices.Add(i);
        }

        wxSingleChoiceDialog dialog(this,
                                    wxT("Conversion history"),
                                    wxT("Please select a value"),
                                    choices);

        if (dialog.ShowModal() == wxID_OK) {
            int idx = dialog.GetSelection();
            auto sel = Control.SetFromHistory(idx);
            wxLogDebug("Set item from history: %s", sel.ToString());

            m_sourceRadix->SetValue(sel.radix1);
            m_outputRadix->SetValue(sel.radix2);
            m_sourceNumber->ChangeValue(sel.number1);
            m_outputNumber->ChangeValue(sel.number2);
            UpdateControls(sel.radix1);
            OnSourceNumber(event);
        }
    }

    void OnAddToHistory(wxCommandEvent& event) {
        wxLogDebug("Click History");
        Control.AddToHistory();
    }

    void OnSourceNumber(wxCommandEvent& event) {
        std::string result = Control.Convert();
        wxLogDebug("Do conversion for: %s -> %s", m_sourceNumber->GetValue(), result);
        m_outputNumber->ChangeValue(result);
    }

    int SetPrecision() {
        int p = m_outputPrecision->GetSelection();
        Control.SetOutputPrecision(p);
        return p;
    }

    void OnPrecisionChoice(wxCommandEvent& event) {
        int p = SetPrecision();
        wxLogDebug("Set output precision: '%d'", p);
        OnSourceNumber(event);
    }

    void OnSign(wxCommandEvent& event) {
        Control.AddSign();
        m_sourceNumber->ChangeValue(Control.GetSourceNumberAsStr());
        OnSourceNumber(event);
    }

    void OnDot(wxCommandEvent& event) {
        try {
            Control.AddDot();
            m_sourceNumber->ChangeValue(Control.GetSourceNumberAsStr());
            SetPrecision();
        } catch (NCtrl::invalid_digit& e) {
            wxLogDebug("Failed to add dot: %s", e.what());
        }
    }

    void OnBackspace(wxCommandEvent& event) {
        wxLogDebug("Click on backspace button");
        Control.Backspace();
        m_sourceNumber->ChangeValue(Control.GetSourceNumberAsStr());
        OnSourceNumber(event);
    }

    void OnClear(wxCommandEvent& WXUNUSED(event)) {
        wxLogDebug("Click on clear button");
        Control.Clear();
        m_sourceNumber->ChangeValue("");
        m_outputNumber->ChangeValue("");
    }

    void OnSliderOutputRadix(wxCommandEvent& event) {
        int outputRadix = m_outputRadix->GetValue();
        wxLogDebug("Set output radix = %d", outputRadix);
        Control.SetOutputRadix(outputRadix);
        OnSourceNumber(event);
    }

    void OnSliderSourceRadix(wxCommandEvent& WXUNUSED(event)) {
        int sourceRadix = m_sourceRadix->GetValue();
        try {
            Control.SetSourceRadix(sourceRadix);
            // convert source number to valid and reset ui text
            m_sourceNumber->ChangeValue(Control.SetToSource());
        } catch (std::exception& e) {
            wxLogDebug("Failed to convert number: %s", e.what());
        }
        UpdateControls(sourceRadix);
    }

    void UpdateControls(int toRadix) {
        wxLogDebug("EnableButtons for sourceRadix = %d", toRadix);
        for (int idx = 0; idx < toRadix; idx++) {
            numbers[idx]->Enable();
        }
        for (int idx = toRadix; idx < numbers.size(); idx++) {
            numbers[idx]->Disable();
        }
    }

    void OnNumberClick(wxCommandEvent& event) {
        int id = event.GetId();
        wxButton* nb = (wxButton*)wxWindow::FindWindowById(id);
        char n = nb->GetLabel().GetChar(0);
        wxLogDebug("Click on %c", n);

        try {
            Control.AddDigit(n);
        } catch (NCtrl::invalid_digit& e) {
            wxLogDebug("Invalid char '%c': %s", n, e.what());
        } catch (std::exception& e) {
            wxLogDebug("Unexpected exception from AddDigit for '%c': %s", n, e.what());
        }
        m_sourceNumber->ChangeValue(Control.GetSourceNumberAsStr());
        OnSourceNumber(event);
    }

    void OnExit(wxCommandEvent& event) {
        Close(true);
    }
    void OnAbout(wxCommandEvent& event) {
        wxMessageBox("Radix Converter from 2 to 16 and back",
                     "About Converter", wxOK | wxICON_INFORMATION);
    }

    void OnSourceNumberTextUpdate(wxCommandEvent& event) {
        wxString inStr = m_sourceNumber->GetValue();
        wxLogDebug("Process string: '%s'", inStr);
        std::string tmp = inStr.ToStdString();
        bool ok = false;
        try {
            // try reset PNumber
            inStr = Control.ReSetNumber(tmp);
            ok = tmp != inStr;
        } catch (NCtrl::invalid_digit& e) {
            wxLogDebug("Invalid string: '%s'", inStr);
        } catch (std::exception& e) {
            wxLogDebug("Unknown exception while parse '%s': %s", inStr, e.what());
        }
        inStr = Control.GetSourceNumberAsStr();
        m_sourceNumber->ChangeValue(inStr);
        m_sourceNumber->SetInsertionPoint(inStr.Length());
        if (ok) {
            wxLogDebug("Trigger Convert for: %s", inStr);
            OnSourceNumber(event);
        }
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
