#include <stdlib.h>
#include <GL/glut.h>

GLfloat CarBodySize = 10.0;
GLfloat CarWheelSize = 3.5;

GLfloat CarWheelAngle = 0.0;
GLfloat CarBodyAngle = 0.0;

GLfloat CameraSRotX = 0.0, CameraSRotY = 0.0;
GLfloat CameraTRotX = 0.0, CameraTRotY = 0.0;

int lastPosX = -1, lastPosY = -1;
GLfloat ScaleX = 1.0f, ScaleY = 1.0f;
GLfloat CamShiftFB = 0.0f,CamShiftLR = 0.0f;

bool MotionPlay = false;
bool ActiveMouse = false;
bool Ctrl_actived = false;

void DrawAxis(void){ // a little tool for drawing local axis
	glBegin(GL_LINES);
	glColor3ub(255,0,0); //X-Axis
	glVertex3f(0,0,0); glVertex3f(30,0,0);
	glColor3ub(0,255,0); //Y-Axis
	glVertex3f(0,0,0); glVertex3f(0,30,0);
	glColor3ub(0,0,255); //Z-Axis
	glVertex3f(0,0,0); glVertex3f(0,0,30);
	glEnd();
}

void DrawCar(void)
{
	//車身
	//glRotatef(CarBodyAngle, 0.0, 1.0, 0.0);
	glTranslatef(20.0, 0.0, 0.0);

	glColor3ub(150,0,0);
	glPushMatrix();
	glScalef(1.0, 0.5, 1.0);
		glutSolidCube(CarBodySize);
	glPopMatrix();

	//Right wheel(1)
	glPushMatrix();
		glColor3ub(150,100,0);
		glTranslatef(CarBodySize*0.5+CarWheelSize*0.5, 0.0, -CarBodySize*0.5+CarWheelSize*0.25);
		glPushMatrix();
			glRotatef(-CarWheelAngle,1.0,0.0,0.0);	//wheel rotate
			glPushMatrix();
				glScalef(0.5, 1.0, 1.0);
				glutSolidSphere(CarWheelSize, 15.0, 15.0);
			glPopMatrix();
			glTranslatef(0,CarWheelSize,0);
			glColor3ub(0,100,0);	//Little Green dot
			glutSolidSphere(CarWheelSize*0.1, 5.0, 5.0);
		glPopMatrix();
	glPopMatrix();
	
	//Right wheel(2)
	glPushMatrix();
		glColor3ub(150,100,0);
		glTranslatef(CarBodySize*0.5+CarWheelSize*0.5, 0.0, CarBodySize*0.5-CarWheelSize*0.25);
		glPushMatrix();
			glRotatef(-CarWheelAngle,1.0,0.0,0.0);	//wheel rotate
			glPushMatrix();
				glScalef(0.5, 1.0, 1.0);
				glutSolidSphere(CarWheelSize, 15.0, 15.0);
			glPopMatrix();
			glTranslatef(0,CarWheelSize,0);
			glColor3ub(0,100,0);	//Little Green dot
			glutSolidSphere(CarWheelSize*0.1, 5.0, 5.0);
		glPopMatrix();
	glPopMatrix();

	//Left wheel(1)
	glPushMatrix();
	    glColor3ub(150, 100, 0);
	    glTranslatef(-CarBodySize*0.5 - CarWheelSize*0.5, 0.0, -CarBodySize*0.25 - CarWheelSize*0.25);
	    glPushMatrix();
			glRotatef(-CarWheelAngle,1.0,0.0,0.0);	//wheel rotate
			glPushMatrix();
				glScalef(0.5, 1.0, 1.0);
				glutSolidSphere(CarWheelSize, 15.0, 15.0);
			glPopMatrix();
			glTranslatef(0,CarWheelSize,0);
			glColor3ub(0,100,0);	//Little Green dot
			glutSolidSphere(CarWheelSize*0.1, 5.0, 5.0);
	    glPopMatrix();
	glPopMatrix();

	//Left wheel(2)
	glPushMatrix();
	    glColor3ub(150, 100, 0);
	    glTranslatef(-CarBodySize*0.5 - CarWheelSize*0.5, 0.0, CarBodySize*0.5 - CarWheelSize*0.25);
	    glPushMatrix();
			glRotatef(-CarWheelAngle,1.0,0.0,0.0);	//wheel rotate
			glPushMatrix();
				glScalef(0.5, 1.0, 1.0);
				glutSolidSphere(CarWheelSize, 15.0, 15.0);
			glPopMatrix();
			glTranslatef(0,CarWheelSize,0);
			glColor3ub(0,100,0);	//Little Green dot
			glutSolidSphere(CarWheelSize*0.1, 5.0, 5.0);
	    glPopMatrix();
	glPopMatrix();
}

void display(void)
{
	glEnable(GL_COLOR_MATERIAL);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();
	glRotatef(CameraSRotX, 1.0, 0.0, 0.0); //攝影機自轉
	glRotatef(CameraSRotY, 0.0, 1.0, 0.0); //
	gluLookAt(0.0+CamShiftLR, 75.0-CamShiftFB, 50.0-CamShiftFB, 0.0+CamShiftLR, 0.0, 0.0, 0.0, 1.0, 0.0);
	glRotatef(CameraTRotX, 1.0, 0.0, 0.0); //旋轉物體
	glRotatef(CameraTRotY, 0.0, 1.0, 0.0); //

	
	DrawAxis();
	DrawCar();

	glFlush();
	glutSwapBuffers();
}

void myMotion(int _value)
{
	if(MotionPlay == true)
	{
		CarWheelAngle+=2;
		CarBodyAngle--;

		glutPostRedisplay();
		glutTimerFunc(10, myMotion, 1);
	}
}

float step = 10.0f;
void keys(unsigned char key, int x, int y)
{

   if(key == 'p') //MotionPlay = MotionPlay ? false : true;
   		if(MotionPlay == false){
			MotionPlay = true;
			glutTimerFunc(10, myMotion, 1);
		}
		else
			MotionPlay = false;

   if(key == 'w')
		CamShiftFB += step;
   if(key == 's')
		CamShiftFB -= step;
   if(key == 'd')
		CamShiftLR += step;
   if(key == 'a')
		CamShiftLR -= step;

   display();
}

void myMouseMotion(int x, int y)
{
	if(ActiveMouse){
		int dx, dy;
		dx = x - lastPosX;
		dy = y - lastPosY;
		if(Ctrl_actived){
			CameraSRotY += dx*ScaleX;
			CameraSRotX += dy*ScaleY;
		}
		else{
			CameraTRotY += dx*ScaleX;
			CameraTRotX += dy*ScaleY;
		}
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
		//y=winHeight-y;
		lastPosX = x;
		lastPosY = y;
		ActiveMouse = true;
		break;
	case GLUT_UP:
		ActiveMouse = false;
		break;
	} 

	if(GLUT_ACTIVE_CTRL == glutGetModifiers())
		Ctrl_actived = true;
	else
		Ctrl_actived = false;
}

void myReshape(int w, int h)
{
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION); 
		glLoadIdentity();
		gluPerspective(45.0f,(float)w/(float)h, 0.1 ,500.0f);

	ScaleX = 180.0f / w;
	ScaleY = 180.0f / h;

	glMatrixMode(GL_MODELVIEW);
}

void initLight(void)
{
	GLfloat  ambientLight[] = { 0.6f, 0.6f, 0.6f, 1.0f };
	GLfloat  diffuseLight[] = { 0.6f, 0.6f, 0.6f, 1.0f };
	GLfloat  specular[] = { 0.5f, 0.5f, 0.5f, 1.0f};
	GLfloat  Lposition[] = {0.0f, 100.0f, 100.0f, 1.0f};
    
	glEnable(GL_LIGHTING);
	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);
	
	glLightfv(GL_LIGHT0,GL_AMBIENT,ambientLight);
	glLightfv(GL_LIGHT0,GL_DIFFUSE,diffuseLight);
	glLightfv(GL_LIGHT0,GL_SPECULAR,specular);
	glLightfv(GL_LIGHT0,GL_POSITION,Lposition);
	glEnable(GL_LIGHT0);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_NORMALIZE);

	glClearColor(0.5,0.5,0.5,1.0);
}

void
main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(500, 500);
	glutCreateWindow("racing car");
	
	initLight();
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	glutReshapeFunc(myReshape);
	glutDisplayFunc(display);
	glutKeyboardFunc(keys);
	
	glutMouseFunc(myMouseButton);
    glutMotionFunc(myMouseMotion);
	
	glutMainLoop();
}
