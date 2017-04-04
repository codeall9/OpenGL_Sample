#include <iostream>
#include <time.h>
#include <vector>
#include <cmath>
#include <climits>
#include <cfloat>

#include <GL/glut.h>
#include "objloader_bmp.hpp"
#include "texture.hpp"

#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define SIZE 512
#define OUTPUT false

GLfloat BodyAngle = 0.0;
GLfloat CameraTRotX = 0.0, CameraTRotY = 0.0;
float theta = 0.0f;

int lastPosX = -1, lastPosY = -1;
GLfloat ScaleX = 1.0f, ScaleY = 1.0f;
GLfloat CamShiftFB = 0.0f, CamShiftLR = 0.0f;
bool ActiveMouse = false;
bool inSelection = false;
bool MotionPlay = false;

void DrawAxis(void) { // a little tool for drawing local axis
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
std::vector<vec2> uvs;
std::vector<vec3> normals;
vec3 mVec[8] = { { -1.0,-1.0,-1.0 },{ 1.0,-1.0,-1.0 },
{ 1.0,1.0,-1.0 },{ -1.0,1.0,-1.0 },{ -1.0,-1.0,1.0 },
{ 1.0,-1.0,1.0 },{ 1.0,1.0,1.0 },{ -1.0,1.0,1.0 } };

vec3 mNormal[8] = { { -1.0,-1.0,-1.0 },{ 1.0,-1.0,-1.0 },
{ 1.0,1.0,-1.0 },{ -1.0,1.0,-1.0 },{ -1.0,-1.0,1.0 },
{ 1.0,-1.0,1.0 },{ -1.0,1.0,1.0 },{ -1.0,1.0,1.0 } };

vec2 mUV[4] = { { 0.0, 0.0 },{ 1.0, 0.0 },{ 1.0, 1.0 },{ 0.0, 1.0 } };

GLfloat mColors[8][3] = { { 1.0,0.0,1.0 },{ 1.0,0.0,0.0 },
{ 1.0,1.0,0.0 },{ 0.0,0.0,1.0 },{ 0.0,1.0,0.0 },{ 0.0,1.0,1.0 },{ 0.5,0.5,0.5 },{ 1.0,1.0,1.0 } };

							//back		up		left	right	front		bottom
GLubyte cubeIndices[24] = { 3,2,1,0, 2,3,7,6, 0,4,7,3, 1,2,6,5, 4,5,6,7, 0,1,5,4 };
int diceIndex[4] = { 4,4,4,4 };

void UniObj(void) {
	float ux = FLT_MIN, uy = FLT_MIN, uz = FLT_MIN;
	float lx = FLT_MAX, ly = FLT_MAX, lz = FLT_MAX;
	for (unsigned int i = 0; i < vertices.size(); i++) {
		if (vertices[i].x > ux) ux = vertices[i].x;
		if (vertices[i].y > uy) uy = vertices[i].y;
		if (vertices[i].z > uz) uz = vertices[i].z;

		if (vertices[i].x < lx) lx = vertices[i].x;
		if (vertices[i].y < ly) ly = vertices[i].y;
		if (vertices[i].z < lz) lz = vertices[i].z;
	}
	float sx = abs(ux - lx), sy = abs(uy - ly), sz = abs(uz - lz);
	float cx = (ux + lx) / 2.0f, cy = (uy + ly) / 2.0f, cz = (uz + lz) / 2.0f;
	float scale = MAX(sx, MAX(sy, sz)) * 2;
	for (unsigned int i = 0; i < vertices.size(); i++) {
		vertices[i].x = (vertices[i].x - cx) / scale;
		vertices[i].y = (vertices[i].y - cy) / scale;
		vertices[i].z = (vertices[i].z - cz) / scale;
	}
}

GLuint textureID = 0;
GLuint textureID_S = 0;
GLuint tex_DiceID[6] = { 0 };

void mLoad() {
	for (int i = 0; i < 24; i++) {
		vertices.push_back(mVec[cubeIndices[i]]);
		normals.push_back(mNormal[cubeIndices[i]]);
		uvs.push_back(mUV[cubeIndices[i % 4]]);
	}
}

void processhit(GLuint hits, GLuint buffer[])
{
	GLuint i, j, names, *ptr;
	ptr = (GLuint *)buffer;

	printf("-------hits=  %d \n", hits);
	GLuint nearestID = -1, nearestDistance = 4294967295, currentDist;
	for (i = 0; i < hits; i++)
	{
		names = *ptr;
		std::cout << "DepthRanmge-(" << *(ptr + 1) / 4294967295.0 << ", " << *(ptr + 2) / 4294967295.0 << ")" << std::endl;
		currentDist = (*(ptr + 1) + *(ptr + 2)) / 2;
		ptr += 3; //skip MinZ and MaxZ
		std::cout << "Hit" << "-[" << *ptr << "]" << std::endl;
		if (currentDist < nearestDistance)
		{
			nearestDistance = currentDist;
			nearestID = *ptr;
		}
		ptr++;
	}
	if (nearestID != -1) {
		if (diceIndex[nearestID] < 5) {
			diceIndex[nearestID] += 1;
		}
		std::cout << "Current Hit" << "-[" << nearestID << "] (the nearest)" << std::endl;
	}
	else
		std::cout << "Miss" << std::endl;
	// End of Version 2 -------------------
}

void loadModel() {

	mLoad();
	UniObj();

	tex_DiceID[0] = loadBMP_custom(".\\data\\one.bmp");
	tex_DiceID[1] = loadBMP_custom(".\\data\\two.bmp");
	tex_DiceID[2] = loadBMP_custom(".\\data\\three.bmp");
	tex_DiceID[3] = loadBMP_custom(".\\data\\four.bmp");
	tex_DiceID[4] = loadBMP_custom(".\\data\\five.bmp");
	tex_DiceID[5] = loadBMP_custom(".\\data\\six.bmp");
}

void DrawObj(void) {

	if (inSelection) glPushName(0);
	GLfloat tr[4][3] = { { -0.25f, -0.25f, -0.25f },{ 0.25f, 0.25f, -0.25f },{ -0.25f, 0.25f, 0.25f },{ 0.25f, -0.25f, 0.25f } };
	for (int n = 0; n < 4; n++) {
		glBindTexture(GL_TEXTURE_2D, tex_DiceID[diceIndex[n]]);
		if (inSelection) glLoadName(n);
		glPushMatrix();
		glTranslatef(tr[n][0], tr[n][1], tr[n][2]);
		glBegin(GL_QUADS);
		for (unsigned int i = 0; i < vertices.size(); i++) {
			glColor3fv(mColors[i / 4]);
			glNormal3f(normals[i].x,normals[i].y,normals[i].z);
			glTexCoord2f(uvs[i].x, uvs[i].y);
			glVertex3f(vertices[i].x, vertices[i].y, vertices[i].z);
		}
		glEnd();
		glPopMatrix();
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	if (inSelection) glPopName();

}

void myDisplay(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(50.0, 50.0, 50.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	glRotatef(CameraTRotX, 1.0, 0.0, 0.0);
	glRotatef(CameraTRotY, 0.0, 1.0, 0.0);

	DrawAxis();
	glEnable(GL_LIGHTING);
	glEnable(GL_COLOR_MATERIAL);
	glScalef(35.0f, 35.0f, 35.0f);
	glRotatef(theta, 0.0f, 1.0f, 0.0f);

	DrawObj();
	glDisable(GL_LIGHTING);

	if (inSelection)
		glFlush();
	else
		glutSwapBuffers();
}

void spinIt()
{
	if (!MotionPlay)
		return;

	clock_t now = clock();
	static clock_t lastTime1 = 0;
	if ((now - lastTime1) / (float)CLOCKS_PER_SEC > (1.0f / 60.0f)) { //<60fps
																	 /* Idle callback, spin cube 2 degrees about selected axis */
		theta += 0.5;
		if (theta > 360.0) theta -= 360.0;

		glutPostRedisplay();
		// Record current time for next time:
		lastTime1 = now;
	}

	static clock_t lastTime2 = 0;
	if ((now - lastTime2) / (float)CLOCKS_PER_SEC > (1.0f / 1.0f)) {

		for (int i = 0; i < 4; i++) {
			if (diceIndex[i] > 0) {
				diceIndex[i] -= 1;
			}
		}
		glutPostRedisplay();
		lastTime2 = now;
	}
}

void myKeys(unsigned char key, int x, int y)
{
	if (key == 'p' || key == 'P') {
		MotionPlay = !MotionPlay;
	}
}

void myMouseMotion(int x, int y)
{
	if (ActiveMouse) {
		int dx, dy;
		dx = x - lastPosX;
		dy = y - lastPosY;
		CameraTRotY += dx*ScaleX;
		CameraTRotX += dy*ScaleY;
		glutPostRedisplay();
	}
	lastPosX = x;
	lastPosY = y;
}

void myMouseButton(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		GLint viewport[4];
		GLuint selectbuf[SIZE];
		GLuint hits;

		glGetIntegerv(GL_VIEWPORT, viewport);

		glSelectBuffer(SIZE, selectbuf);
		glRenderMode(GL_SELECT);

		glInitNames();

		glMatrixMode(GL_PROJECTION);
		glPushMatrix();
		glLoadIdentity();
		gluPickMatrix((GLdouble)x, (GLdouble)(viewport[3] - y), 0.5, 0.5, viewport);
		gluPerspective(45.0, (GLdouble)viewport[2] / (GLdouble)viewport[3], 1.0f, 500.0f);
		glMatrixMode(GL_MODELVIEW);

		inSelection = true;
		myDisplay();
		inSelection = false;

		glMatrixMode(GL_PROJECTION);
		glPopMatrix();

		hits = glRenderMode(GL_RENDER);
		processhit(hits, selectbuf);

		glutPostRedisplay();

	}/*
	else if (button == GLUT_RIGHT_BUTTON) switch (state)
	{
	case GLUT_DOWN:
		lastPosX = x;
		lastPosY = y;
		ActiveMouse = true;
		glutPostRedisplay();
		break;
	case GLUT_UP:
		ActiveMouse = false;
		glutPostRedisplay();
		break;
	}*/
}

void initLight(void)
{
	GLfloat Lambient[] = { 0.3, 0.3, 0.3, 1.0 };
	GLfloat Ldiffuse[] = { 0.7, 0.7, 0.7, 1.0 };
	GLfloat Lspecular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat Lposition[] = { 100.0f, 50.0f, 50.0f, 1.0f };

	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);
	glLightfv(GL_LIGHT0, GL_AMBIENT, Lambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, Ldiffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, Lspecular);
	glLightfv(GL_LIGHT0, GL_POSITION, Lposition);
	glEnable(GL_LIGHT0);

	glEnable(GL_NORMALIZE);

	glClearColor(1.0, 1.0, 1.0, 1.0);
}

void myReshape(int w, int h)
{
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f, (float)w / (float)h, 0.1, 500.0);

	ScaleX = 180.0f / w;
	ScaleY = 180.0f / h;

	glMatrixMode(GL_MODELVIEW);
}

void main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(500, 500);
	glutCreateWindow("Countdown Cude");

	loadModel();
	initLight();
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE); //Disable this to show the back faces
	glEnable(GL_TEXTURE_2D);

	glutReshapeFunc(myReshape);
	glutDisplayFunc(myDisplay);
	glutKeyboardFunc(myKeys);
	glutIdleFunc(spinIt);

	glutMouseFunc(myMouseButton);
	glutMotionFunc(myMouseMotion);

	glutMainLoop();
}
