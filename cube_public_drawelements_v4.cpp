/* Rotating cube with color interpolation (with glDrawElements) */

/* Both normals and colors are assigned to the vertices */
/* Cube is centered at origin so (unnormalized) normals
are the same as the vertex values */

#include <ctime>
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

GLfloat vertices[][3] = {{-1.0,-1.0,-1.0},{1.0,-1.0,-1.0},
{1.0,1.0,-1.0}, {-1.0,1.0,-1.0}, {-1.0,-1.0,1.0}, 
{1.0,-1.0,1.0}, {1.0,1.0,1.0}, {-1.0,1.0,1.0}};

GLfloat normals[][3] = {{-1.0,-1.0,-1.0},{1.0,-1.0,-1.0},
{1.0,1.0,-1.0}, {-1.0,1.0,-1.0}, {-1.0,-1.0,1.0}, 
{1.0,-1.0,1.0}, {1.0,1.0,1.0}, {-1.0,1.0,1.0}};

GLfloat colors[][3] = {{1.0,1.0,1.0},{1.0,0.0,0.0},
{1.0,1.0,0.0}, {0.0,1.0,0.0}, {0.0,0.0,1.0}, 
{1.0,0.0,1.0}, {1.0,1.0,1.0}, {0.0,1.0,1.0}};

GLubyte cubeIndices[24] = {3,2,1,0, 2,3,7,6, 7,3,0,4, 1,2,6,5, 4,5,6,7, 0,1,5,4};

int WinID[] = {0,0};

void polygon(int a, int b, int c , int d)
{/* draw a polygon via list of vertices */
	glBegin(GL_POLYGON);
	glColor3fv(colors[a]);
	glNormal3fv(normals[a]);
	glVertex3fv(vertices[a]);
	glColor3fv(colors[b]);
	glNormal3fv(normals[b]);
	glVertex3fv(vertices[b]);
	glColor3fv(colors[c]);
	glNormal3fv(normals[c]);
	glVertex3fv(vertices[c]);
	glColor3fv(colors[d]);
	glNormal3fv(normals[d]);
	glVertex3fv(vertices[d]);
	glEnd();
}

void colorcube(void)
{/* map vertices to faces */
	polygon(3,2,1,0); //Green
	polygon(2,3,7,6); //R+G = Yellow
	polygon(7,3,0,4); //B+G = Cyan
	polygon(1,2,6,5); //Red
	polygon(4,5,6,7); //Blue
	polygon(0,1,5,4); //White
}

static GLfloat theta[] = {0.0,0.0,0.0};
static GLint axis = 2;

void display(void)
{
	//Display callback, clear frame buffer and z buffer,
	//rotate cube and draw, swap buffers

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();
	glRotatef(theta[0], 1.0, 0.0, 0.0);
	glRotatef(theta[1], 0.0, 1.0, 0.0);
	glRotatef(theta[2], 0.0, 0.0, 1.0);

	colorcube(); //< Immediate mode: http://en.wikipedia.org/wiki/Immediate_mode

	//glEnableClientState(GL_VERTEX_ARRAY); 
	//glEnableClientState(GL_COLOR_ARRAY);
	//glEnableClientState(GL_NORMAL_ARRAY); 

	//// Draw with Polygon mode ////////
	//for(int i=0; i<6; i++) 
	//	glDrawElements(GL_POLYGON, 4, GL_UNSIGNED_BYTE, &cubeIndices[4*i]);

	//// Draw with Quads mode ////////
	//glDrawElements(GL_QUADS, 24, GL_UNSIGNED_BYTE, &cubeIndices[0]);

	//glDisableClientState(GL_VERTEX_ARRAY);
	//glDisableClientState(GL_COLOR_ARRAY);
	//glDisableClientState(GL_NORMAL_ARRAY);

	glFlush();
	glutSwapBuffers();
}

void spinCube()
{	
	clock_t now = clock();
	static clock_t lastTime = 0;
	if((now - lastTime)/(float)CLOCKS_PER_SEC > (1.0f/60.0f)){ //<60fps

		// Idle callback, spin cube 0.3 degrees about selected axis
		theta[axis] += 2;
		if( theta[axis] > 360.0 ) theta[axis] -= 360.0;

		glutPostRedisplay();

		// Record current time for next time:
		lastTime = now;
	}
}

void mouse(int btn, int state, int x, int y)
{
	//Mouse callback, selects an axis about which to rotate
	if(btn==GLUT_LEFT_BUTTON && state == GLUT_DOWN) axis = 0;
	if(btn==GLUT_MIDDLE_BUTTON && state == GLUT_DOWN) axis = 1;
	if(btn==GLUT_RIGHT_BUTTON && state == GLUT_DOWN) axis = 2;
}

void myReshape(int w, int h)
{
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	//glLoadIdentity();
	if (w <= h)
		glOrtho(-2.0, 2.0, -2.0 * (GLfloat) h / (GLfloat) w,
		2.0 * (GLfloat) h / (GLfloat) w, -10.0, 10.0);
	else
		glOrtho(-2.0 * (GLfloat) w / (GLfloat) h,
		2.0 * (GLfloat) w / (GLfloat) h, -2.0, 2.0, -10.0, 10.0);
	glMatrixMode(GL_MODELVIEW);
}

void Init(void)
{
	//Enable hidden--surface--removal////////////
	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_CULL_FACE);
	//glCullFace(GL_FRONT); //default: GL_BACK

	glShadeModel(GL_FLAT); //use color or first point as color of polygon

	//Added for glDrawElements
	glVertexPointer(3, GL_FLOAT, 0, vertices);
	glColorPointer(3, GL_FLOAT, 0, colors);
	glNormalPointer(GL_FLOAT, 0, normals);

}

void main(int argc, char **argv)
{
	glutInit(&argc, argv);
	//Both double buffering and z buffer are needed
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowPosition(0,0);
	glutInitWindowSize(500, 500);
	WinID[0] = glutCreateWindow("colorcube");
	glutReshapeFunc(myReshape);
	glutDisplayFunc(display);
	glutIdleFunc(spinCube);
	glutMouseFunc(mouse);
	Init();

	glutMainLoop();
}
