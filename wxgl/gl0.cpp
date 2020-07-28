// gl0.cpp

#include "wx/wx.h"
#include "wx/glcanvas.h"
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

wxGLAttributes GetGlAttributes() {
  wxGLAttributes attributes;
  attributes.PlatformDefaults().MinRGBA(8, 8, 8, 8).DoubleBuffer().Depth(24).Stencil(8).EndList();
  if (!wxGLCanvas::IsDisplaySupported(attributes)) {
    wxMessageBox("Visual attributes for OpenGL are not accepted.\nThe app will exit now.",
                 "Error with OpenGL", wxOK | wxICON_ERROR);
  }
  return attributes;
}

//! Canvas for OpenGL graphics
class GLCanvas : public wxGLCanvas
{
		friend class AppFrame;
	private:
		bool init;
	public:
		GLCanvas(wxWindow * parent,
			const wxWindowID id = -1,
			const wxPoint & pos = wxDefaultPosition,
			const wxSize & size = wxDefaultSize,
			long style = 0,
			const wxString & name = "GLCanvas");

		virtual ~GLCanvas(void);

		void OnPaint(wxPaintEvent & event);
		void OnSize(wxSizeEvent & event);
		void OnEraseBackground(wxEraseEvent & event);
		void OnEnterWindow(wxMouseEvent & event);

		void Render(void);
		void InitGL(void);

	DECLARE_EVENT_TABLE()
};

//! The application class itself.
class App : public wxApp
{
	public:
		virtual bool OnInit();
};

//! This applicaiton only needs one frame.
class AppFrame : public wxFrame
{
	private:
		GLCanvas * canvas;
	public:
		AppFrame(const wxString& title,
			const wxPoint& pos, const wxSize& size);
		void OnExit(wxCommandEvent & event);
	DECLARE_EVENT_TABLE()
};

//! Menu entries.
enum
{
	//! Close application.
	ID_Exit = 1
};

// Event Tables
BEGIN_EVENT_TABLE(AppFrame, wxFrame)
	EVT_MENU(ID_Exit, AppFrame::OnExit)
END_EVENT_TABLE()

BEGIN_EVENT_TABLE(GLCanvas, wxGLCanvas)
  EVT_SIZE(GLCanvas::OnSize)
  EVT_PAINT(GLCanvas::OnPaint)
  EVT_ERASE_BACKGROUND(GLCanvas::OnEraseBackground)
  EVT_ENTER_WINDOW(GLCanvas::OnEnterWindow)
END_EVENT_TABLE()

IMPLEMENT_APP(App)

//! Function initializes the application and creates
//! necessary objects.
bool App::OnInit()
{
	AppFrame * frame = new AppFrame("wxWindows OpenGL Demo",
		wxDefaultPosition, wxSize(600, 500));
	frame->Show(true);
	SetTopWindow(frame);
	return true;
}

//! Creates and initializes the main frame, containing the
//! canvas, the statusbar and the menu.
AppFrame::AppFrame(const wxString & title,
	const wxPoint & pos, const wxSize & size)
	: wxFrame((wxFrame *)0, -1, title, pos, size)
{
	// status bar
    CreateStatusBar();
	SetStatusText("Successfully started...");

	// canvas
	canvas = new GLCanvas(this, -1, wxDefaultPosition, wxDefaultSize);
}

//! Function called to exit the application.
void AppFrame::OnExit(wxCommandEvent & event)
{
	Destroy();
}

//! Constructor to initialize the canvas.
GLCanvas::GLCanvas(wxWindow * parent, const wxWindowID id,
	const wxPoint & pos, const wxSize & size,
	long style, const wxString & name)
	//: wxGLCanvas(parent, (wxGLCanvas *)0, id, pos, size, style, name),
	: wxGLCanvas(parent, id, GetGlAttributes(), pos, size, style, name),
	  init(false)
{}

//! Destructor
GLCanvas::~GLCanvas(void)
{}

//! Paint event. This method gets called every time the graphics
//! should be drawn.
void GLCanvas::OnPaint(wxPaintEvent & event)
{
	wxPaintDC dc(this);
// #ifndef __WXMOTIF__
// 	if (!GetContext()) return;
// #endif
//	SetCurrent();
	// init OpenGL once, but after SetCurrent
	if (!init)
	{
		InitGL();
		init = true;
	}
	Render();
	glFlush();
	SwapBuffers();
	event.Skip();
}

//! This method gets handles all resize events that may occur on
//! the canvas. It preserves the aspect ratio and defines the
//! projection matrix (GL_PROJECTION).
void GLCanvas::OnSize(wxSizeEvent & event)
{
	// this is also necessary to update the context on some platforms
	//wxGLCanvas::OnSize(event);

	// set GL viewport
	int width;
	int height;
	GetClientSize(&width, &height);
// #ifndef __WXMOTIF__
// 	if (GetContext())
// #endif
	{
		//SetCurrent();
		glViewport(0, 0, (GLint)width, (GLint)height);
	}
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	//gluPerspective(45.0, (double)width / (double)height, 0.1, 100.0);
	glMatrixMode(GL_MODELVIEW);
}

//! This method is responsible to clear the background. However
//! it does nothing but consume the event. This is to avoid flashing
//! effects.
void GLCanvas::OnEraseBackground(wxEraseEvent & event)
{
	event.Skip();
}

//! Focus handling. This event handler consumes the event.
void GLCanvas::OnEnterWindow(wxMouseEvent & event)
{
	SetFocus();
	event.Skip();
}

//! This method draws the scene.
void GLCanvas::Render(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	glTranslatef(0.0, 0.0, -15.0);
	glRotatef(30.0, 1.0, 0.0, 0.0);
	glColor4f(0.3, 0.3, 0.3, 1.0);
	//glutInit(0, 0);
	//glutWireTeapot(3.0);
}

//! Initialization of all OpenGL specific parameters.
void GLCanvas::InitGL(void)
{
    //SetCurrent();
	glClearColor(0.8, 0.8, 0.8, 0.0);
    glEnable(GL_DEPTH_TEST);
	glShadeModel(GL_SMOOTH);
}