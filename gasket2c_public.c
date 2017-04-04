/* Recursive subdivision of triangle to form Sierpinski gasket */
//v2 fake 3D triangles
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

/* initial triangle */
typedef GLfloat point3[3]; // a 3D point data type
point3 v_2D[]={{-1.0f, -0.58f, 0.0f}, {1.0f, -0.58f, 0.0f}, {0.0f, 1.15f, 0.0f}};
point3 v[]={{0.0f, 0.0f, 1.15f}, {-1.0f, -0.58f, -0.58f}, {1.0f, -0.58f, -0.58f}, {0.0f, 1.15f, -0.58f}};
int n = 3;

void triangle( point3 a, point3 b, point3 c) {
/* specify one triangle */
       glVertex3fv(a);
       glVertex3fv(b);
       glVertex3fv(c);
}

void divide_triangle(point3 a, point3 b, point3 c, int m) {
/* triangle subdivision using vertex numbers */
    point3 v0, v1, v2;
    int j;
    if(m>0)
    {
        for(j=0; j<3; j++) v0[j]=(a[j]+b[j])/2;
        for(j=0; j<3; j++) v1[j]=(a[j]+c[j])/2;
        for(j=0; j<3; j++) v2[j]=(b[j]+c[j])/2;
        divide_triangle(a, v0, v1, m-1);
        divide_triangle(c, v1, v2, m-1);
        divide_triangle(b, v2, v0, m-1);
    }
    else triangle(a,b,c); /* draw triangle at end of recursion */
}

void tetrahedron( int m) {
/* Apply triangle subdivision to faces of tetrahedron */
	glColor3f(1.0,0.0,0.0);
    divide_triangle(v[0], v[1], v[2], m);
	glColor3f(0.0,1.0,0.0);
    divide_triangle(v[3], v[2], v[1], m);
	glColor3f(0.0,0.0,1.0);
    divide_triangle(v[0], v[3], v[1], m);
	glColor3f(0.0,0.0,0.0);
    divide_triangle(v[0], v[2], v[3], m);
}

void myDisplay() {
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    glBegin(GL_TRIANGLES);
    //divide_triangle(v_2D[0], v_2D[1], v_2D[2], n);
	tetrahedron(n);
    glEnd();
    glFlush();
}

void myReshape(int w, int h)
{
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
	//gluOrtho2D(-2.0, 2.0, -2.0, 2.0);
	glOrtho(-2.0, 2.0, -2.0, 2.0, -5.0, 5.0);
    //if (w <= h)
    //    glOrtho(-2.0, 2.0, -2.0 * (GLfloat) h / (GLfloat) w,
    //        2.0 * (GLfloat) h / (GLfloat) w, -10.0, 10.0);
    //else
    //    glOrtho(-2.0 * (GLfloat) w / (GLfloat) h,
    //        2.0 * (GLfloat) w / (GLfloat) h, -2.0, 2.0, -10.0, 10.0);
    glMatrixMode(GL_MODELVIEW);
    glutPostRedisplay();
}

//useless
//void myInit() {
//	glMatrixMode(GL_PROJECTION);
//	glLoadIdentity();
//	gluOrtho2D(-2.0, 2.0, -2.0, 2.0);
//	glMatrixMode(GL_MODELVIEW);
//	glClearColor (1.0, 1.0, 1.0, 1.0);
//	glColor3f(0.0,0.0,0.0);
//}

int main(int argc, char **argv) { 
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH); // <-- + GLUT_DEPTH
    glutInitWindowSize(500, 500);
    glutCreateWindow("Fake 3D Sierpinski Gasket");
    glutDisplayFunc(myDisplay);
	glutReshapeFunc(myReshape); // <-- + myReshape
    
	//glEnable(GL_DEPTH_TEST);
	glClearColor (1.0, 1.0, 1.0, 1.0);
	glColor3f (0.0, 0.0, 0.0);
    
	glutMainLoop();
}
