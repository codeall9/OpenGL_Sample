// Demonstrates OpenGL nested coordinate transformations
// and perspective
// Program by Richard S. Wright Jr. (OpenGL SuperBible)

#include <cmath>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif


// Lighting values
GLfloat  whiteLight[] = { 0.2f, 0.2f, 0.2f, 1.0f };
GLfloat  sourceLight[] = { 0.8f, 0.8f, 0.8f, 1.0f };
GLfloat	 lightPos[] = { 0.0f, 0.0f, 0.0f, 1.0f };

bool MotionPlay = false;
float distance = 600;

void DrawAxis(int length = 30){ // a little tool for drawing local axis
	glPushAttrib(GL_LIGHTING_BIT);
	glDisable(GL_LIGHTING);
	glBegin(GL_LINES);
		glColor3ub(255,0,0);	//RED: X-axis
		glVertex3f(0,0,0); glVertex3f(length,0,0);
		glColor3ub(0,255,0);	//GREEN: Y-axis
		glVertex3f(0,0,0); glVertex3f(0,length,0);
		glColor3ub(0,0,255);	//Blue: Z-axis
		glVertex3f(0,0,0); glVertex3f(0,0,length);
	glEnd();
	glPopAttrib();
}

// Called to draw scene
void myDisplay(void)
{
	// Earth and Moon angle of revolution
	static float fEarthRot = 0.0f;	//Blue Earth

	// Clear the window with current clearing color
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0.0, distance, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0);
		
	// Set material color, Red
	// Sun
	glColor3ub(255, 255, 0);
	glutSolidSphere(15.0f, 30, 17);
	DrawAxis(50);
	
	// Draw the Earth and its system
	// [the Earth, Size = 13]
	glColor3ub(0,0,220);
	glTranslatef(100.0f, 0.0f, 0.0f);
	glutSolidSphere(13.0f, 30, 17);
	DrawAxis();

	// [the RED CITY, size = 4] 
	// (On the surface of Earth)
	glColor3ub(255,0,0);
	glTranslatef(-13.0f, 0.0f, 0.0f);
	glutSolidSphere(4.0f, 30, 17);
	DrawAxis();
	
	if(MotionPlay){
		(fEarthRot > 360.0f) ? fEarthRot = 0.0f : (fEarthRot += 0.5f);	//25 deg/sec
	}
	
	// Show the image
	glutSwapBuffers();
}

// Called to draw scene
void myDisplay2(void)
{
	// Earth and Moon angle of revolution
	static float fEarthRot = 0.0f;	//Blue Earth
	static float fMoonRot = 0.0f;	//orbit (Gray)
	static float fMarsRot = 0.0f;	//Red Mars

	// Clear the window with current clearing color
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0.0, distance, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0);
		
	// Set material color, Red
	// Sun
	glDisable(GL_LIGHTING);
	glColor3ub(255, 255, 0);
	glutSolidSphere(15.0f, 30, 17);
	if(!MotionPlay) DrawAxis(100.0f);
	glEnable(GL_LIGHTING);

	glLightfv(GL_LIGHT0,GL_POSITION, lightPos);	

	// Draw the Earth and its system
	glPushMatrix();
		// [the Earth, Size = 13]
		// *Distance from Sun: 100, 
		// *Relative angular velocity: 25 deg./sec (Sun)
		// *Self rotation rate: 100 deg./sec (Sun)
		glRotatef(fEarthRot, 0.0f, 1.0f, 0.0f);		//25 deg./sec
		glColor3ub(0,0,220);
		glTranslatef(100.0f, 0.0f, 0.0f);
		glPushMatrix();
			glRotatef(fEarthRot*4, 0.0f, 1.0f, 0.0f);	//速度快四倍，但會比實際上多繞一圈
			//glRotatef(fEarthRot*4-fEarthRot, 0.0f, 1.0f, 0.0f);   //正確版，消弭原來地球公轉的影響對自轉的影響
			glutSolidSphere(13.0f, 30, 17);
			if(!MotionPlay) DrawAxis(30.0f);

			// [the RED CITY, size = 4] 
			// (On the surface of Earth)
			glPushMatrix();
				glColor3ub(255,0,0);
				glTranslatef(-13.0f, 0.0f, 0.0f);
				glutSolidSphere(4.0f, 30, 17);
			glPopMatrix();
		glPopMatrix();

		// [the Moon size = 6] 
		// *Distance from Earth: 30, 
		// *Relative angular velocity: 150 deg./sec (Sun)
		// *no self rotation rate: 150 deg./sec (Sun)
		glPushMatrix();
			glRotatef(fMoonRot, 0.0f, 0.0f, 1.0f);
			//glRotatef(fMoonRot-fEarthRot, 0.0f, 1.0f, 0.0f);   //正確版，消弭原來地球公轉的影響對自轉的影響
			glColor3ub(150,150,150);
			glTranslatef(30.0f, 0.0f, 0.0f);
			glutSolidSphere(6.0f, 30, 17);
			if(!MotionPlay) DrawAxis(20.0f);
		glPopMatrix();
	// End of Earth system
	glPopMatrix();

	// Draw the Mars and its system
	glPushMatrix();
		// [Draw the Mars, Size = 10]
		// *Distance from Sun: 180, 
		// *Relative angular velocity: 50 deg./sec (Sun), 
		// *Self rotation rate: 100 deg./sec (Sun))
		glRotatef(fMarsRot, 0.0f, 1.0f, 0.0f);	
		glColor3ub(220,50,50);
		glTranslatef(180, 0.0f, 0.0f);
		glRotatef(fMarsRot*2 - fMarsRot, 0.0f, 1.0f, 0.0f);	   //正確版，消弭原來火星公轉的影響對自轉的影響
		glutSolidSphere(10.0f, 30, 17);
		if(!MotionPlay) DrawAxis();

		// [Draw the GREEN CITY] (on the surface of Mars, size = 4)
		glPushMatrix();
			glColor3ub(0,200,0);
			glTranslatef(10.0f, 0.0f, 0.0f);
			glutSolidSphere(4.0f, 30, 17);
			if(!MotionPlay) DrawAxis();
		glPopMatrix();

		// [Draw a Geosynchronous Orbit to keep observing the city, size = 4]
		// *Distance from Mars's "surface": 25, 
		// *Relative angular velocity: 0 (GREEN CITY)
		glColor3ub(140,140,240);
		glTranslatef(10.0f+25.0f, 0.0f, 0.0f);
		glutSolidSphere(4.0f, 30, 17);
		if(!MotionPlay) DrawAxis(10);
	glPopMatrix();

	//you may add additional star
	
	if(MotionPlay){
		(fEarthRot > 360.0f) ? fEarthRot = 0.0f : (fEarthRot += 0.5f);	//25 deg/sec
		(fMoonRot > 360.0f) ? fMoonRot = 0.0f : (fMoonRot += 3.0f);		//150 deg/sec
		(fMarsRot > 360.0f) ? fMarsRot = 0.0f : (fMarsRot += 1.0f);		//50 deg/sec
	}
	
	// Show the image
	glutSwapBuffers();
}

void myKey(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'P': case 'p':
		MotionPlay = !MotionPlay; break;
	case 'W': case 'w':
		distance -= 100.0f; break;
	case 'S': case 's':
		distance += 100.0f; break;
	default:
		break;
	}
	return;
}

// This function does any needed initialization on the rendering context. 
void init()
{
	// Light values and coordinates
	glEnable(GL_DEPTH_TEST);	// Hidden surface removal

	// Enable lighting
	glEnable(GL_LIGHTING);
	// Setup and enable light 0
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT,whiteLight);
	glLightfv(GL_LIGHT0,GL_DIFFUSE,sourceLight);
	glLightfv(GL_LIGHT0,GL_POSITION,lightPos);
	glEnable(GL_LIGHT0);
	// Enable color material
	glEnable(GL_COLOR_MATERIAL);

	// Set Material properties to follow glColor values directly
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

	// Black background
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f );
}

void myTimer(int value)
{
	glutPostRedisplay();
	glutTimerFunc(value, myTimer, value);
}

void myReshape(int w, int h)
{
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	// field of view of 45 degrees, near and far planes 1.0 and 425
	gluPerspective(45.0f, (GLfloat)w/(GLfloat)h, 1.0, 1000.0);
	
	glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(800, 800);
	glutCreateWindow("Earth/Moon?/Sun System");
	glutReshapeFunc(myReshape);
	glutDisplayFunc(myDisplay);
	glutKeyboardFunc(myKey); //add keyboard function
	glutTimerFunc(20, myTimer, 20); //~50fps
	init();
	glutMainLoop();

	return 0;
}
