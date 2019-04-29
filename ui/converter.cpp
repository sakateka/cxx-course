///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Apr 25 2019)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "converter.h"

///////////////////////////////////////////////////////////////////////////

ConverterFrame::ConverterFrame( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );

	m_menubar1 = new wxMenuBar( 0 );
	main = new wxMenu();
	wxMenuItem* m_menuHistory;
	m_menuHistory = new wxMenuItem( main, wxID_History, wxString( wxT("History") ) , wxEmptyString, wxITEM_NORMAL );
	main->Append( m_menuHistory );

	wxMenuItem* m_menuExit;
	m_menuExit = new wxMenuItem( main, wxID_EXIT, wxString( wxT("Exit") ) , wxEmptyString, wxITEM_NORMAL );
	main->Append( m_menuExit );

	m_menubar1->Append( main, wxT("Main") );

	Help = new wxMenu();
	wxMenuItem* m_menuabout;
	m_menuabout = new wxMenuItem( Help, wxID_ABOUT, wxString( wxT("About") ) , wxEmptyString, wxITEM_NORMAL );
	Help->Append( m_menuabout );

	m_menubar1->Append( Help, wxT("Help") );

	this->SetMenuBar( m_menubar1 );

	wxBoxSizer* bSizer1;
	bSizer1 = new wxBoxSizer( wxVERTICAL );

	m_textCtrl1 = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_RIGHT );
	m_textCtrl1->SetToolTip( wxT("Source Number") );
	m_textCtrl1->SetHelpText( wxT("Set Srource number") );

	bSizer1->Add( m_textCtrl1, 0, wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL, 5 );

	wxGridSizer* gSizer10;
	gSizer10 = new wxGridSizer( 1, 3, 0, 0 );

	m_staticText1 = new wxStaticText( this, wxID_ANY, wxEmptyString, wxPoint( -1,-1 ), wxDefaultSize, wxALIGN_RIGHT|wxST_ELLIPSIZE_END );
	m_staticText1->Wrap( -1 );
	gSizer10->Add( m_staticText1, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT|wxALL|wxEXPAND, 0 );

	m_staticText5 = new wxStaticText( this, wxID_ANY, wxT("Source Radix"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText5->Wrap( -1 );
	gSizer10->Add( m_staticText5, 0, wxALL, 5 );

	m_staticText6 = new wxStaticText( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText6->Wrap( -1 );
	gSizer10->Add( m_staticText6, 0, wxALL, 5 );


	bSizer1->Add( gSizer10, 0, wxEXPAND, 2 );

	m_slider1 = new wxSlider( this, wxID_ANY, 10, 2, 16, wxDefaultPosition, wxDefaultSize, wxSL_AUTOTICKS|wxSL_HORIZONTAL|wxSL_LABELS|wxSL_SELRANGE|wxSL_VALUE_LABEL );
	m_slider1->SetToolTip( wxT("Source Radix") );

	bSizer1->Add( m_slider1, 0, wxALL|wxEXPAND, 2 );

	m_textCtrl11 = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_RIGHT );
	m_textCtrl11->SetToolTip( wxT("Source Number") );
	m_textCtrl11->SetHelpText( wxT("Set Srource number") );

	bSizer1->Add( m_textCtrl11, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxEXPAND, 5 );

	wxGridSizer* gSizer11;
	gSizer11 = new wxGridSizer( 2, 3, 0, 10 );

	m_staticText11 = new wxStaticText( this, wxID_ANY, wxEmptyString, wxPoint( 2,-1 ), wxDefaultSize, wxALIGN_RIGHT );
	m_staticText11->Wrap( -1 );
	gSizer11->Add( m_staticText11, 0, wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT, 0 );

	m_staticText7 = new wxStaticText( this, wxID_ANY, wxT("Precision"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText7->Wrap( -1 );
	gSizer11->Add( m_staticText7, 0, wxALL, 5 );

	wxString m_choice1Choices[] = { wxT("0"), wxT("1"), wxT("2"), wxT("3"), wxT("4"), wxT("5"), wxT("6"), wxT("7"), wxT("8"), wxT("9"), wxT("10"), wxT("11"), wxT("12"), wxT("13"), wxT("14"), wxT("15") };
	int m_choice1NChoices = sizeof( m_choice1Choices ) / sizeof( wxString );
	m_choice1 = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_choice1NChoices, m_choice1Choices, 0 );
	m_choice1->SetSelection( 3 );
	gSizer11->Add( m_choice1, 1, wxALIGN_RIGHT|wxALL, 0 );

	m_staticText4 = new wxStaticText( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT );
	m_staticText4->Wrap( -1 );
	gSizer11->Add( m_staticText4, 1, wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT|wxALL|wxEXPAND, 0 );

	m_staticText8 = new wxStaticText( this, wxID_ANY, wxT("Output Radix"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText8->Wrap( -1 );
	gSizer11->Add( m_staticText8, 0, wxALL, 5 );

	m_staticText9 = new wxStaticText( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText9->Wrap( -1 );
	gSizer11->Add( m_staticText9, 0, wxALL, 5 );


	bSizer1->Add( gSizer11, 0, wxEXPAND, 0 );

	m_slider11 = new wxSlider( this, wxID_ANY, 16, 2, 16, wxDefaultPosition, wxDefaultSize, wxSL_AUTOTICKS|wxSL_HORIZONTAL|wxSL_LABELS|wxSL_VALUE_LABEL );
	m_slider11->SetToolTip( wxT("Source Radix") );

	bSizer1->Add( m_slider11, 0, wxALL|wxEXPAND, 2 );

	wxGridSizer* gSizer8;
	gSizer8 = new wxGridSizer( 5, 4, 0, 0 );

	m_button0 = new wxButton( this, wxID_ANY, wxT("0"), wxDefaultPosition, wxDefaultSize, 0 );
	gSizer8->Add( m_button0, 0, wxALL, 5 );

	m_button1 = new wxButton( this, wxID_ANY, wxT("1"), wxDefaultPosition, wxDefaultSize, 0 );
	gSizer8->Add( m_button1, 0, wxALL, 5 );

	m_button2 = new wxButton( this, wxID_ANY, wxT("2"), wxDefaultPosition, wxDefaultSize, 0 );
	gSizer8->Add( m_button2, 0, wxALL, 5 );

	m_button3 = new wxButton( this, wxID_ANY, wxT("3"), wxDefaultPosition, wxDefaultSize, 0 );
	gSizer8->Add( m_button3, 0, wxALL, 5 );

	m_button4 = new wxButton( this, wxID_ANY, wxT("4"), wxDefaultPosition, wxDefaultSize, 0 );
	gSizer8->Add( m_button4, 0, wxALL, 5 );

	m_button5 = new wxButton( this, wxID_ANY, wxT("5"), wxDefaultPosition, wxDefaultSize, 0 );
	gSizer8->Add( m_button5, 0, wxALL, 5 );

	m_button6 = new wxButton( this, wxID_ANY, wxT("6"), wxDefaultPosition, wxDefaultSize, 0 );
	gSizer8->Add( m_button6, 0, wxALL, 5 );

	m_button7 = new wxButton( this, wxID_ANY, wxT("7"), wxDefaultPosition, wxDefaultSize, 0 );
	gSizer8->Add( m_button7, 0, wxALL, 5 );

	m_button8 = new wxButton( this, wxID_ANY, wxT("8"), wxDefaultPosition, wxDefaultSize, 0 );
	gSizer8->Add( m_button8, 0, wxALL, 5 );

	m_button9 = new wxButton( this, wxID_ANY, wxT("9"), wxDefaultPosition, wxDefaultSize, 0 );
	gSizer8->Add( m_button9, 0, wxALL, 5 );

	m_buttonA = new wxButton( this, wxID_ANY, wxT("A"), wxDefaultPosition, wxDefaultSize, 0 );
	gSizer8->Add( m_buttonA, 0, wxALL, 5 );

	m_buttonB = new wxButton( this, wxID_ANY, wxT("B"), wxDefaultPosition, wxDefaultSize, 0 );
	gSizer8->Add( m_buttonB, 0, wxALL, 5 );

	m_buttonC = new wxButton( this, wxID_ANY, wxT("C"), wxDefaultPosition, wxDefaultSize, 0 );
	gSizer8->Add( m_buttonC, 0, wxALL, 5 );

	m_buttonD = new wxButton( this, wxID_ANY, wxT("D"), wxDefaultPosition, wxDefaultSize, 0 );
	gSizer8->Add( m_buttonD, 0, wxALL, 5 );

	m_buttonE = new wxButton( this, wxID_ANY, wxT("E"), wxDefaultPosition, wxDefaultSize, 0 );
	gSizer8->Add( m_buttonE, 0, wxALL, 5 );

	m_buttonF = new wxButton( this, wxID_ANY, wxT("F"), wxDefaultPosition, wxDefaultSize, 0 );
	gSizer8->Add( m_buttonF, 0, wxALL, 5 );

	m_buttonDot = new wxButton( this, wxID_ANY, wxT("."), wxDefaultPosition, wxDefaultSize, 0 );
	gSizer8->Add( m_buttonDot, 0, wxALL, 5 );

	m_buttonBS = new wxButton( this, wxID_ANY, wxT("BS"), wxDefaultPosition, wxDefaultSize, 0 );
	gSizer8->Add( m_buttonBS, 0, wxALL, 5 );

	m_buttonCE = new wxButton( this, wxID_ANY, wxT("CE"), wxDefaultPosition, wxDefaultSize, 0 );
	gSizer8->Add( m_buttonCE, 0, wxALL, 5 );

	m_buttonDo = new wxButton( this, wxID_ANY, wxT("DO"), wxDefaultPosition, wxDefaultSize, 0 );
	gSizer8->Add( m_buttonDo, 0, wxALL, 5 );


	bSizer1->Add( gSizer8, 0, wxALIGN_CENTER|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL|wxEXPAND, 1 );


	this->SetSizer( bSizer1 );
	this->Layout();
	m_statusBar1 = this->CreateStatusBar( 1, wxSTB_SIZEGRIP, wxID_ANY );

	this->Centre( wxBOTH );
}

ConverterFrame::~ConverterFrame()
{
}

HistoryFrame::HistoryFrame( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxSize( 300,200 ), wxSize( 300,600 ) );

	wxBoxSizer* bSizer3;
	bSizer3 = new wxBoxSizer( wxVERTICAL );

	m_listBox2 = new wxListBox( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0, NULL, 0 );
	bSizer3->Add( m_listBox2, 1, wxALL|wxEXPAND, 5 );


	this->SetSizer( bSizer3 );
	this->Layout();
	bSizer3->Fit( this );

	this->Centre( wxBOTH );
}

HistoryFrame::~HistoryFrame()
{
}
