#pragma once 
#include <memory> 
#include "wx/glcanvas.h" 
class TriangleCanvas : public wxGLCanvas 
{
    public:
        TriangleCanvas(wxWindow* parent, wxWindowID id = wxID_ANY, 
            const int* attribList = 0,
            const wxPoint& pos = wxDefaultPosition, 
            const wxSize& size = wxDefaultSize, long style = 0L, 
            const wxString& name = L"GLCanvas", 
            const wxPalette& palette = wxNullPalette);
        virtual ~TriangleCanvas();
        TriangleCanvas(const TriangleCanvas& tc) = delete;
        TriangleCanvas(TriangleCanvas&& tc) = delete;
        TriangleCanvas& operator=(const TriangleCanvas& tc) = delete; 
        TriangleCanvas& operator=(TriangleCanvas&& tc) = delete; 
    private:
            void InitializeGLEW();
            void SetupGraphics();
            void OnPaint(wxPaintEvent& event);

            std::unique_ptr<wxGLContext> m_context;
            GLuint m_vbo; // vertex buffer pointer
            GLuint m_vao; // vertex array pointer 
};

TriangleCanvas::TriangleCanvas(wxWindow* parent, wxWindowID id, 
        const int* attribList, const wxPoint& pos, const wxSize& size,
        long style, const wxString& name, const wxPalette& palette)
	: wxGLCanvas(parent, id, attribList, pos, size, style, name, palette),
	m_vbo(0), m_vao(0)
{
	m_context = std::make_unique<wxGLContext>(this);
	Bind(wxEVT_PAINT, &TriangleCanvas::OnPaint, this);
	
	SetCurrent(*m_context);
	InitializeGLEW();
	SetupGraphics();
}

void TriangleCanvas::InitializeGLEW()
{
	glewExperimental = true;
	GLenum err = glewInit();
	if (err != GLEW_OK) {
		const GLubyte* msg = glewGetErrorString(err);
		throw std::exception(reinterpret_cast<const char*>(msg));
	}
}

void TriangleCanvas::SetupGraphics()
{
	// define vertices
	float points[] = {
		0.0f, 0.5f,
		0.5f, -0.5f,
		-0.5f, -0.5f
	};
	// upload vertex data
	glGenBuffers(1, &m_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);
	// setup vertex array objects
	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, NULL);
}

TriangleCanvas::~TriangleCanvas()
{
	SetCurrent(*m_context);
	glDeleteVertexArrays(1, &m_vao);
	glDeleteBuffers(1, &m_vbo);
}

void TriangleCanvas::OnPaint(wxPaintEvent& event)
{
	SetCurrent(*m_context);

	// set background to black
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// draw the graphics
	glDrawArrays(GL_TRIANGLES, 0, 3);
	// and display
	glFlush();
	SwapBuffers();
}