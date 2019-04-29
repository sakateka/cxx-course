#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif
#include <wx/log.h>
#include "ui/ui.cc"

class MyApp : public wxApp {
public:
    virtual bool OnInit();
};

wxIMPLEMENT_APP(MyApp);

bool MyApp::OnInit() {
    Ui* ui = new Ui();
    ui->UpdateControls();
    ui->Show(true);
    return true;
}
