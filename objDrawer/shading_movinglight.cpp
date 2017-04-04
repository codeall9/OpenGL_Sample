#include <cmath>
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif


// initial values of tetrahedron
typedef float point[3];
point v[]={{0.0, 0.0, 1.0}, {0.0, 0.942809, -0.33333},
{-0.816497, -0.471405, -0.333333}, {0.816497, -0.471405, -0.333333}};

//default setting, a directional light at +z, with light shining along the -z axis.
GLfloat light_position[]={0.0, 0.0, 0.0, 1.0};
GLfloat CameraSRotX = 0.0, CameraSRotY = 0.0;
int lastPosX = -1, lastPosY = -1;
GLfloat ScaleX = 1.0f, ScaleY = 1.0f;
bool ActiveMouse = false;

int n = 5;
int mode = 0;

GLfloat mat_ambient[]={0.0, 0.0, 0.0, 1.0};
GLfloat mat_diffuse[]={0.0, 0.0, 0.0, 1.0};

void drawAxis(void)
	/* a little tool for drawing local axis */
{ 
	glEnable(GL_COLOR_MATERIAL);
	glBegin(GL_LINES);
	glColor3ub(255,0,0); //X-Axis (Red)
	glVertex3f(0,0,0); glVertex3f(30,0,0);
	glColor3ub(0,255,0); //Y-Axis (Green)
	glVertex3f(0,0,0); glVertex3f(0,30,0);
	glColor3ub(0,0,255); //Z-Axis (Blue)
	glVertex3f(0,0,0); glVertex3f(0,0,30);
	glEnd();
	glDisable(GL_COLOR_MATERIAL);
}

void triangle( point a, point b, point c)
	/* display one triangle using a line loop for wire frame, a single
	normal for constant shading, or three normals for interpolative shading */
{
	if (mode==0) glBegin(GL_LINE_LOOP);
	else glBegin(GL_POLYGON);
	if(mode==1) glNormal3fv(a); //flat shading (1 normal pre triangle)
	if(mode==2) glNormal3fv(a); //smooth shading (3 normal per triangle)
	glVertex3fv(a);
	if(mode==2) glNormal3fv(b);
	glVertex3fv(b);
	if(mode==2) glNormal3fv(c);
	glVertex3fv(c);
	glEnd();
}

void normalize(point p)
	/* normalize a vector */
{
	double d = 0.0;
	int i;
	for(i=0; i<3; i++) d+=p[i]*p[i];
	d=sqrt(d);
	if(d>0.0) for(i=0; i<3; i++) p[i]/=d;
}

void divide_triangle(point a, point b, point c, int m)
	/* triangle subdivision using vertex numbers
	righthand rule applied to create outward pointing faces */
{
	point v1, v2, v3;
	int j;
	if(m>0)
	{
		for(j=0; j<3; j++) v1[j]=a[j]+b[j];
		normalize(v1);
		for(j=0; j<3; j++) v2[j]=a[j]+c[j];
		normalize(v2);
		for(j=0; j<3; j++) v3[j]=b[j]+c[j];
		normalize(v3);
		divide_triangle(a, v1, v2, m-1);
		divide_triangle(c, v2, v3, m-1);
		divide_triangle(b, v3, v1, m-1);
		divide_triangle(v1, v3, v2, m-1);
	}
	else(triangle(a,b,c)); /* draw triangle at end of recursion */
}

void tetrahedron( int m)
{/* Apply triangle subdivision to faces of tetrahedron */
	divide_triangle(v[0], v[1], v[2], m);
	divide_triangle(v[3], v[2], v[1], m);
	divide_triangle(v[0], v[3], v[1], m);
	divide_triangle(v[0], v[2], v[3], m);
}

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	
	glDisable(GL_LIGHTING);	// temporally disalbe lighting
	glPushMatrix();
		glRotatef(CameraSRotX, 1.0, 0.0, 0.0); //Rotate the light
		glRotatef(CameraSRotY, 0.0, 1.0, 0.0);
		drawAxis();
		glTranslatef(0.0f,0.0f,3.0f);
		glLightfv(GL_LIGHT0, GL_POSITION, light_position);	
		glColor3ub(255,255,0);
		glutSolidSphere(0.2,30,30);
	glPopMatrix();	
	glEnable(GL_LIGHTING);	// enable lighting

	mode=1; //flat shading (1 normal pre triangle)
	glTranslatef(-1.2, 0.0,0.0);
	mat_ambient[0] = 1.0; mat_ambient[1] = 0.0;
	mat_diffuse[0] = 1.0; mat_diffuse[1] = 0.0;
	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glShadeModel(GL_FLAT);
	//tetrahedron(n);
	glutSolidSphere(1.0,15,15);

	mode=2; //smooth shading (3 normal per triangle)
	glTranslatef( 2.4, 0.0,0.0);
	mat_ambient[0] = 0.0; mat_ambient[1] = 1.0;
	mat_diffuse[0] = 0.0; mat_diffuse[1] = 1.0;
	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glShadeModel(GL_SMOOTH);
	//tetrahedron(n);
	glutSolidSphere(1.0,15,15);

	glutSwapBuffers();
}


void myReshape(int w, int h)
{
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	if (w <= h)
		glOrtho(-4.0, 4.0, -4.0 * (GLfloat) h / (GLfloat) w,
		4.0 * (GLfloat) h / (GLfloat) w, -10.0, 10.0);
	else
		glOrtho(-4.0 * (GLfloat) w / (GLfloat) h,
		4.0 * (GLfloat) w / (GLfloat) h, -4.0, 4.0, -10.0, 10.0);

	ScaleX = 180.0f / w;
	ScaleY = 180.0f / h;

	glMatrixMode(GL_MODELVIEW);
}

void myMouseMotion(int x, int y)
{
	if(ActiveMouse){
		int dx, dy;
		dx = x - lastPosX;
		dy = y - lastPosY;
		CameraSRotY += dx*ScaleX;
		CameraSRotX += dy*ScaleY;
		glutPostRedisplay();
		lastPosX = x;
		lastPosY = y;
	}
}

void myMouseButton(int button, int state, int x, int y)
{
	if(button==GLUT_RIGHT_BUTTON) exit(0);
	if(button==GLUT_LEFT_BUTTON) switch(state) 
	{
	case GLUT_DOWN:
		lastPosX = x;
		lastPosY = y;
		ActiveMouse = true;
		break;
	case GLUT_UP:
		ActiveMouse = false;
		break;
	} 
}

void myinit()
{
	GLfloat mat_specular[]={1.0, 1.0, 1.0, 1.0};
	GLfloat mat_shininess={16.0};

	GLfloat light_ambient[]={0.3, 0.3, 0.3, 1.0};
	GLfloat light_diffuse[]={0.2, 0.2, 0.2, 1.0};
	GLfloat light_specular[]={1.0, 1.0, 1.0, 1.0};

	/* set up ambient, diffuse, and specular components for light 0 */
	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);

	/* define material proerties for front face of all polygons */
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glMaterialf(GL_FRONT, GL_SHININESS, mat_shininess);

	glShadeModel(GL_SMOOTH);/* enable smooth shading */
	glEnable(GL_LIGHTING);	/* enable lighting */
	glEnable(GL_LIGHT0);	/* enable light 0 */
	glEnable(GL_DEPTH_TEST);/* enable z buffer */

	glClearColor (1.0, 1.0, 1.0, 1.0);
	glColor3f (0.0, 0.0, 0.0);
}

void
	main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(800, 800);
	glutCreateWindow("twin spheres");
	myinit();
	glutReshapeFunc(myReshape);
	glutDisplayFunc(display);

	glutMouseFunc(myMouseButton);
	glutMotionFunc(myMouseMotion);

	glutMainLoop();
}