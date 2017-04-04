/* Dual Rotating cube with dual screen */

#include <time.h> //STEP:5.2 add this for timer

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

	GLfloat vertices[][3] = {{-1.0,-1.0},{1.0,-1.0}, {1.0,1.0}, {-1.0,1.0}};

	GLfloat colors[][3] = {{0.0,0.0,0.0},{1.0,0.0,0.0},
	{1.0,1.0,0.0}, {0.0,1.0,0.0}, {0.0,0.0,1.0}, 
	{1.0,0.0,1.0}, {1.0,1.0,1.0}, {0.0,1.0,1.0}};
	
	int WinID[] = {0,0};
	int m = 0; //STEP:1
	int width = 0, height = 0;
	float theta = 0; //STEP:5.1 angle of current spinning cube

void display(void)
{
    /* display callback, clear frame buffer and z buffer,
       rotate cube and draw, swap buffers */
	//Note:http://en.cppreference.com/w/cpp/language/operator_precedence
	glClear(GL_COLOR_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glRotatef(theta, 0.0, 0.0, 1.0); //STEP:5.3 Rotate it!
	
	glViewport(0, 0, 0.5*width, 0.5*height);
	glBegin(GL_POLYGON);
		glColor3fv(colors[m]); //STEP:1.1
		glVertex2fv(vertices[0]);
		
		glColor3fv(colors[(m+1)%7]); //STEP:1.2
		glVertex2fv(vertices[1]);
		
		glColor3fv(colors[(m+2)%7]); //
		glVertex2fv(vertices[2]);
		
		glColor3fv(colors[(m+3)%7]); //
		glVertex2fv(vertices[3]);
	glEnd();

	//STEP: 4.4 add new code segment for new viewport
	glViewport(0.5*width, 0.5*height, 0.5*width, 0.5*height);
	glBegin(GL_POLYGON);
		glColor3fv(colors[m+3]); //STEP:1.1
		glVertex2fv(vertices[0]);
		
		glColor3fv(colors[(m+4)%7]); //STEP:1.2
		glVertex2fv(vertices[1]);
		
		glColor3fv(colors[(m+5)%7]); //
		glVertex2fv(vertices[2]);
		
		glColor3fv(colors[(m+6)%7]); //
		glVertex2fv(vertices[3]);
	glEnd();

	glFlush();
}

void mouse(int btn, int state, int x, int y)
{
	/* mouse callback, selects an axis about which to rotate */
    if(btn==GLUT_LEFT_BUTTON && state == GLUT_DOWN)
		m = (m+1)%7; //STEP:2, use mouse control the number "m"
	glutPostRedisplay(); //STEP:3, ask it to re-display
}

void myReshape(int w, int h)
{
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-2.0, 2.0, -2.0, 2.0);
}

void myReshape2(int w, int h)
{
    glViewport(0, 0, 0.5*w, 0.5*h); //STEP:4.3 change viewport parameter p2
    //glViewport(0.5*w, 0.5*h, 0.5*w, 0.5*h); //add new viewport?? NONONO
	width = w; height = h;
	glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-2.0, 2.0, -2.0, 2.0);
}

void spinIt() //STEP:5.2 spin it
{	
	clock_t now = clock(); //need time.h
	static clock_t lastTime = 0;
	if((now - lastTime)/(float)CLOCKS_PER_SEC > (1.0f/60.0f)){ //<60fps

		/* Idle callback, spin cube 2 degrees about selected axis */
		theta += 2;
		if( theta > 360.0 ) theta -= 360.0;

		glutSetWindow(WinID[0]); //STEP:5.4 only work on 1 screen?? add this
		glutPostRedisplay();

		glutSetWindow(WinID[1]); //add this to redraw 2nd screen
		glutPostRedisplay();
	
		// Record current time for next time:
		lastTime = now;
	}
}

void main(int argc, char **argv)
{
    glutInit(&argc, argv);

    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowPosition(0,0);
    glutInitWindowSize(500, 500);
    WinID[0] = glutCreateWindow("colorcube 1st");
    glutReshapeFunc(myReshape);
    glutDisplayFunc(display);
	glutIdleFunc(spinIt); //STEP:5 add spin function
	glutMouseFunc(mouse);

	//STEP:4, create second window
	//glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB); //same as previous one
	glutInitWindowPosition(550,0);
    //glutInitWindowSize(500, 500);
    WinID[1] = glutCreateWindow("colorcube 2nd"); //STEP:4.1 What is windows ID
    glutReshapeFunc(myReshape2); //STEP:4.2 change viewport parameter
    glutDisplayFunc(display);
	glutIdleFunc(spinIt); //STEP:5 add spin function
	glutMouseFunc(mouse);
    
	glutMainLoop();
}
