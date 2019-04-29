///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Apr 25 2019)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#pragma once

#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
#include <wx/string.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/menu.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/textctrl.h>
#include <wx/stattext.h>
#include <wx/sizer.h>
#include <wx/slider.h>
#include <wx/choice.h>
#include <wx/button.h>
#include <wx/statusbr.h>
#include <wx/frame.h>
#include <wx/listbox.h>
#include <wx/dialog.h>

///////////////////////////////////////////////////////////////////////////

#define wxID_History 1000

///////////////////////////////////////////////////////////////////////////////
/// Class ConverterFrame
///////////////////////////////////////////////////////////////////////////////
class ConverterFrame : public wxFrame
{
	private:

	protected:
		wxMenuBar* m_menubar1;
		wxMenu* main;
		wxMenu* Help;
		wxTextCtrl* m_textCtrl1;
		wxStaticText* m_staticText1;
		wxStaticText* m_staticText5;
		wxStaticText* m_staticText6;
		wxSlider* m_slider1;
		wxTextCtrl* m_textCtrl11;
		wxStaticText* m_staticText11;
		wxStaticText* m_staticText7;
		wxChoice* m_choice1;
		wxStaticText* m_staticText4;
		wxStaticText* m_staticText8;
		wxStaticText* m_staticText9;
		wxSlider* m_slider11;
		wxButton* m_button0;
		wxButton* m_button1;
		wxButton* m_button2;
		wxButton* m_button3;
		wxButton* m_button4;
		wxButton* m_button5;
		wxButton* m_button6;
		wxButton* m_button7;
		wxButton* m_button8;
		wxButton* m_button9;
		wxButton* m_buttonA;
		wxButton* m_buttonB;
		wxButton* m_buttonC;
		wxButton* m_buttonD;
		wxButton* m_buttonE;
		wxButton* m_buttonF;
		wxButton* m_buttonDot;
		wxButton* m_buttonBS;
		wxButton* m_buttonCE;
		wxButton* m_buttonDo;
		wxStatusBar* m_statusBar1;

	public:

		ConverterFrame( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxEmptyString, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 370,620 ), long style = wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL );

		~ConverterFrame();

};

///////////////////////////////////////////////////////////////////////////////
/// Class HistoryFrame
///////////////////////////////////////////////////////////////////////////////
class HistoryFrame : public wxDialog
{
	private:

	protected:
		wxListBox* m_listBox2;

	public:

		HistoryFrame( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxEmptyString, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxDEFAULT_DIALOG_STYLE );
		~HistoryFrame();

};

