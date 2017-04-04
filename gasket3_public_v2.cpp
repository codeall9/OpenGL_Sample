/* Recursive subdivision of triangle to form Sierpinski gasket */
//v3 real 3D tetrahedrons

#include <cstdlib> //atoi()

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <ctime> //clock_t

/* initial tetrahedron */
typedef GLfloat point3[3]; // a 3D point data type
point3 v[]={{0.0, 0.0, 1.15}, {-1.0, -0.58, -0.58}, 
{1.0, -0.58, -0.58}, {0.0, 1.15, -0.58}};

GLfloat colors[4][3] = {{1.0, 0.0, 0.0}, {0.0, 1.0, 0.0},
{0.0, 0.0, 1.0}, {0.0, 0.0, 0.0}};

int n = 3;
float theta = 0.5; //STEP:5.1 angle of current spinning cube

void triangle( point3 a, point3 b, point3 c) {
	/* specify one triangle */
	glVertex3fv(a);
	glVertex3fv(b);
	glVertex3fv(c);
}

void tetrahedron(point3 a, point3 b, point3 c, point3 d)
{
	glColor3fv(colors[0]);
	triangle(a, b, c);
	glColor3fv(colors[1]);
	triangle(a, c, d);
	glColor3fv(colors[2]);
	triangle(a, d, b);
	glColor3fv(colors[3]);
	triangle(b, d, c);
}

void divide_tetra(point3 a, point3 b, point3 c, point3 d, int m)
{
	point3 v0, v1, v2, v3, v4, v5;
	int j;
	if(m>0)
	{
		/* compute six midpoints */
		for(j=0; j<3; j++) v0[j]=(a[j]+b[j])/2;
		for(j=0; j<3; j++) v1[j]=(a[j]+c[j])/2;
		for(j=0; j<3; j++) v2[j]=(a[j]+d[j])/2;
		for(j=0; j<3; j++) v3[j]=(b[j]+c[j])/2;
		for(j=0; j<3; j++) v4[j]=(c[j]+d[j])/2;
		for(j=0; j<3; j++) v5[j]=(b[j]+d[j])/2;

		/* create 4 tetrahedrons by subdivision */
		divide_tetra(a, v0, v1, v2, m-1);
		divide_tetra(v0, b, v3, v5, m-1);
		divide_tetra(v1, v3, c, v4, m-1);
		divide_tetra(v2, v5, v4, d, m-1);
	}
	else(tetrahedron(a,b,c,d)); /* draw tetrahedron at end of recursion */
}

void myDisplay()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	//glLoadIdentity();
	glRotatef(theta, 0.0, -1.0, -1.0); //STEP:5.3 Rotate it!
	glBegin(GL_TRIANGLES);
	divide_tetra(v[0], v[1], v[2], v[3], n);
	glEnd();
	glFlush();
}

void myReshape(int w, int h)
{
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	if (w <= h)
		glOrtho(-2.0, 2.0, -2.0 * (GLfloat) h / (GLfloat) w,
		2.0 * (GLfloat) h / (GLfloat) w, -10.0, 10.0);
	else
		glOrtho(-2.0 * (GLfloat) w / (GLfloat) h,
		2.0 * (GLfloat) w / (GLfloat) h, -2.0, 2.0, -10.0, 10.0);
	glMatrixMode(GL_MODELVIEW);
	glutPostRedisplay();
}

void myKey(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'Q': case 'q':
		if(n > 0) n--; break;
	case 'E': case 'e':
		n++; break;
	default:
		break;
	}
	glutPostRedisplay();
}

void spinIt() //STEP:5.2 spin it
{
	clock_t now = clock(); //need time.h
	static clock_t lastTime = 0;
	if ((now - lastTime) / (float)CLOCKS_PER_SEC > (1.0f / 60.0f)) { //<60fps

		/* Idle callback, spin cube 2 degrees about selected axis */
		//theta += 2;
		if (theta > 360.0) theta -= 360.0;
		glutPostRedisplay();

		// Record current time for next time:
		lastTime = now;
	}
}

int main(int argc, char **argv)
{
	if(argc > 1) n=atoi(argv[1]); /* or set number of subdivision steps here */
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(500, 500);
	glutCreateWindow("3D Gasket");
	glutReshapeFunc(myReshape);
	glutDisplayFunc(myDisplay);
	glutKeyboardFunc(myKey); //add keyboard function
	glutIdleFunc(spinIt); //STEP:5 add spin function
	glEnable(GL_DEPTH_TEST);
	glClearColor (1.0, 1.0, 1.0, 1.0);
	glutMainLoop();
}
