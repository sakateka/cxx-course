///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Apr 25 2019)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#pragma once

#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
#include <wx/cshelp.h>
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
#include <wx/button.h>
#include <wx/sizer.h>
#include <wx/slider.h>
#include <wx/choice.h>
#include <wx/statusbr.h>
#include <wx/frame.h>

///////////////////////////////////////////////////////////////////////////

#define wxID_History 1000

///////////////////////////////////////////////////////////////////////////////
/// Class ConverterFrame
///////////////////////////////////////////////////////////////////////////////
class ConverterFrame : public wxFrame
{
	private:

	protected:
		wxMenuBar* m_mainMenuBar;
		wxMenu* m_main;
		wxMenu* m_help;
		wxTextCtrl* m_sourceNumber;
		wxStaticText* m_staticText1;
		wxStaticText* m_staticText5;
		wxButton* m_buttonToHistory;
		wxSlider* m_sourceRadix;
		wxTextCtrl* m_outputNumber;
		wxStaticText* m_staticText11;
		wxStaticText* m_staticText7;
		wxChoice* m_outputPrecision;
		wxStaticText* m_staticText4;
		wxStaticText* m_staticText8;
		wxStaticText* m_staticText9;
		wxSlider* m_outputRadix;
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
		wxButton* m_buttonSign;
		wxStatusBar* m_statusBar1;

		// Virtual event handlers, overide them in your derived class
		virtual void OnHistory( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnExit( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnAbout( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnSourceNumberTextUpdate( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnSourceNumber( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnAddToHistory( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnSliderSourceRadix( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnPrecisionChoice( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnSliderOutputRadix( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnNumberClick( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnDot( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnBackspace( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnClear( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnSign( wxCommandEvent& event ) { event.Skip(); }


	public:

		ConverterFrame( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxEmptyString, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 370,620 ), long style = wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL );

		~ConverterFrame();

};

