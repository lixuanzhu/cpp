#include "wx/wxprec.h"
#include "TriangleCanvas.h"
#include "TriangleWindow.h"

const int triCanvasID = 2000;			// TriangleCanvas widget ID

TriangleWindow::TriangleWindow(wxWindow* parent, const std::wstring& title, const wxPoint& pos,
	const wxSize& size)
	: wxFrame(parent, wxID_ANY, title, pos, size, wxMINIMIZE_BOX | wxCLOSE_BOX | 
		wxSYSTEM_MENU | wxCAPTION | wxCLIP_CHILDREN)
{
	TriangleCanvas* canvas = new TriangleCanvas(this, triCanvasID, nullptr, { 0, 0 },
	{ 800, 800 });
	Fit();
	Centre();
}

TriangleWindow::~TriangleWindow()
{
}