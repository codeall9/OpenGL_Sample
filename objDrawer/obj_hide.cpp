//#include <stdio.h>
//#include <stdlib.h>
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
bool inSelection = false;

void DrawAxis(void){ // a little tool for drawing local axis
	glBegin(GL_LINES);
	glColor3ub(255, 0, 0); //X-Axis (Red)
	glVertex3f(0, 0, 0); glVertex3f(30, 0, 0);
	glColor3ub(0, 255, 0); //Y-Axis (Green)
	glVertex3f(0, 0, 0); glVertex3f(0, 30, 0);
	glColor3ub(0, 0, 255); //Z-Axis (Blue)
	glVertex3f(0, 0, 0); glVertex3f(0, 0, 30);
	glEnd();
}

std::vector<vec3> vertices;
std::vector<vec2> uvs; // Won't be used at the moment.
std::vector<vec3> normals;
std::vector<int> colors;
int id = 0;

void processhit(GLuint hits,GLuint buffer[])
{
	GLuint i, j, names, *ptr;
	ptr = (GLuint *)buffer;

	printf("-------hits=  %d \n",hits);

	//Version 1, hit anything.
	/*for (i = 0; i < hits; i++)
	{
		names = *ptr; //n
		std::cout << "DepthRanmge-(" << *(ptr + 1)/4294967295.0 << ", " << *(ptr + 2)/4294967295.0 << ")" << std::endl;
		//Depth values (which are in the range [0,1]) are multiplied by 2^32 - 1 , before being placed in the hit record.
		//glDepthRange(): the near plane to 0 and the far plane to 1
		ptr += 3; //skip MinZ and MaxZ

		std::cout << "Hit";
		for (j = 0; j < names; j++)
		{
			std::cout << "-[" << *ptr << "]" << std::endl;
			if(colors[*ptr] == 0) colors[*ptr] = 1;
			else colors[*ptr] = 0;

			ptr++;
		}
	} // End of Version 1
	*/
	//Version 2, hit the nearest triangle
	GLuint nearestID = -1, nearestDistance = 4294967295, currentDist;
	for (i = 0; i < hits; i++)
	{
		names = *ptr; //n==4 in this case
		std::cout << "DepthRanmge-(" << *(ptr + 1)/4294967295.0 << ", " << *(ptr + 2)/4294967295.0 << ")" << std::endl;
		currentDist = (*(ptr+1) + *(ptr+2))/2;
		ptr += 3; //skip MinZ and MaxZ
		std::cout << "Hit" << "-[" << *ptr << "]" << std::endl;
		if(currentDist < nearestDistance)
		{
			nearestDistance = currentDist;
			nearestID = *ptr;
		}
		ptr++;
	}
	if(nearestID != -1){
		if (colors[nearestID] == 0) {
			colors[nearestID] = 1;
		} else {
			colors[nearestID] = 0;
		}
		std::cout << "Current Hit" << "-[" << nearestID << "] (the nearest)" << std::endl;
	}
	else
		std::cout << "Miss" << std::endl;
	 // End of Version 2 -------------------
}


void UniObj(void){
	float ux = FLT_MIN, uy = FLT_MIN, uz = FLT_MIN;
	float lx = FLT_MAX, ly = FLT_MAX, lz = FLT_MAX;
	for (unsigned int i = 0; i < vertices.size(); i++){
		if (vertices[i].x > ux) ux = vertices[i].x;
		if (vertices[i].y > uy) uy = vertices[i].y;
		if (vertices[i].z > uz) uz = vertices[i].z;

		if (vertices[i].x < lx) lx = vertices[i].x;
		if (vertices[i].y < ly) ly = vertices[i].y;
		if (vertices[i].z < lz) lz = vertices[i].z;
	}
	float sx = abs(ux - lx), sy = abs(uy - ly), sz = abs(uz - lz);
	float cx = (ux + lx) / 2.0f, cy = (uy + ly) / 2.0f, cz = (uz + lz) / 2.0f;
	float scale = MAX(sx, MAX(sy, sz));
	for (unsigned int i = 0; i < vertices.size(); i++){
		vertices[i].x = (vertices[i].x - cx) / scale;
		vertices[i].y = (vertices[i].y - cy) / scale;
		vertices[i].z = (vertices[i].z - cz) / scale;
	}
}

void loadModel(){
	// Read an .obj file
	//if (loadOBJ(".\\data\\cube.obj", vertices, uvs, normals))
	//if(loadOBJ(".\\data\\3color-sphere.obj", vertices, uvs, normals))
	if(loadOBJ(".\\data\\icosphere.obj", vertices, uvs, normals)){	
		UniObj();
		for (int i = 0; i < vertices.size(); i += 3) {
			if (i % 2 == 0) {
				colors.push_back(0);
			} else {
				colors.push_back(1);
			}
			//colors.push_back(0);
		}
	}
}

void DrawObj(void){	
	for (unsigned int i = 0; i < vertices.size(); i+=3){
		if(inSelection) glPushName(i/3);

		//glEnable(GL_POLYGON_OFFSET_FILL); //https://www.opengl.org/sdk/docs/man2/xhtml/glPolygonOffset.xml
		//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); //https://www.opengl.org/sdk/docs/man3/xhtml/glPolygonMode.xml
		//glPolygonOffset( 1.0f, 1.0f); //http://www.felixgers.de/teaching/jogl/polygonOffset.html
		glBegin(GL_TRIANGLES);
			if (colors[(i / 3)] == 0) {
				glColor3ub(116, 40, 148);

				glNormal3f(normals[i].x, normals[i].y, normals[i].z);
				glVertex3f(vertices[i].x, vertices[i].y, vertices[i].z);

				glNormal3f(normals[i + 1].x, normals[i + 1].y, normals[i + 1].z);
				glVertex3f(vertices[i + 1].x, vertices[i + 1].y, vertices[i + 1].z);

				glNormal3f(normals[i + 2].x, normals[i + 2].y, normals[i + 2].z);
				glVertex3f(vertices[i + 2].x, vertices[i + 2].y, vertices[i + 2].z);
			}
		glEnd();
		//glDisable( GL_POLYGON_OFFSET_FILL );  

		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		//glPushAttrib(GL_LINE_BIT); //https://www.opengl.org/sdk/docs/man2/xhtml/glPushAttrib.xml
		//glLineWidth(1.0f);
		//glBegin(GL_TRIANGLES);
		//	glColor3f(0.0,0.0,0.0);
		//	glNormal3f(normals[i].x, normals[i].y, normals[i].z);
		//	glVertex3f(vertices[i].x, vertices[i].y, vertices[i].z);

		//	glNormal3f(normals[i + 1].x, normals[i + 1].y, normals[i + 1].z);
		//	glVertex3f(vertices[i + 1].x, vertices[i + 1].y, vertices[i + 1].z);

		//	glNormal3f(normals[i + 2].x, normals[i + 2].y, normals[i + 2].z);
		//	glVertex3f(vertices[i + 2].x, vertices[i + 2].y, vertices[i + 2].z);
		//glEnd();
		//
		//glLineWidth(1.0f);
		//glPopAttrib();

		if(inSelection) glPopName();
	}
}

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(50.0, 50.0, 50.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	glRotatef(CameraTRotX, 1.0, 0.0, 0.0); //±ÛÂàª«Åé
	glRotatef(CameraTRotY, 0.0, 1.0, 0.0); //

	DrawAxis();

	glEnable(GL_LIGHTING);
	glShadeModel(GL_SMOOTH); //GL_FLAT

	//glPolygonMode(GL_FRONT, GL_LINE);
	glPolygonMode(GL_BACK, GL_LINE);
	glPolygonMode(GL_FRONT, GL_FILL);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	//*GL_COLOR_MATERIAL
	glEnable(GL_COLOR_MATERIAL);
	//glColor3ub(116, 40, 148); //VS2012 main color

	//*back-facing polygons are lighted using the back material parameters, and have their normals reversed 
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE); //default: GL_FALSE(0)

	//*Use openGL Materials
	GLfloat zero[] = { 0.0, 0.0, 0.0, 1.0 };
	GLfloat ambient[] = { 0.1, 0.1, 0.1, 1.0 };
	GLfloat diffuse[] = { 0.455, 0.157, 0.580 };
	GLfloat specular[] = { 1.0, 0.078, 0.576, 1.0 };
	GLfloat shine = 200.0; //alpha of specular	
	GLfloat bspecular[] = { 0.0, 1.0, 1.0, 1.0 };
	GLfloat bshine = 5.0; //alpha of specular	
						  
	//glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, zero); //< clean all back face's default materials 
	//glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, diffuse); // = glColor3ub(116, 40, 148);
	glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
	glMaterialf(GL_FRONT, GL_SHININESS, shine);

	glMaterialfv(GL_BACK, GL_SPECULAR, bspecular);
	glMaterialf(GL_BACK, GL_SHININESS, bshine);
	//glMaterialfv(GL_BACK, GL_AMBIENT, ambient); //< The default ambient reflectance for both front-facing and back-facing materials is (0.2, 0.2, 0.2, 1.0).
	//glMaterialfv(GL_BACK, GL_DIFFUSE, diffuse); //< The default diffuse reflectance for both front-facing and back-facing materials is (0.8, 0.8, 0.8, 1.0).

	glScalef(40.0f, 40.0f, 40.0f);
	glRotatef(BodyAngle, 0.0f, 1.0f, 0.0f);
	DrawObj();
	glDisable(GL_LIGHTING);
	//glDisable(GL_COLOR_MATERIAL);
	if(inSelection)
		glFlush();
	else
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
	GLint viewport[4];
	GLuint selectbuf[SIZE];
	GLuint hits;

	if (button == GLUT_RIGHT_BUTTON && state==GLUT_DOWN)
	{
		glGetIntegerv(GL_VIEWPORT,viewport);

		glSelectBuffer(SIZE,selectbuf);
		glRenderMode(GL_SELECT);

		glInitNames();

		glMatrixMode(GL_PROJECTION);
		glPushMatrix();
			glLoadIdentity();
			gluPickMatrix((GLdouble) x,(GLdouble)(viewport[3]-y), 0.5, 0.5, viewport);
			gluPerspective(45.0,(GLdouble) viewport[2]/(GLdouble)viewport[3], 1.0f, 500.0f);
			glMatrixMode(GL_MODELVIEW);
		
			inSelection = true;
			display();
			inSelection = false;

			glMatrixMode(GL_PROJECTION);
		glPopMatrix();

		hits = glRenderMode(GL_RENDER);
		processhit(hits,selectbuf);

		glutPostRedisplay();

	}
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
	GLfloat  Lposition[] = { 100.0f, 100.0f, 100.0f, 1.0f };

	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
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
	gluPerspective(45.0f, (float)w / (float)h, 1.0f, 500.0f);

	ScaleX = 180.0f / w;
	ScaleY = 180.0f / h;

	glMatrixMode(GL_MODELVIEW);
	initLight();
}

void myMenu(int id)
{
    if(id == 1) {
		for(int i = 0; i < colors.size(); ++i)
			colors[i] = 0;
		glutPostRedisplay();
	}
    if(id == 2) exit(0);
}


int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(600, 600);
	glutCreateWindow("Object Drawer+Picker");

	loadModel();
	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_CULL_FACE); //Disable this to show the back faces (and hit the back face)
	//glCullFace(GL_FRONT);
	//glCullFace(GL_BACK); //default state

	glutReshapeFunc(myReshape);
	glutDisplayFunc(display);
	glutKeyboardFunc(keys);

	glutMouseFunc(myMouseButton);
	glutMotionFunc(myMouseMotion);

	int menu_id = glutCreateMenu(myMenu);
	glutAddMenuEntry("clear selections", 1);
	glutAddMenuEntry("exit", 2);
	glutAttachMenu(GLUT_MIDDLE_BUTTON);

	glutMainLoop();
}