// Solar.c (with selection)
// OpenGL SuperBible
// Demonstrates OpenGL nested coordinate transformations
// and perspective
// Program by Richard S. Wright Jr.


#include <stdlib.h>
#include <stdio.h>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <math.h>

#define SIZE 512
//typedef enum { false, true } bool;

// Lighting values
GLfloat  whiteLight[] = { 0.2f, 0.2f, 0.2f, 1.0f };
GLfloat  sourceLight[] = { 0.8f, 0.8f, 0.8f, 1.0f };
GLfloat  sourceLightR[] = { 0.8f, 0.0f, 0.0f, 1.0f };
GLfloat	 lightPos[] = { 0.0f, 0.0f, 0.0f, 1.0f }; // if you want to lit the sun, you need to put the sun much higher (y-axis)
bool animating = true;
enum TargetID{SUN = 0,EARTH,CITY,MOON,MARS,PHOBUS};
const char* Target[] = {"SUN", "EARTH", "CITY", "MOON", "MARS", "PHOBUS" };
bool inSelection = false;

// processHits prints out the contents of the selection array
void processHits (GLint hits, GLuint buffer[])
{
	GLuint i, j, names, *ptr;

	printf ("----\nhits = %d\n", hits);
	ptr = (GLuint *) buffer;
	for (i = 0; i < hits; i++)
	{ //for each hit
		names = *ptr; //N
		ptr+=3; //skip MinZ and MaxZ
        printf("TOP");
        ptr+=(names-1);
		for (j = 0; j < names; j++)
		{ //for each name
			printf("-[%s]", Target[*ptr]);
			ptr--;
		}
		printf ("\n");
        ptr+=(names+1);
	}
}

// Called to draw scene
void myDisplay(void)
{
	// Earth and Moon angle of revolution
	static float fEarthRot = 0.0f;	//Blue Earth
	static float fMoonRot = 0.0f;	//Polar orbit (Gray)
	static float fMarsRot = 0.0f;	//Red Mars
	static float fMoonRot2 = 0.0f;	//Geostationary orbit (Purple)

	// Clear the window with current clearing color
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Save the matrix state and do the rotations
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0.0, 500.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0);

	// Set material color, Red
	// Sun
	if (inSelection) glPushName(SUN); //Push first Name
	glDisable(GL_LIGHTING);
	glColor3ub(255, 255, 0);
	glutSolidSphere(15.0f, 30, 17);
	glLightfv(GL_LIGHT0,GL_POSITION, lightPos);
	glEnable(GL_LIGHTING);
	
	if (inSelection) glPushName(EARTH);
	glPushMatrix();
		// Rotate coordinate system
		glRotatef(fEarthRot, 0.0f, 1.0f, 0.0f);	

		// Draw the Earth
		glColor3ub(0,0,220);
		glTranslatef(100.0f, 0.0f, 0.0f);
		glRotatef(fEarthRot, 0.0f, 1.0f, 0.0f);	
		glutSolidSphere(15.0f, 30, 17);
		
		if (inSelection) glPushName(CITY);
		//Draw the RED CITY (on the surface of Earth)
		glPushMatrix();
			glColor3ub(255,0,0);
			glTranslatef(15.0f, 0.0f, 0.0f);
			glutSolidSphere(4.0f, 30, 17);
		glPopMatrix();
		if (inSelection) glPopName();

		//Draw a Polar orbit to visit the city every round
		if (inSelection) glPushName(MOON);
		glPushMatrix(); /// *unnecessary statement
			glRotatef(fMoonRot, 0.0f, 0.0f, 1.0f);
			glColor3ub(150,150,150);
			glTranslatef(0.0f, 40.0f, 0.0f);
			glutSolidSphere(6.0f, 20, 17);
		glPopMatrix(); ///
		if (inSelection) glPopName();
	glPopMatrix();
	if (inSelection) glPopName();

	// Draw the Mars
	if (inSelection) glPushName(MARS);
	glPushMatrix(); ///
		glRotatef(fMarsRot, 0.0f, 1.0f, 0.0f);	
		glColor3ub(220,50,50);
		glTranslatef(180.0f, 0.0f, 0.0f);
		glRotatef(fMarsRot, 0.0f, 1.0f, 0.0f);	
		glutSolidSphere(10.0f, 30, 17);

		// Draw a Geostationary Orbit
		if (inSelection) glPushName(PHOBUS);
		glPushMatrix(); ///
			glRotatef(fMoonRot2, 0.0f, 1.0f, 0.0f);
			glColor3ub(140,140,240);
			glTranslatef(25.0f, 0.0f, 0.0f);
			glutSolidSphere(6.0f, 30, 17);
		glPopMatrix(); ///
		if (inSelection) glPopName();
	glPopMatrix(); ///
	if (inSelection) glPopName();

	// Update Rotation Angle
	if(!inSelection && animating){
		(fEarthRot > 360.0f) ? fEarthRot = 0.0f : (fEarthRot += 0.2f);
		(fMoonRot > 360.0f) ? fMoonRot = 0.0f : (fMoonRot += 2.0f);
		(fMarsRot > 360.0f) ? fMarsRot = 0.0f : (fMarsRot += 0.3f);
		(fMoonRot2 > 360.0f) ? fMoonRot2 = 0.0f : (fMoonRot2 += 2.0f);
	}
	
	// Show the image
	if(!inSelection)
		glutSwapBuffers();
	else
		glFlush();
}

void myMouse(int button, int state, int x, int y)
{
	GLuint selectBuf[SIZE];
	GLint hits;
	GLint viewport[4];

	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		glGetIntegerv (GL_VIEWPORT, viewport);

		glSelectBuffer (SIZE, selectBuf);
		glRenderMode(GL_SELECT); // *Enter selection mode (using mouse)

		glInitNames();

		glMatrixMode (GL_PROJECTION);
		glPushMatrix ();
		glLoadIdentity ();

		//gluPickMatrix creates a projection matrix that can be used to restrict drawing to 
		//  a small region of the viewport.
		// *create 1x1 pixel picking region near cursor location
		gluPickMatrix ((GLdouble) x, (GLdouble) (viewport[3] - y), 1.0, 1.0, viewport);

		gluPerspective(45.0f, (GLfloat)viewport[2]/(GLfloat)viewport[3], 1.0, 600.0);

		glMatrixMode(GL_MODELVIEW); ///This command can be ignore
		
		// Draw the scene
		inSelection = true;
		myDisplay();
		inSelection = false;

		glMatrixMode (GL_PROJECTION);
		glPopMatrix ();	//pop matrix which was made by gluPickMatrix + gluOrtho2D
		glFlush ();

		hits = glRenderMode (GL_RENDER); // *Reenter normal render mode
		processHits (hits, selectBuf);

		glutPostRedisplay();
	}
}

void myKeyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'p': case 'P':
		animating = !animating;
		break;
	}
}

// This function does any needed initialization on the rendering context. 
void init()
{
	// Light values and coordinates
	glEnable(GL_DEPTH_TEST);	// Hidden surface removal
	glEnable(GL_CULL_FACE);

	// Enable lighting
	glEnable(GL_LIGHTING);

	// Setup and enable light 0
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT,whiteLight);
	glLightfv(GL_LIGHT0,GL_DIFFUSE,sourceLight);
	glEnable(GL_LIGHT0);

	// Enable color tracking
	glEnable(GL_COLOR_MATERIAL);

	// Set Material properties to follow glColor values
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

	// Black blue background
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f );
}

void myTimer(int value)
{
	glutPostRedisplay();
	glutTimerFunc(10, myTimer, 1);
}

void myReshape(int w, int h)
{
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	// Prevent a divide by zero
	if(h == 0)
		h = 1;

	// field of view of 45 degrees, near and far planes 1.0 and 425
	gluPerspective(45.0f, (GLfloat)w/(GLfloat)h, 1.0, 600.0);
	
	glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(800, 600);
	glutCreateWindow("Earth/Moon?/Sun System");
	glutReshapeFunc(myReshape);
	glutDisplayFunc(myDisplay);
	glutMouseFunc(myMouse);
	glutKeyboardFunc(myKeyboard);
	glutTimerFunc(250, myTimer, 1);
	init();
	glutMainLoop();

	return 0;
}