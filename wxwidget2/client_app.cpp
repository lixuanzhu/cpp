
#include <wx/wx.h>
#include "client_window.h"

class ClientApp : public wxApp
{
  public:
    virtual bool OnInit();
};

bool ClientApp::OnInit()
{
    ClientWindow *clientWindow = new ClientWindow(wxPoint(200, 200), wxSize(300, 300));
    clientWindow->Show(true);

    return true;
}

IMPLEMENT_APP(ClientApp)