// gl1.cpp

#include "wx/wx.h"
#include "wx/glcanvas.h"
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <math.h>

// (c) Copyright 1993, 1994, Silicon Graphics, Inc.
// ALL RIGHTS RESERVED
// Permission to use, copy, modify, and distribute this software for
// any purpose and without fee is hereby granted, provided that the above
// copyright notice appear in all copies and that both the copyright notice
// and this permission notice appear in supporting documentation, and that
// the name of Silicon Graphics, Inc. not be used in advertising
// or publicity pertaining to distribution of the software without specific,
// written prior permission.
//
// THE MATERIAL EMBODIED ON THIS SOFTWARE IS PROVIDED TO YOU "AS-IS"
// AND WITHOUT WARRANTY OF ANY KIND, EXPRESS, IMPLIED OR OTHERWISE,
// INCLUDING WITHOUT LIMITATION, ANY WARRANTY OF MERCHANTABILITY OR
// FITNESS FOR A PARTICULAR PURPOSE.  IN NO EVENT SHALL SILICON
// GRAPHICS, INC.  BE LIABLE TO YOU OR ANYONE ELSE FOR ANY DIRECT,
// SPECIAL, INCIDENTAL, INDIRECT OR CONSEQUENTIAL DAMAGES OF ANY
// KIND, OR ANY DAMAGES WHATSOEVER, INCLUDING WITHOUT LIMITATION,
// LOSS OF PROFIT, LOSS OF USE, SAVINGS OR REVENUE, OR THE CLAIMS OF
// THIRD PARTIES, WHETHER OR NOT SILICON GRAPHICS, INC.  HAS BEEN
// ADVISED OF THE POSSIBILITY OF SUCH LOSS, HOWEVER CAUSED AND ON
// ANY THEORY OF LIABILITY, ARISING OUT OF OR IN CONNECTION WITH THE
// POSSESSION, USE OR PERFORMANCE OF THIS SOFTWARE.
//
// US Government Users Restricted Rights
// Use, duplication, or disclosure by the Government is subject to
// restrictions set forth in FAR 52.227.19(c)(2) or subparagraph
// (c)(1)(ii) of the Rights in Technical Data and Computer Software
// clause at DFARS 252.227-7013 and/or in similar or successor
// clauses in the FAR or the DOD or NASA FAR Supplement.
// Unpublished-- rights reserved under the copyright laws of the
// United States.  Contractor/manufacturer is Silicon Graphics,
// Inc., 2011 N.  Shoreline Blvd., Mountain View, CA 94039-7311.
//
// OpenGL(TM) is a trademark of Silicon Graphics, Inc.

//! Virtual Trackball.
//! Implemented by: Gavin Bell, lots if ideas from Thant Tessman and the August 1988 issue
//! of Siggraph's "Computer Graphics", pp. 121-129.
//! Origial code by: David M. Ciemiewicz, Mark Grossman, Henry Moreton, Paul Haeberli
//! Ported to C++: Mario Konrad
class Trackball
{
	private:
		static const float BallSize;
	private:
		//! Copies one vector (3x1) into another.
		static void vec_copy(float a[3], float v[3])
		{
			v[0] = a[0]; v[1] = a[1]; v[2] = a[2];
		}

		//! scales the specified vector to the length given through s.
		static void vec_scale(float v[3], float s)
		{
			v[0] *= s; v[1] *= s; v[2] *= s;
		}

		//! Initializes the vector to a null vector.
		static void vec_zero(float v[3])
		{
			v[0] = v[1] = v[2] = 0.0;
		}

		//! Sets the vector to the specified values.
		static void vec_set(float v[3], float x, float y, float z)
		{
			v[0] = x; v[1] = y; v[2] = z;
		}

		//! Project an (x,y) pair onto a sphere or radius r or a hyperbolic sheet
		//! if we are away from the center of the sphere.
		static float project_to_sphere(float r, float x, float y)
		{
			float d = sqrt(x*x + y*y);
			if (d < r * 0.70710678118654752440)  // 1.0/sqrt(2)
				return sqrt(r*r - d*d); // inside sphere
			// on hyperbola
			float t = r / 1.41421756237309504880; // sqrt(2)
			return t*t / d;
		}

		//! Calculates the cross product of two vectors  (a x b = c).
		static void vec_cross(float a[3], float b[3], float c[3])
		{
			float t[3];
			t[0] = a[1] * b[2] - a[2] * b[1];
			t[1] = a[2] * b[0] - a[0] * b[2];
			t[2] = a[0] * b[1] - a[1] * b[0];
			c[0] = t[0];
			c[1] = t[1];
			c[2] = t[2];
		}

		//! Calculates the dot product of two vectors  (a o b).
		static float vec_dot(float a[3], float b[3])
		{
			return a[0]*b[0] + a[1]*b[1] + a[2]*b[2];
		}

		//! Calculates the difference between two vectors (a - b = c)
		static void vec_sub(float a[3], float b[3], float c[3])
		{
			c[0] = a[0] - b[0];
			c[1] = a[1] - b[1];
			c[2] = a[2] - b[2];
		}

		//! Calculates the sum of two vectors (a + b = c)
		static void vec_add(float a[3], float b[3], float c[3])
		{
			c[0] = a[0] + b[0];
			c[1] = a[1] + b[1];
			c[2] = a[2] + b[2];
		}

		//! Calculates the length of the vector v.
		static float vec_length(float v[3])
		{
			return sqrt(v[0]*v[0] + v[1]*v[1] + v[2]*v[2]);
		}

		//! Normalizes the specified vector.
		static void vec_normalize(float v[3])
		{
			float l = vec_length(v);
			if (l < 1.0e-9) return;
			v[0] /= l;
			v[1] /= l;
			v[2] /= l;
		}

		//! Normalizes the quaternion.
		static void quat_normalize(float q[4])
		{
			float l = sqrt(q[0]*q[0] + q[1]*q[1] + q[2]*q[2] + q[3]*q[3]);
			if (l < 1.0e-9) return;
			q[0] /= l;
			q[1] /= l;
			q[2] /= l;
			q[3] /= l;
		}

		//! Defines the quaternion according the specified rotation axis
		//! and rotation angle.
		static void axis_to_quaternion(float a[3], float phi, float q[4])
		{
			vec_normalize(a);
			phi /= 2.0;
			float sphi = sin(phi);
			q[0] = a[0] * sphi;
			q[1] = a[1] * sphi;
			q[2] = a[2] * sphi;
			q[3] = cos(phi);
		}

	public:
		//! Calculates the rotation quaternion according the distance on the screen
		//! and the size of the trackball.
		static void calc_quat(float q[4], float p1x, float p1y, float p2x, float p2y)
		{
			if (p1x == p2x && p1y == p2y)
			{ // zero rotation
				vec_zero(q);
				q[3] = 1.0;
				return;
			}
			float p1[3];
			float p2[3];
			float rot_axis[3];
			float d[3];
			vec_set(p1, p1x, p1y, project_to_sphere(BallSize, p1x, p1y));
			vec_set(p2, p2x, p2y, project_to_sphere(BallSize, p2x, p2y));
			vec_cross(p2, p1, rot_axis);
			vec_sub(p1, p2, d);
			float t = vec_length(d) / (2.0 * BallSize);
			// avoid problems with out-of-control values:
			if (t >  1.0) t =  1.0;
			if (t < -1.0) t = -1.0;
			axis_to_quaternion(rot_axis, 2.0 * asin(t), q);
		}

		//! Calculates the roation matrix out of the specified quaternion.
		static void build_rot_matrix(float m[4][4], float q[4])
		{
			m[0][0] = 1.0 - 2.0 * (q[1] * q[1] + q[2] * q[2]);
			m[0][1] =       2.0 * (q[0] * q[1] - q[2] * q[3]);
			m[0][2] =       2.0 * (q[2] * q[0] + q[1] * q[3]);
			m[0][3] = 0.0;

			m[1][0] =       2.0 * (q[0] * q[1] + q[2] * q[3]);
			m[1][1] = 1.0 - 2.0 * (q[2] * q[2] + q[0] * q[0]);
			m[1][2] =       2.0 * (q[1] * q[2] - q[0] * q[3]);
			m[1][3] = 0.0;

			m[2][0] =       2.0 * (q[2] * q[0] - q[1] * q[3]);
			m[2][1] =       2.0 * (q[1] * q[2] + q[0] * q[3]);
			m[2][2] = 1.0 - 2.0 * (q[1] * q[1] + q[0] * q[0]);
			m[2][3] = 0.0;

			m[3][0] = 0.0;
			m[3][1] = 0.0;
			m[3][2] = 0.0;
			m[3][3] = 1.0;
		}

		//! Given two rotation, defined as quaternions, this function calculates
		//! the equivalent single rotation.
		static void add_rot_quats(float a[4], float b[4], float q[4])
		{
			float t1[4];
			float t2[4];
			float t3[4];
			float tf[4];

			vec_copy(a, t1); vec_scale(t1, b[3]);
			vec_copy(b, t2); vec_scale(t2, a[3]);
			
			vec_cross(b, a, t3);
			tf[0] = t1[0] + t2[0] + t3[0];
			tf[1] = t1[1] + t2[1] + t3[1];
			tf[2] = t1[2] + t2[2] + t3[2];
			tf[3] = a[3] * b[3] - (a[0]*b[0] + a[1]*b[1] + a[2]*b[2]);

			q[0] = tf[0];
			q[1] = tf[1];
			q[2] = tf[2];
			q[3] = tf[3];

			quat_normalize(q);
		}
};

const float Trackball::BallSize = 0.8;

//! Base class of all drawable objects.
class Object
{
	public:
		virtual void draw(void) = 0;
};

class WireTeapot : public Object
{
	public:
		virtual void draw(void)
		{
			glColor4f(0.3, 0.3, 0.3, 1.0);
			glutWireTeapot(3.0);
		}
};

class SolidTeapot : public Object
{
	public:
		virtual void draw(void)
		{
			glColor4f(0.5, 0.5, 0.5, 1.0);
			glutSolidTeapot(3.0);
		}
};

class WireSphere : public Object
{
	public:
		virtual void draw(void)
		{
			glColor4f(0.3, 0.3, 0.3, 1.0);
			glutWireSphere(3.0, 16, 16);
		}
};

class SolidSphere : public Object
{
	public:
		virtual void draw(void)
		{
			glColor4f(0.5, 0.5, 0.5, 1.0);
			glutSolidSphere(3.0, 16, 16);
		}
};

//! Position of light source no. 0
static const GLfloat light0_pos[4]     = { 0.0, 10.0, 10.0, 0.0 };

//! Diffuse light color of light source no. 0
static const GLfloat light0_diffuse[4] = { 0.8,  0.8,  0.9, 1.0 };

//! Canvas for OpenGL graphics
class GLCanvas : public wxGLCanvas
{
		friend class AppFrame;
	private:
		bool init;
		int mouse_x;
		int mouse_y;
		float rot_quat[4];

		bool enable_grid;
		bool enable_axis;
		Object * object;
	public:
		GLCanvas(wxWindow * parent,
			const wxWindowID id = -1,
			const wxPoint & pos = wxDefaultPosition,
			const wxSize & size = wxDefaultSize,
			long style = 0,
			const wxString & name = "GLCanvas");

		GLCanvas(wxWindow * parent,
			const GLCanvas & other,
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
		void OnMouse(wxMouseEvent & event);

		void Render(void);
		void InitGL(void);

		void draw_grid(void);
		void draw_axis(void);

		void set_object(Object *);
		void set_enable_grid(bool);
		void set_enable_axis(bool);

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
		void OnMenuShape(wxCommandEvent & event);
		void OnMenuView(wxCommandEvent & event);
	DECLARE_EVENT_TABLE()
};

//! Menu entries.
enum
{
	// Close application.
	ID_EXIT = 1,

	// Shape Menu
	ID_MENU_SHAPE = 100,
	ID_MENU_SHAPE_WIRE_TEAPOT,
	ID_MENU_SHAPE_SOLID_TEAPOT,
	ID_MENU_SHAPE_WIRE_SPHERE,
	ID_MENU_SHAPE_SOLID_SPHERE,

	// View Menu
	ID_MENU_VIEW = 200,
	ID_MENU_VIEW_GRID,
	ID_MENU_VIEW_AXIS,
};

// Event Tables
BEGIN_EVENT_TABLE(AppFrame, wxFrame)
	EVT_MENU(ID_EXIT, AppFrame::OnExit)
	EVT_MENU(ID_MENU_SHAPE_WIRE_TEAPOT, AppFrame::OnMenuShape)
	EVT_MENU(ID_MENU_SHAPE_SOLID_TEAPOT, AppFrame::OnMenuShape)
	EVT_MENU(ID_MENU_SHAPE_WIRE_SPHERE, AppFrame::OnMenuShape)
	EVT_MENU(ID_MENU_SHAPE_SOLID_SPHERE, AppFrame::OnMenuShape)
	EVT_MENU(ID_MENU_VIEW_GRID, AppFrame::OnMenuView)
	EVT_MENU(ID_MENU_VIEW_AXIS, AppFrame::OnMenuView)
END_EVENT_TABLE()

BEGIN_EVENT_TABLE(GLCanvas, wxGLCanvas)
	EVT_SIZE(GLCanvas::OnSize)
	EVT_PAINT(GLCanvas::OnPaint)
	EVT_ERASE_BACKGROUND(GLCanvas::OnEraseBackground)
	EVT_ENTER_WINDOW(GLCanvas::OnEnterWindow)
	EVT_MOUSE_EVENTS(GLCanvas::OnMouse)
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
	// canvas
	canvas = new GLCanvas(this, -1, wxDefaultPosition, wxDefaultSize);
	canvas->set_object(new WireTeapot);

	// menu: file
    wxMenu * menuFile = new wxMenu;
    menuFile->Append(ID_EXIT, "E&xit");

	// menu: view
	wxMenu * menuView = new wxMenu;
	menuView->AppendCheckItem(ID_MENU_VIEW_GRID, "Grid");
	menuView->AppendCheckItem(ID_MENU_VIEW_AXIS, "Axis");

	// menu: shape
	wxMenu * menuShape = new wxMenu;
	menuShape->AppendRadioItem(ID_MENU_SHAPE_WIRE_TEAPOT, "Wired Teapot");
	menuShape->AppendRadioItem(ID_MENU_SHAPE_SOLID_TEAPOT, "Solid Teapot");
	menuShape->AppendRadioItem(ID_MENU_SHAPE_WIRE_SPHERE, "Wired Sphere");
	menuShape->AppendRadioItem(ID_MENU_SHAPE_SOLID_SPHERE, "Solid Sphere");

	// menubar
    wxMenuBar * menuBar = new wxMenuBar;
    menuBar->Append(menuFile, "&File");
	menuBar->Append(menuView, "&View");
	menuBar->Append(menuShape, "&Shape");
    SetMenuBar(menuBar);

	// status bar
    CreateStatusBar();
	SetStatusText("Successfully started...");
}

//! Function called to exit the application.
void AppFrame::OnExit(wxCommandEvent & event)
{
	Destroy();
}

//! This method gets called whenever a selection in the menu "Shape" is
//! made.
void AppFrame::OnMenuShape(wxCommandEvent & event)
{
	switch (event.GetId())
	{
		case ID_MENU_SHAPE_WIRE_TEAPOT:  canvas->set_object(new WireTeapot);  break;
		case ID_MENU_SHAPE_SOLID_TEAPOT: canvas->set_object(new SolidTeapot); break;
		case ID_MENU_SHAPE_WIRE_SPHERE:  canvas->set_object(new WireSphere);  break;
		case ID_MENU_SHAPE_SOLID_SPHERE: canvas->set_object(new SolidSphere); break;
		default: canvas->set_object(0); break;
	}
	canvas->Refresh(true);
}

//! This menthod handles 'View' menu related operations.
void AppFrame::OnMenuView(wxCommandEvent & event)
{
	switch (event.GetId())
	{
		case ID_MENU_VIEW_GRID: canvas->set_enable_grid(event.IsChecked()); break;
		case ID_MENU_VIEW_AXIS: canvas->set_enable_axis(event.IsChecked()); break;
		default: break;
	}
	canvas->Refresh(true);
}

//! Constructor to initialize the canvas.
GLCanvas::GLCanvas(wxWindow * parent, const wxWindowID id,
	const wxPoint & pos, const wxSize & size,
	long style, const wxString & name)
	: wxGLCanvas(parent, (wxGLCanvas *)0, id, pos, size, style, name),
	  init(false), mouse_x(-1), mouse_y(-1),
	  enable_grid(false), enable_axis(false), object(0)
{
	Trackball::calc_quat(rot_quat, 0.0, 0.0, 0.0, 0.0);
}

//! Copy constructor.
GLCanvas::GLCanvas(wxWindow * parent, const GLCanvas & other,
	const wxWindowID id, const wxPoint & pos,
	const wxSize & size, long style,
	const wxString & name)
	: wxGLCanvas(parent, other.GetContext(), id, pos, size, style, name),
	  init(false), mouse_x(-1), mouse_y(-1),
	  enable_grid(false), enable_axis(false), object(0)
{
	Trackball::calc_quat(rot_quat, 0.0, 0.0, 0.0, 0.0);
}

//! Destructor
GLCanvas::~GLCanvas(void)
{}

//! Paint event. This method gets called every time the graphics
//! should be drawn.
void GLCanvas::OnPaint(wxPaintEvent & event)
{
	wxPaintDC dc(this);
#ifndef __WXMOTIF__
	if (!GetContext()) return;
#endif
	SetCurrent();
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
	wxGLCanvas::OnSize(event);

	// set GL viewport
	int width;
	int height;
	GetClientSize(&width, &height);
#ifndef __WXMOTIF__
	if (GetContext())
#endif
	{
		SetCurrent();
		glViewport(0, 0, (GLint)width, (GLint)height);
	}
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0, (double)width / (double)height, 0.1, 100.0);
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

//! This method handles all mouse events.
void GLCanvas::OnMouse(wxMouseEvent & event)
{
	if (event.Dragging())
	{
		wxSize sz(GetClientSize());
		float quat[4];
		Trackball::calc_quat(quat,
			(2.0 * mouse_x - sz.x) / sz.x,
			(sz.y - 2.0 * mouse_y) / sz.y,
			(2.0 * event.GetX() - sz.x) / sz.x,
			(sz.y - 2.0 * event.GetY()) / sz.y);
		Trackball::add_rot_quats(quat, rot_quat, rot_quat);
		Refresh(false); // prevent flashing
	}
	mouse_x = event.GetX();
	mouse_y = event.GetY();
}

//! Sets the object to draw.
void GLCanvas::set_object(Object * obj)
{
	if (object)
	{
		delete object;
		object = 0;
	}
	object = obj;
}

//! Sets the policy of grid drawing.
void GLCanvas::set_enable_grid(bool flag)
{
	enable_grid = flag;
}

//! Sets the policy of axis drawing.
void GLCanvas::set_enable_axis(bool flag)
{
	enable_axis = flag;
}

//! Draws the grid.
void GLCanvas::draw_grid(void)
{
	if (!enable_grid) return;
	glDisable(GL_LIGHTING);
	glColor4f(0.4, 0.4, 0.4, 1.0);
	glBegin(GL_LINES);
	for (int x = 0; x < 33; ++x)
	{
		glVertex3f(-8.0 + 0.5 * x, -8.0, 0.0);
		glVertex3f(-8.0 + 0.5 * x,  8.0, 0.0);
	}
	for (int y = 0; y < 33; ++y)
	{
		glVertex3f(-8.0, -8.0 + 0.5 * y, 0.0);
		glVertex3f( 8.0, -8.0 + 0.5 * y, 0.0);
	}
	glEnd();
}

//! Draws all three axis.
void GLCanvas::draw_axis(void)
{
	if (!enable_axis) return;
	glDisable(GL_LIGHTING);
	glColor4f(1.0, 1.0, 1.0, 1.0);
	glBegin(GL_LINES);
		glVertex3f(0.0, 0.0, 0.0); glVertex3f(10.0,  0.0,  0.0);
		glVertex3f(0.0, 0.0, 0.0); glVertex3f( 0.0, 10.0,  0.0);
		glVertex3f(0.0, 0.0, 0.0); glVertex3f( 0.0,  0.0, 10.0);
	glEnd();
}

//! This method draws the scene.
void GLCanvas::Render(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	glTranslatef(0.0, 0.0, -15.0); // eye position

	glEnable(GL_LIGHTING);
	glLightfv(GL_LIGHT0, GL_POSITION, light0_pos);

	// rotation of scene
	float m[4][4];
	Trackball::build_rot_matrix(m, rot_quat);
	glMultMatrixf(&(m[0][0]));

	// draw scene
	draw_grid();
	draw_axis();
	glEnable(GL_LIGHTING);
	if (object) object->draw();
}

//! Initialization of all OpenGL specific parameters.
void GLCanvas::InitGL(void)
{
    SetCurrent();
	glClearColor(0.8, 0.8, 0.8, 0.0);

	// flags
	glFrontFace(GL_CCW);
	glDisable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);

	// nice shading
	glShadeModel(GL_SMOOTH);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);

	// lights
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_diffuse);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHTING);
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
	glEnable(GL_COLOR_MATERIAL);
} 