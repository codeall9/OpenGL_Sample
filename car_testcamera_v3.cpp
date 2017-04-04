#include <stdlib.h>
#include <GL/glut.h>

GLfloat CarBodySize = 10.0;
GLfloat CarWheelSize = 3.5;

GLfloat CarWheelAngle = 0.0;
GLfloat CarBodyAngle = 0.0;

bool MotionPlay = false;

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
	//glTranslatef(20.0, 0.0, 0.0);

	glColor3ub(150,0,0);
	glPushMatrix();
	glScalef(1.0, 0.5, 1.0);
		glutSolidCube(CarBodySize);
	glPopMatrix();

	glPushMatrix(); //記錄世界座標系

	//4個車輪之一
	//---------------------------------
	glPushMatrix(); //記錄車身的座標系
		glColor3ub(150,100,0);
		glTranslatef(CarBodySize*0.5+CarWheelSize*0.5, 0.0, CarBodySize*0.5-CarWheelSize*0.25);
		glPushMatrix();
			glRotatef(CarBodyAngle,1.0,0.0,0.0);
			glPushMatrix();
				glScalef(0.5, 1.0, 1.0);
					glutSolidSphere(CarWheelSize, 15.0, 15.0);
			glPopMatrix();
			glTranslatef(0,CarWheelSize,0);
			glColor3ub(0,100,0);
				glutSolidSphere(CarWheelSize*0.1, 5.0, 5.0);
		glPopMatrix();
	glPopMatrix();  //跳回車身的座標系

	glPushMatrix(); //記錄車身的座標系
		glColor3ub(150,100,0);
		glTranslatef(CarBodySize*0.5+CarWheelSize*0.5, 0.0, -CarBodySize*0.5+CarWheelSize*0.25);
		glPushMatrix();
			glPushMatrix();
			glScalef(0.5, 1.0, 1.0);
			glRotatef(CarBodyAngle,1.0,0.0,0.0);
				glutSolidSphere(CarWheelSize, 15.0, 15.0);
			glPopMatrix();
			glTranslatef(0,CarWheelSize,0);
			glColor3ub(0,100,0);
				glutSolidSphere(CarWheelSize*0.1, 5.0, 5.0);
		glPopMatrix();
	glPopMatrix();  //跳回車身的座標系

	glPopMatrix();
}

void display(void)
{
	glEnable(GL_COLOR_MATERIAL);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();
	glTranslatef(0.0, 0.0, -50.0);
	//glRotatef(CarWheelAngle, 0.0, 1.0, 0.0); //攝影機反向(CW)自轉
	//gluLookAt(0.0, 50.0, 50.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	
	////gluLookAt(0.0, 0.0, 50.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0); //把攝影機放在Z=50
	//glTranslatef(0.0, 0.0, -50.0); //與把世界位移-50相同

	gluLookAt(0.0, 50.0, 50.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0); //把攝影機放在(0,50,50)
	//glRotatef(45.0, 1.0, 0.0, 0.0); //攝影機旋轉
	//glTranslatef(0.0, -50.0, -50.0);
	
	////gluLookAt(50.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0); //用攝影機看側面
	//glTranslatef(0.0, 0.0, -50.0); //與把世界位移-50 (把攝影機往後移50)
	//glRotatef(-90, 0.0, 1.0, 0.0); //世界自轉 (攝影機繞著原點反向-CW旋轉)

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

void keys(unsigned char key, int x, int y)
{

   if(key == 'p') //MotionPlay = MotionPlay ? false : true;
   		if(MotionPlay == false){
			MotionPlay = true;
			glutTimerFunc(10, myMotion, 1);
		}
		else
			MotionPlay = false;

   display();
}

void myReshape(int w, int h)
{
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION); 
		glLoadIdentity();
		gluPerspective(45.0f,(float)w/(float)h, 0.1 ,500.0f);

	glMatrixMode(GL_MODELVIEW);
}

void initLight(void)
{
	GLfloat  ambientLight[] = { 0.6f, 0.6f, 0.6f, 1.0f };
	GLfloat  diffuseLight[] = { 0.6f, 0.6f, 0.6f, 1.0f };
	GLfloat  specular[] = { 0.5f, 0.5f, 0.5f, 1.0f};
	GLfloat  Lposition[] = {0.0f, 100.0f, 0.0f, 1.0f}; //set to point light source
    
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
	
	glutMainLoop();
}
