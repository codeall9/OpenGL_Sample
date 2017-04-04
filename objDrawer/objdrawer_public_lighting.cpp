//FAQ of OpenGL Lighting
//https://www.opengl.org/archives/resources/faq/technical/lights.htm

#include <iostream>
#include <vector>
#include <cmath>
#include <cfloat>
#include <GL/glut.h>
#include "objloader.hpp"
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define SIZE 512

GLfloat BodyAngle = 0.0; 
GLfloat CameraTRotX = 0.0, CameraTRotY = 0.0;

int lastPosX = -1, lastPosY = -1;
GLfloat ScaleX = 1.0f, ScaleY = 1.0f;
GLfloat CamShiftFB = 0.0f, CamShiftLR = 0.0f;

bool MotionPlay = false;
bool ActiveMouse = false;
bool Ctrl_actived = false;

std::vector<vec3> vertices;
std::vector<vec2> uvs; // Won't be used at the moment.
std::vector<vec3> normals;

void DrawAxis(float length = 30){ // a little tool for drawing local axis
	glBegin(GL_LINES);
	glColor3ub(255, 0, 0); //X-Axis (Red)
	glVertex3f(0, 0, 0); glVertex3f(length, 0, 0);
	glColor3ub(0, 255, 0); //Y-Axis (Green)
	glVertex3f(0, 0, 0); glVertex3f(0, length, 0);
	glColor3ub(0, 0, 255); //Z-Axis (Blue)
	glVertex3f(0, 0, 0); glVertex3f(0, 0, length);
	glEnd();
}

void loadModel(){
	// Read an .obj file
	loadOBJ(".\\data\\icosphere.obj", vertices, uvs, normals);
	glColor3f(0.8, 0.8, 1.0);
}

void DrawObj(void){	
	for (unsigned int i = 0; i < vertices.size(); i+=3){
		glBegin(GL_TRIANGLES);
			//If your application doesn't call glNormal*(), then it uses the default normal of (0.0, 0.0, 1.0) at every vertex
			glNormal3f(normals[i].x, normals[i].y, normals[i].z);
			glVertex3f(vertices[i].x, vertices[i].y, vertices[i].z);

			glNormal3f(normals[i + 1].x, normals[i + 1].y, normals[i + 1].z);
			glVertex3f(vertices[i + 1].x, vertices[i + 1].y, vertices[i + 1].z);

			glNormal3f(normals[i + 2].x, normals[i + 2].y, normals[i + 2].z);
			glVertex3f(vertices[i + 2].x, vertices[i + 2].y, vertices[i + 2].z);
		glEnd();
	}
}

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(100.0, 100.0, 100.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	glRotatef(CameraTRotX, 1.0, 0.0, 0.0); //±ÛÂàª«Åé
	glRotatef(CameraTRotY, 0.0, 1.0, 0.0); //
	
	glEnable(GL_LIGHTING);
	//glShadeModel(GL_FLAT); (note: check the specular effect)
	
	//glPolygonMode(GL_FRONT, GL_LINE);
	//glPolygonMode(GL_BACK, GL_FILL);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	//*GL_COLOR_MATERIAL
	glEnable(GL_COLOR_MATERIAL);
	glColor3ub(116, 40, 148); //VS2012 main color (0.455, 0.157, 0.580)

	//*back-facing polygons are lighted using the back material parameters, and have their normals reversed 
	//glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE); //default: GL_FALSE(0)

	//*Use openGL Materials
	GLfloat zero[] = { 0.0, 0.0, 0.0, 1.0 };
	GLfloat ambient[] = { 0.1, 0.1, 0.1, 1.0 };
	GLfloat diffuse[] = { 0.455, 0.157, 0.580 };
	GLfloat specular[] = { 1.0, 0.0, 0.0, 1.0 };
	GLfloat shine = 100.0; //alpha of specular	
	
	//glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, zero); //< clean all back face's default materials 
	//glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, diffuse); // = glColor3ub(116, 40, 148);
	//glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);
	//glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);
	//glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
	//glMaterialf(GL_FRONT, GL_SHININESS, shine);

	//glMaterialfv(GL_BACK, GL_AMBIENT, ambient); //< The default ambient reflectance for both front-facing and back-facing materials is (0.2, 0.2, 0.2, 1.0).
	//glMaterialfv(GL_BACK, GL_DIFFUSE, diffuse); //< The default diffuse reflectance for both front-facing and back-facing materials is (0.8, 0.8, 0.8, 1.0).
	
	glScalef(40.0f, 40.0f, 40.0f); // require a glEnable(GL_NORMALIZE) to re-normalize the vertex NORMAL
	glRotatef(BodyAngle, 0.0f, 1.0f, 0.0f);
	DrawObj();
	glDisable(GL_LIGHTING);
	DrawAxis(5.0f);

	glutSwapBuffers();//An implicit glFlush is done by glutSwapBuffers before it returns.
}

void myMotion(int _value)
{
	if (MotionPlay == true)
	{
		BodyAngle++;
		glutPostRedisplay();
		glutTimerFunc(_value, myMotion, _value);
	}
}

float step = 10.0f;
void keys(unsigned char key, int x, int y)
{
	if (key == 'p') //MotionPlay = MotionPlay ? false : true;
		if (MotionPlay == false){
			MotionPlay = true;
			glutTimerFunc(1, myMotion, 20);
		}
		else
			MotionPlay = false;

	display();
}

void myMouseMotion(int x, int y)
{
	if (ActiveMouse){
		int dx, dy;
		dx = x - lastPosX;
		dy = y - lastPosY;
		CameraTRotY += dx*ScaleX;
		CameraTRotX += dy*ScaleY;
		glutPostRedisplay();
		lastPosX = x;
		lastPosY = y;
	}
}

void myMouseButton(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON) 
		switch (state)
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
}

void initLight(void)
{
	GLfloat  ambientLight[] = { 0.2f, 0.2f, 0.2f, 1.0f };
	GLfloat  diffuseLight[] = { 0.6f, 0.6f, 0.6f, 1.0f };
	GLfloat  specular[] = { 0.5f, 0.5f, 0.5f, 1.0f };
	GLfloat  Lposition[] = { 100.0f, 100.0f, 0.0f, 1.0f };

	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specular);

	//*The light position is transformed by the contents of the current top of the ModelView matrix stack 
	// when you specify the light position with a call to glLightfv(GL_LIGHT_POSITION,¡K).
	glLightfv(GL_LIGHT0, GL_POSITION, Lposition);
	glEnable(GL_LIGHT0);

	glEnable(GL_NORMALIZE); // normalize the input normal (important step), it requires OpenGL to rescale all of the normal vectors to have a magnitude of 1.

	glClearColor(0.2, 0.2, 0.2, 1.0);
}

void myReshape(int w, int h)
{
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f, (float)w / (float)h, 0.1, 500.0f);

	ScaleX = 180.0f / w;
	ScaleY = 180.0f / h;

	glMatrixMode(GL_MODELVIEW);
	//initLight(); << wrong place (note: the light position may been effected by MODELVIEW MATRIX)
}

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(600, 600);
	glutCreateWindow("Object Drawer+Picker");

	loadModel();
	initLight();
	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_CULL_FACE); //Disable this to show the back faces (and hit the back face)
	//glCullFace(GL_FRONT);
	//glCullFace(GL_BACK); //default state

	glutReshapeFunc(myReshape);
	glutDisplayFunc(display);
	glutKeyboardFunc(keys);

	glutMouseFunc(myMouseButton);
	glutMotionFunc(myMouseMotion);

	glutMainLoop();
}
