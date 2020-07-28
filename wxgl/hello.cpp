// hello.cpp

#include "wx/wx.h"

class HelloWorldApp: public wxApp
{
	public:
		virtual bool OnInit();
};

class HelloWorldFrame: public wxFrame
{
	public:
		HelloWorldFrame(const wxString& title,
			const wxPoint& pos, const wxSize& size);
//		void OnQuit(wxCommandEvent& event);
//		void OnAbout(wxCommandEvent& event);
//	DECLARE_EVENT_TABLE()
};

// enum
// {
// 	ID_Quit = 1,
// 	ID_About,
// };

// BEGIN_EVENT_TABLE(HelloWorldFrame, wxFrame)
// 	EVT_MENU(ID_Quit, HelloWorldFrame::OnQuit)
// 	EVT_MENU(ID_About, HelloWorldFrame::OnAbout)
// END_EVENT_TABLE()

IMPLEMENT_APP(HelloWorldApp)

//! Iniitalizes the applicaiton
bool HelloWorldApp::OnInit()
{
	HelloWorldFrame * frame = new HelloWorldFrame("Hello World",
		wxPoint(50, 50), wxSize(450, 340));
	frame->Show(true);
	SetTopWindow(frame);
	return true;
}

//! Build the frame and its content. Menu and Statusbar are
//! defined as well.
HelloWorldFrame::HelloWorldFrame(const wxString & title,
	const wxPoint & pos, const wxSize & size)
	: wxFrame((wxFrame *)NULL, -1, title, pos, size)
{
	CreateStatusBar();
	SetStatusText("Welcome to wxWindows!");
}

// //! Exits the application (closes the window).
// void HelloWorldFrame::OnQuit(wxCommandEvent & WXUNUSED(event))
// {
// 	Close(true);
// }

// //! Shows the about dialog.
// void HelloWorldFrame::OnAbout(wxCommandEvent & WXUNUSED(event))
// {
// 	wxMessageBox("This is a wxWindows Hello world sample",
// 		"About Hello World", wxOK | wxICON_INFORMATION, this);
// }