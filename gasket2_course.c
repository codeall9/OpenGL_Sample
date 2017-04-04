/* Recursive subdivision of triangle to form Sierpinski gasket */
//v1 flat triangle
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

/* initial triangle */
typedef GLfloat point2[2]; // a 2D point data type
point2 v[]={{-1.0f, -0.58f}, {1.0f, -0.58f}, {0.0f, 1.15f}};
int n = 3; /* number of recursive steps */

void triangle( point2 a, point2 b, point2 c)
{/* draw one triangle */
	glVertex2fv(a);
	glVertex2fv(b);
	glVertex2fv(c);
}

void divide_triangle(point2 a, point2 b, point2 c, int m)
{/* triangle subdivision using vertex numbers */
	point2 v0, v1, v2;
	int j;
	if(m > 0)
	{
		for(j=0; j<2; j++) v0[j]=(a[j]+b[j])/2;
		for(j=0; j<2; j++) v1[j]=(a[j]+c[j])/2;
		for(j=0; j<2; j++) v2[j]=(b[j]+c[j])/2;
		divide_triangle(a, v0, v1, m-1);
		divide_triangle(c, v1, v2, m-1);
		divide_triangle(b, v2, v0, m-1);
	}
	else triangle(a,b,c); /* draw triangle at end of recursion */
}

void myDisplay()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glBegin(GL_TRIANGLES);
		divide_triangle(v[0], v[1], v[2], n);
	glEnd();
	glFlush();
}

void myInit()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(-2.0, 2.0, -2.0, 2.0);
	glMatrixMode(GL_MODELVIEW);

	glClearColor (1.0, 1.0, 1.0, 1.0);
	glColor3f(0.0,0.0,0.0);
}

int main(int argc, char **argv)
{
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(500, 500);
	glutCreateWindow("2D Sierpinski Gasket");
	glutDisplayFunc(myDisplay);
	myInit();

	glutMainLoop();
	return 0;
}
