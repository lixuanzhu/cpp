#include "client_window.h"

ClientWindow::ClientWindow(const wxPoint& point, const wxSize& size)
       : wxFrame(NULL, wxID_ANY, "", point, size, wxNO_BORDER)
{
  //Centre();
  CreateStatusBar();
  SetStatusText("Idle");
}
