#pragma once

class TriangleWindow : public wxFrame
{
public:
	TriangleWindow(wxWindow* parent, const std::wstring& title, const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxDefaultSize);
	virtual ~TriangleWindow();
	TriangleWindow(const TriangleWindow& tw) = delete;
	TriangleWindow(TriangleWindow&& tw) = delete;
	TriangleWindow& operator=(const TriangleWindow& tw) = delete;
	TriangleWindow& operator=(TriangleWindow&&) = delete;
};