#include <stdlib.h>
#include <GL/glut.h>

void renderScene(void);

int main(int argc, char **argv) {
  glutInit(&argc, argv);
  glutInitWindowPosition(-1, -1);
  glutInitWindowSize(500, 500);	
  glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE | GLUT_DEPTH);

  glutCreateWindow("A Simple Window");
  glutDisplayFunc(renderScene);
  glutMainLoop();  
  return 0;
}

void renderScene(void) {
  glClear(GL_COLOR_BUFFER_BIT);
  glFlush();
}
