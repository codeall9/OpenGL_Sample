#include <stdio.h>
#include <stdlib.h>
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

int lastPosX = -1, lastPosY = -1;
GLfloat ScaleX = 1.0f, ScaleY = 1.0f;
GLfloat CamShiftFB = 0.0f,CamShiftLR = 0.0f;
bool ActiveMouse = false;
bool ColorTex = true;

void DrawAxis(void){ // a little tool for drawing local axis
	glBegin(GL_LINES);
	glColor3ub(255,0,0); //X-Axis (Red)
	glVertex3f(0,0,0); glVertex3f(30,0,0);
	glColor3ub(0,255,0); //Y-Axis (Green)
	glVertex3f(0,0,0); glVertex3f(0,30,0);
	glColor3ub(0,0,255); //Z-Axis (Blue)
	glVertex3f(0,0,0); glVertex3f(0,0,30);
	glEnd();
}

std::vector<vec3> vertices;
std::vector<vec2> uvs; // Won't be used at the moment.
std::vector<vec3> normals;

void UniObj(void){
	float ux = FLT_MIN, uy = FLT_MIN, uz = FLT_MIN;
	float lx = FLT_MAX, ly = FLT_MAX, lz = FLT_MAX;
	for(unsigned int i = 0; i < vertices.size(); i++){	
		if(vertices[i].x > ux) ux = vertices[i].x;
		if(vertices[i].y > uy) uy = vertices[i].y;
		if(vertices[i].z > uz) uz = vertices[i].z;

		if(vertices[i].x < lx) lx = vertices[i].x;
		if(vertices[i].y < ly) ly = vertices[i].y;
		if(vertices[i].z < lz) lz = vertices[i].z;
	}
	float sx = abs(ux-lx), sy = abs(uy-ly), sz = abs(uz-lz);
	float cx = (ux+lx)/2.0f, cy = (uy+ly)/2.0f, cz = (uz+lz)/2.0f;
	float scale = MAX(sx, MAX(sy,sz));
	for(unsigned int i = 0; i < vertices.size(); i++){
		vertices[i].x = (vertices[i].x - cx)/scale;
		vertices[i].y = (vertices[i].y - cy)/scale;
		vertices[i].z = (vertices[i].z - cz)/scale;
	}
}

GLuint texGen(int mask = 0x8)
{
	// Create one OpenGL texture
	GLuint tmp_textureID;
	glGenTextures(1, &tmp_textureID);
	
	// "Bind" the newly created texture : all future texture functions will modify this texture
	glBindTexture(GL_TEXTURE_2D, tmp_textureID);

	// Generate texture
	GLubyte image[64][64][3];
	int i, j,  c;
	for(i=0;i<64;i++)
	{
		for(j=0;j<64;j++)
		{
			// 0x8 means 1000, ^ means XOR (only 1^0 and 0^1 = 1)
			c = ((((i&mask)==0)^((j&mask))==0))*255;
			image[i][j][0]= (GLubyte) c;
			image[i][j][1]= (GLubyte) c;
			image[i][j][2]= (GLubyte) c;
		}
	}
	
	// load texture
	glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,64,64,0,GL_RGB,GL_UNSIGNED_BYTE, image);
	
	// Basic texture filtering
	glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);

	return tmp_textureID;
}

GLuint textureID = 0;
GLuint textureID_S = 0;
void loadModel(){
	// Read an .obj file
	if(loadOBJ(".\\data\\cube.obj", vertices, uvs, normals))	
	//if(loadOBJ(".\\data\\cat2.obj", vertices, uvs, normals))	
		UniObj();
	
	//textureID = texGen(0x4);
	textureID = loadBMP_custom(".\\data\\cube.bmp");
	textureID_S = loadBMP_custom(".\\data\\cube_s.bmp");
	//textureID_S = textureID = loadBMP_custom(".\\data\\cat2.bmp");	
}

void DrawObj(void){
	glBegin(GL_TRIANGLES);
	for(unsigned int i = 0; i < vertices.size(); i++){	
		glNormal3f(normals[i].x,normals[i].y,normals[i].z);
		glTexCoord2f(uvs[i].x, uvs[i].y);
		glVertex3f(vertices[i].x,vertices[i].y,vertices[i].z);
	}
	glEnd();
}


void myDisplay(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0.0, 0.0, 75.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	glRotatef(CameraTRotX, 1.0, 0.0, 0.0);
	glRotatef(CameraTRotY, 0.0, 1.0, 0.0);
		
	glDisable(GL_LIGHTING);
	DrawAxis();	
	glEnable(GL_LIGHTING);
		
	glScalef(30.0f, 30.0f, 30.0f);
	glRotatef(BodyAngle, 0.0f, 1.0f, 0.0f);

	//*Parameter of openGL Materials
	GLfloat zero[] = {0.0, 0.0, 0.0, 1.0};
	GLfloat ambient[] = {0.5, 0.5, 0.5, 1.0};
	GLfloat diffuse[] = {116/150.0, 40/150.0, 148/150.0, 1.0};
	GLfloat specular[] = {1.0, 1.0, 1.0, 1.0};
	GLfloat shine = 100.0; //alpha of specular
	
	//Draw SOLID Model
	glPolygonMode(GL_FRONT, GL_FILL);
	//glPolygonMode(GL_BACK, GL_FILL);
	glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
	glMaterialf(GL_FRONT, GL_SHININESS, shine);
	
	glEnable(GL_TEXTURE_2D);

	if(ColorTex)
		glBindTexture(GL_TEXTURE_2D, textureID);
	else
		glBindTexture(GL_TEXTURE_2D, textureID_S);
	
	DrawObj();

	glBindTexture(GL_TEXTURE_2D, 0);
	
	glutSwapBuffers();
}

float step = 10.0f;
void myKeys(unsigned char key, int x, int y)
{
}

void myMouseMotion(int x, int y)
{
	if(ActiveMouse){
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
	if(button==GLUT_LEFT_BUTTON) switch(state) 
	{
	case GLUT_DOWN:
		lastPosX = x;
		lastPosY = y;
		ActiveMouse = true;
		ColorTex = false;
		glutPostRedisplay();
		break;
	case GLUT_UP:
		ActiveMouse = false;
		ColorTex = true;
		glutPostRedisplay();
		break;
	} 
}

void initLight(void)
{    
	GLfloat Lambient[] = {0.3, 0.3, 0.3, 1.0};
	GLfloat Ldiffuse[] = {0.7, 0.7, 0.7, 1.0};
	GLfloat Lspecular[] = {1.0, 1.0, 1.0, 1.0};
	GLfloat Lposition[] = {100.0f, 100.0f, 100.0f, 1.0f};

	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);
	glLightfv(GL_LIGHT0,GL_AMBIENT,Lambient);
	glLightfv(GL_LIGHT0,GL_DIFFUSE,Ldiffuse);
	glLightfv(GL_LIGHT0,GL_SPECULAR,Lspecular);
	glLightfv(GL_LIGHT0,GL_POSITION,Lposition);
	glEnable(GL_LIGHT0);

	glEnable(GL_NORMALIZE);

	glClearColor(0.2,0.2,0.2,1.0);
}

void myReshape(int w, int h)
{
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION); 
		glLoadIdentity();
		gluPerspective(45.0f,(float)w/(float)h, 0.1 ,500.0);

	ScaleX = 180.0f / w;
	ScaleY = 180.0f / h;

	glMatrixMode(GL_MODELVIEW);
}

void main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(500, 500);
	glutCreateWindow("Object Drawer");
	
	loadModel();
	initLight();
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE); //Disable this to show the back faces

	glutReshapeFunc(myReshape);
	glutDisplayFunc(myDisplay);
	glutKeyboardFunc(myKeys);
	
	glutMouseFunc(myMouseButton);
    glutMotionFunc(myMouseMotion);
	
	glutMainLoop();
}
