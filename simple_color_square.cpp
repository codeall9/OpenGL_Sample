/* a Cube with color interpolation */

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

GLfloat vertices[][3] = {{-1.0,-1.0},{1.0,-1.0}, {1.0,1.0}, {-1.0,1.0}};

GLfloat colors[][3] = {{0.0,0.0,0.0},{1.0,0.0,0.0},
	{1.0,1.0,0.0}, {0.0,1.0,0.0}, {0.0,0.0,1.0}, 
	{1.0,0.0,1.0}, {1.0,1.0,1.0}, {0.0,1.0,1.0}};

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	glBegin(GL_POLYGON);
		glColor3fv(colors[0]);
		glVertex2fv(vertices[0]);
		glColor3fv(colors[1]);
		glVertex2fv(vertices[1]);
		glColor3fv(colors[2]);
		glVertex2fv(vertices[2]);
		glColor3fv(colors[3]);
		glVertex2fv(vertices[3]);
	glEnd();

	glFlush();
}

void myReshape(int w, int h)
{
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
	gluOrtho2D(-2.0, 2.0, -2.0, 2.0);
	//void gluOrtho2D(GLdouble left, GLdouble right, GLdouble bottom, GLdouble top);
	//https://www.opengl.org/sdk/docs/man2/xhtml/gluOrtho2D.xml
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowPosition(0,0);
    glutInitWindowSize(600, 600);
    glutCreateWindow("Color Square");
    glutReshapeFunc(myReshape);
    glutDisplayFunc(display);
    
	glutMainLoop();
}
