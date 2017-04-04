#include<GL/glut.h>
#include<ctime>
#include<iostream>

int winID;

GLfloat *mVertices;

GLfloat vertices[][3] = { { -1.0,-1.0,-1.0 },{ 1.0,-1.0,-1.0 },
{ 1.0,1.0,-1.0 },{ -1.0,1.0,-1.0 },{ -1.0,-1.0,1.0 },
{ 1.0,-1.0,1.0 },{ 1.0,1.0,1.0 },{ -1.0,1.0,1.0 } };

GLfloat normals[][3] = { { -1.0,-1.0,-1.0 },{ 1.0,-1.0,-1.0 },
{ 1.0,1.0,-1.0 },{ -1.0,1.0,-1.0 },{ -1.0,-1.0,1.0 },
{ 1.0,-1.0,1.0 },{ 1.0,1.0,1.0 },{ -1.0,1.0,1.0 } };

GLfloat colors[][3] = { { 1.0,1.0,1.0 },{ 1.0,0.0,0.0 },
{ 1.0,1.0,0.0 },{ 0.0,1.0,0.0 },{ 0.0,0.0,1.0 },
{ 1.0,0.0,1.0 },{ 0.5,0.5,0.5 },{ 0.0,1.0,1.0 } };

GLubyte cubeIndices[24] = { 3,2,1,0, 2,3,7,6, 0,4,7,3, 1,2,6,5, 4,5,6,7, 0,1,5,4 };

unsigned int N = 1, vSize = 96;
float theta = 0.5;

void divide_cude(GLfloat *v, GLfloat *outV)
{
	int j;
	for (j = 0; j < 3; j++) outV[0+j] = (v[0+j] + v[3*3+j]) / 2;
	for (j = 0; j < 3; j++) outV[2*3+j] = (v[3*3+j] + v[2*3+j]) / 2;
	for (j = 0; j < 3; j++) outV[3*3+j] = v[3*3+j];
	for (j = 0; j < 3; j++) outV[7*3+j] = (v[7*3+j] + v[3*3+j]) / 2;
	for (j = 0; j < 3; j++) outV[8*3+j] = (v[0*3+j] + v[4*3+j]) / 2;
	for (j = 0; j < 3; j++) outV[12*3+j] = v[4*3+j];
	for (j = 0; j < 3; j++) outV[13*3+j] = (v[4*3+j] + v[5*3+j]) / 2;
	for (j = 0; j < 3; j++) outV[15*3+j] = (v[4*3+j] + v[7*3+j]) / 2;
	for (j = 0; j < 3; j++) outV[16*3+j] = (v[0*3+j] + v[1*3+j]) / 2;
	for (j = 0; j < 3; j++) outV[17*3+j] = v[1*3+j];
	for (j = 0; j < 3; j++) outV[18*3+j] = (v[2*3+j] + v[1*3+j]) / 2;
	for (j = 0; j < 3; j++) outV[21*3+j] = (v[5*3+j] + v[1*3+j]) / 2;
	for (j = 0; j < 3; j++) outV[26*3+j] = (v[6*3+j] + v[2*3+j]) / 2;
	for (j = 0; j < 3; j++) outV[29*3+j] = (v[6*3+j] + v[5*3+j]) / 2;
	for (j = 0; j < 3; j++) outV[30*3+j] = v[6*3+j];
	for (j = 0; j < 3; j++) outV[31*3+j] = (v[7*3+j] + v[6*3+j]) / 2;

	for (j = 0; j < 3; j++) outV[1*3+j] = (outV[2*3+j] + outV[16*3+j]) / 2;
	for (j = 0; j < 3; j++) outV[19*3+j] = outV[1*3+j];
	for (j = 0; j < 3; j++) outV[4*3+j] = (outV[15*3+j] + outV[0*3+j]) / 2;
	for (j = 0; j < 3; j++) outV[11*3+j] = outV[4*3+j];
	for (j = 0; j < 3; j++) outV[6*3+j] = (outV[7*3+j] + outV[26*3+j]) / 2;
	for (j = 0; j < 3; j++) outV[27*3+j] = outV[6*3+j];
	for (j = 0; j < 3; j++) outV[9*3+j] = (outV[16*3+j] + outV[13*3+j]) / 2;
	for (j = 0; j < 3; j++) outV[20*3+j] = outV[9*3+j];
	for (j = 0; j < 3; j++) outV[5*3+j] = (outV[6*3+j] + outV[9*3+j]) / 2;
	for (j = 0; j < 3; j++) outV[10*3+j] = outV[5*3+j];
	for (j = 0; j < 3; j++) outV[23*3+j] = outV[5*3+j];
	for (j = 0; j < 3; j++) outV[24*3+j] = outV[5*3+j];
	for (j = 0; j < 3; j++) outV[14*3+j] = (outV[15*3+j] + outV[29*3+j]) / 2;
	for (j = 0; j < 3; j++) outV[28*3+j] = outV[14*3+j];
	for (j = 0; j < 3; j++) outV[22*3+j] = (outV[29*3+j] + outV[18*3+j]) / 2;
	for (j = 0; j < 3; j++) outV[25*3+j] = outV[22*3+j];
}

void mDisplay()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glRotatef(100, 1.0, 1.0, -1.0);

	glEnableClientState(GL_VERTEX_ARRAY); 
	glEnableClientState(GL_COLOR_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY); 

	for (unsigned int i = 0; i < N; i++) {
		glVertexPointer(3, GL_FLOAT, 0, &mVertices[i*24]);
		glNormalPointer(GL_FLOAT, 0, &mVertices[i * 24]);
		glDrawElements(GL_QUADS, 24, GL_UNSIGNED_BYTE, cubeIndices);
	}

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);

	glFlush();
	glutSwapBuffers();
}

void mReshape(GLsizei w, GLsizei h)
{
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	if (w <= h)
		glOrtho(-2.0, 2.0, -2.0 * (GLfloat)h / (GLfloat)w,
			2.0 * (GLfloat)h / (GLfloat)w, -10.0, 10.0);
	else
		glOrtho(-2.0 * (GLfloat)w / (GLfloat)h,
			2.0 * (GLfloat)w / (GLfloat)h, -2.0, 2.0, -10.0, 10.0);
	glMatrixMode(GL_MODELVIEW);
	glutPostRedisplay();
}

void mMouse(int btn, int state, int x, int y)
{
	if (btn == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		GLfloat *newVertices = new GLfloat[vSize];
		for (unsigned int i = 0; i < N; i++) {
			divide_cude(&mVertices[i*24], &newVertices[i*96]);
		}
		delete[] mVertices;
		mVertices = newVertices;
		N *= 4;
		vSize *= 4;
		std::cout << vSize << std::endl;
	}
}

void mSpin()
{
	clock_t now = clock(); //need time.h
	static clock_t lastTime = 0;
	if ((now - lastTime) / (float)CLOCKS_PER_SEC > (1.0f / 60.0f)) { //<60fps
																	 /* Idle callback, spin cube 2 degrees about selected axis */
		theta += 1;
		if (theta > 360.0) theta -= 360.0;
		glutPostRedisplay();

		// Record current time for next time:
		lastTime = now;
	}
}

void main(int argc, char *argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(500, 500);

	mVertices = new GLfloat[24];
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 3; j++) {
			mVertices[i * 3 + j] = vertices[i][j];
		}
	}

	winID = glutCreateWindow("CG");
	glutReshapeFunc(mReshape);
	glutDisplayFunc(mDisplay);
	glutIdleFunc(mSpin);
	glutMouseFunc(mMouse);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glShadeModel(GL_FLAT);

	glVertexPointer(3, GL_FLOAT, 0, mVertices);
	glColorPointer(3, GL_FLOAT, 0, colors);
	glNormalPointer(GL_FLOAT, 0, normals);

	glutMainLoop();

	delete[] mVertices;
}