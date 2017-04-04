/* recursive subdivision of a tetrahedron to form 3D Sierpinski gasket */
/* number of recursive steps given on command line */

#include <cstdlib>
#include <ctime> //add this for timer
#include <cmath> //abs

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

/* initial tetrahedron */

//GLfloat v[4][3]={{0.0, 0.0, 1.0}, {0.0, 0.942809, -0.33333},
//      {-0.816497, -0.471405, -0.333333}, {0.816497, -0.471405, -0.333333}};
GLfloat v[4][3]={{0.0, 0.0, 1.15}, {-1.0, -0.58, -0.58}, {1.0, -0.58, -0.58}, {0.0, 1.15, -0.58}};
GLfloat vb[8][3]={{-1.0, 1.0, 1.0}, {1.0, 1.0, 1.0}, {1.0, 1.0, -1.0}, {-1.0, 1.0, -1.0},
				  {-1.0, -1.0, 1.0}, {1.0, -1.0, 1.0}, {1.0, -1.0, -1.0}, {-1.0, -1.0, -1.0}};


GLfloat colors[6][3] = {{1.0, 0.0, 0.0}, {0.0, 1.0, 0.0}, //color of triangles
                        {0.0, 0.0, 1.0}, {1.0, 0.0, 1.0},
						{1.0, 1.0, 0.0}, {0.0, 1.0, 1.0}};

GLubyte colorsb[4][3] = {{80, 174, 255}, {116, 225, 120}, {235, 164, 109}, {255, 132, 126}}; //color of boxes

int n = 1;
float theta = 0; // angle of current spinning cube
bool active = false; // active spin function
bool sMode = false; // switch to sphere mode

int NumOfPoints = 24 * 4;

void quad(GLfloat *va, GLfloat *vb, GLfloat *vc, GLfloat *vd)
{
       glVertex3fv(va);
       glVertex3fv(vb);
       glVertex3fv(vc);
       glVertex3fv(vd);
}

void sphere(GLfloat *A, GLfloat *B, GLfloat *C, GLfloat *D, GLfloat *E, GLfloat *F, GLfloat *G, GLfloat *H)
{
	GLfloat mean[3] = {0.0f, 0.0f, 0.0f};
	for(int i = 0; i < 3; i++) {
		mean[i] = (A[i]+B[i]+C[i]+D[i]+E[i]+F[i]+G[i]+H[i]) / 8.0f;
	}
	glPushMatrix();
	glTranslatef(mean[0], mean[1], mean[2]);
	glutSolidSphere(abs(A[0] - B[0])/2.0f, 36, 17); //Ver.A : Sphere
	//glutSolidCube(abs(A[0] - B[0])); //Ver.B Box
	glPopMatrix();
}

void box(GLfloat *A, GLfloat *B, GLfloat *C, GLfloat *D, GLfloat *E, GLfloat *F, GLfloat *G, GLfloat *H)
{
	glBegin(GL_QUADS);
		//glColor3fv(colors[0]);
		quad(A, B, C, D); 
		//glColor3fv(colors[1]);
		quad(A, E, F, B); 
		//glColor3fv(colors[2]);
		quad(B, F, G, C); 
		//glColor3fv(colors[3]);
		quad(C, G, H, D); 
		//glColor3fv(colors[4]);
		quad(A, E, H, D); 
		//glColor3fv(colors[5]);
		quad(E, F, G, H); 	
    glEnd();
}

void divide_box(GLfloat *A, GLfloat *B, GLfloat *C, GLfloat *D, GLfloat *E, GLfloat *F, GLfloat *G, GLfloat *H, int m)
{
    GLfloat mid[12][3];
	GLfloat cen[7][3];
    int j;
    if(m>0)
    {
        /* compute six midpoints */
        for(j=0; j<3; j++) mid[0][j]=(A[j]+B[j])/2;
		for(j=0; j<3; j++) mid[1][j]=(B[j]+C[j])/2;
		for(j=0; j<3; j++) mid[2][j]=(C[j]+D[j])/2;
		for(j=0; j<3; j++) mid[3][j]=(D[j]+A[j])/2;
		
		for(j=0; j<3; j++) mid[4][j]=(A[j]+E[j])/2;
		for(j=0; j<3; j++) mid[5][j]=(B[j]+F[j])/2;
		for(j=0; j<3; j++) mid[6][j]=(C[j]+G[j])/2;
		for(j=0; j<3; j++) mid[7][j]=(D[j]+H[j])/2;
		
		for(j=0; j<3; j++) mid[8][j]=(E[j]+F[j])/2;
		for(j=0; j<3; j++) mid[9][j]=(F[j]+G[j])/2;
		for(j=0; j<3; j++) mid[10][j]=(G[j]+H[j])/2;
		for(j=0; j<3; j++) mid[11][j]=(H[j]+E[j])/2;

		for(j=0; j<3; j++) cen[0][j]=(A[j]+B[j]+C[j]+D[j])/4;
		for(j=0; j<3; j++) cen[1][j]=(A[j]+E[j]+F[j]+B[j])/4;
		for(j=0; j<3; j++) cen[2][j]=(B[j]+F[j]+G[j]+C[j])/4;
		for(j=0; j<3; j++) cen[3][j]=(C[j]+G[j]+H[j]+D[j])/4;
		for(j=0; j<3; j++) cen[4][j]=(A[j]+E[j]+H[j]+D[j])/4;
		for(j=0; j<3; j++) cen[5][j]=(E[j]+F[j]+G[j]+H[j])/4;
		for(j=0; j<3; j++) cen[6][j]=(cen[0][j]+cen[1][j]+cen[2][j]+cen[3][j]+cen[4][j]+cen[5][j])/6;

        /* create 4 tetrahedrons by subdivision */
		glColor3ubv(colorsb[0]);
        divide_box(A, mid[0], cen[0], mid[3], mid[4], cen[1], cen[6], cen[4], m-1);
		glColor3ubv(colorsb[1]);
        divide_box(cen[0], mid[1], C, mid[2], cen[6], cen[2], mid[6], cen[3], m-1);
		glColor3ubv(colorsb[2]);
		divide_box(cen[1], mid[5], cen[2], cen[6], mid[8], F, mid[9], cen[5], m-1);
		glColor3ubv(colorsb[3]);
		divide_box(cen[4], cen[6], cen[3], mid[7], mid[11], cen[5], mid[10], H, m-1);
    }
    else{
		if(sMode)
			sphere(A,B,C,D,E,F,G,H);
		else
			box(A,B,C,D,E,F,G,H); /* draw a box at end of recursion */
	}
}

GLuint* all_id = nullptr;
GLfloat* all_vertices = nullptr;
GLfloat* all_colors = nullptr;
GLubyte *CurrentColor = nullptr;
int vCounter = 0;
int cCounter = 0;
int iCounter = 0;

void gen_box(GLfloat *A, GLfloat *B, GLfloat *C, GLfloat *D, GLfloat *E, GLfloat *F, GLfloat *G, GLfloat *H)
{
	unsigned int CQBase = vCounter / 3; // Current Quad Base

	all_id[iCounter++] = CQBase+0; all_id[iCounter++] = CQBase+1; all_id[iCounter++] = CQBase+2; all_id[iCounter++] = CQBase+3;
	all_id[iCounter++] = CQBase+0; all_id[iCounter++] = CQBase+4; all_id[iCounter++] = CQBase+5; all_id[iCounter++] = CQBase+1;
	all_id[iCounter++] = CQBase+1; all_id[iCounter++] = CQBase+5; all_id[iCounter++] = CQBase+6; all_id[iCounter++] = CQBase+2;
	all_id[iCounter++] = CQBase+2; all_id[iCounter++] = CQBase+6; all_id[iCounter++] = CQBase+7; all_id[iCounter++] = CQBase+3;
	all_id[iCounter++] = CQBase+0; all_id[iCounter++] = CQBase+4; all_id[iCounter++] = CQBase+7; all_id[iCounter++] = CQBase+3;
	all_id[iCounter++] = CQBase+4; all_id[iCounter++] = CQBase+5; all_id[iCounter++] = CQBase+6; all_id[iCounter++] = CQBase+7;

	all_vertices[vCounter++] = A[0]; all_vertices[vCounter++] = A[1]; all_vertices[vCounter++] = A[2]; //0
	all_vertices[vCounter++] = B[0]; all_vertices[vCounter++] = B[1]; all_vertices[vCounter++] = B[2]; //1
	all_vertices[vCounter++] = C[0]; all_vertices[vCounter++] = C[1]; all_vertices[vCounter++] = C[2]; //2
	all_vertices[vCounter++] = D[0]; all_vertices[vCounter++] = D[1]; all_vertices[vCounter++] = D[2]; //3
	all_vertices[vCounter++] = E[0]; all_vertices[vCounter++] = E[1]; all_vertices[vCounter++] = E[2]; //4
	all_vertices[vCounter++] = F[0]; all_vertices[vCounter++] = F[1]; all_vertices[vCounter++] = F[2]; //5
	all_vertices[vCounter++] = G[0]; all_vertices[vCounter++] = G[1]; all_vertices[vCounter++] = G[2]; //6
	all_vertices[vCounter++] = H[0]; all_vertices[vCounter++] = H[1]; all_vertices[vCounter++] = H[2]; //7

	for(int i = 0; i<24; i++){
		all_colors[cCounter] = CurrentColor[cCounter%3]/255.0f;
		++cCounter;
	}
}

void gen_boxes(GLfloat *A, GLfloat *B, GLfloat *C, GLfloat *D, GLfloat *E, GLfloat *F, GLfloat *G, GLfloat *H, int m)
{
    GLfloat mid[12][3];
	GLfloat cen[7][3];
    int j;
    if(m>0)
    {
        /* compute six midpoints */
        for(j=0; j<3; j++) mid[0][j]=(A[j]+B[j])/2;
		for(j=0; j<3; j++) mid[1][j]=(B[j]+C[j])/2;
		for(j=0; j<3; j++) mid[2][j]=(C[j]+D[j])/2;
		for(j=0; j<3; j++) mid[3][j]=(D[j]+A[j])/2;
		
		for(j=0; j<3; j++) mid[4][j]=(A[j]+E[j])/2;
		for(j=0; j<3; j++) mid[5][j]=(B[j]+F[j])/2;
		for(j=0; j<3; j++) mid[6][j]=(C[j]+G[j])/2;
		for(j=0; j<3; j++) mid[7][j]=(D[j]+H[j])/2;
		
		for(j=0; j<3; j++) mid[8][j]=(E[j]+F[j])/2;
		for(j=0; j<3; j++) mid[9][j]=(F[j]+G[j])/2;
		for(j=0; j<3; j++) mid[10][j]=(G[j]+H[j])/2;
		for(j=0; j<3; j++) mid[11][j]=(H[j]+E[j])/2;

		for(j=0; j<3; j++) cen[0][j]=(A[j]+B[j]+C[j]+D[j])/4;
		for(j=0; j<3; j++) cen[1][j]=(A[j]+E[j]+F[j]+B[j])/4;
		for(j=0; j<3; j++) cen[2][j]=(B[j]+F[j]+G[j]+C[j])/4;
		for(j=0; j<3; j++) cen[3][j]=(C[j]+G[j]+H[j]+D[j])/4;
		for(j=0; j<3; j++) cen[4][j]=(A[j]+E[j]+H[j]+D[j])/4;
		for(j=0; j<3; j++) cen[5][j]=(E[j]+F[j]+G[j]+H[j])/4;
		for(j=0; j<3; j++) cen[6][j]=(cen[0][j]+cen[1][j]+cen[2][j]+cen[3][j]+cen[4][j]+cen[5][j])/6;

        /* create 4 tetrahedrons by subdivision */
		CurrentColor = colorsb[0];
        gen_boxes(A, mid[0], cen[0], mid[3], mid[4], cen[1], cen[6], cen[4], m-1);
		CurrentColor = colorsb[1];
        gen_boxes(cen[0], mid[1], C, mid[2], cen[6], cen[2], mid[6], cen[3], m-1);
		CurrentColor = colorsb[2];
		gen_boxes(cen[1], mid[5], cen[2], cen[6], mid[8], F, mid[9], cen[5], m-1);
		CurrentColor = colorsb[3];
		gen_boxes(cen[4], cen[6], cen[3], mid[7], mid[11], cen[5], mid[10], H, m-1);
    }
    else{
		gen_box(A,B,C,D,E,F,G,H); /* draw a box at end of recursion */
	}
}

void gen_boxes_master(GLfloat *A, GLfloat *B, GLfloat *C, GLfloat *D, GLfloat *E, GLfloat *F, GLfloat *G, GLfloat *H, int n)
{	
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);
	if(all_id != nullptr){
		delete [] all_id;
		delete [] all_vertices;
		delete [] all_colors;
	}
	
	NumOfPoints = 24*(int)pow(4,n);
	all_id = new GLuint[ NumOfPoints ];
	all_vertices = new GLfloat[ NumOfPoints ];
	all_colors = new GLfloat[ NumOfPoints ];

	vCounter = 0;
	cCounter = 0;
	iCounter = 0;

	gen_boxes(A, B, C, D, E, F, G, H, n);
	glVertexPointer(3, GL_FLOAT, 0, all_vertices);
	glColorPointer(3, GL_FLOAT, 0, all_colors);
	glEnableClientState(GL_VERTEX_ARRAY); 
	glEnableClientState(GL_COLOR_ARRAY);
	return;
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glRotatef(theta, 1.0, 0.0, 0.0); //Rotate it!
	glRotatef(theta, 0.0, 1.0, 0.0); //Rotate it!

    if(sMode)
		divide_box(vb[0], vb[1], vb[2], vb[3], vb[4], vb[5], vb[6], vb[7], n);
	else
		glDrawElements(GL_QUADS, NumOfPoints, GL_UNSIGNED_INT, all_id);

    //glFlush();
	glutSwapBuffers();
}

void spinIt() //STEP:5.2 spin it
{	
	if(!active)
		return;

	clock_t now = clock(); //need time.h
	static clock_t lastTime = 0;
	if((now - lastTime)/(float)CLOCKS_PER_SEC > (1.0f/60.0f)){ //<60fps

		/* Idle callback, spin cube 2 degrees about selected axis */		
		theta += 1;
		if( theta > 360.0 ) theta -= 360.0;

		glutPostRedisplay();
	
		// Record current time for next time:
		lastTime = now;
	}
}

void myKey(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'Q': case 'q':
		if(n > 1) {
			n--;
			gen_boxes_master(vb[0], vb[1], vb[2], vb[3], vb[4], vb[5], vb[6], vb[7], n);
			glutPostRedisplay();
		}
		break;
	case 'E': case 'e':
		if(n < 10) {
			n++;
			gen_boxes_master(vb[0], vb[1], vb[2], vb[3], vb[4], vb[5], vb[6], vb[7], n);
			glutPostRedisplay();
		}
		break;
	case 'P': case 'p':
		active = !active; glutPostRedisplay(); break;
	case 'S': case 's':
		sMode = !sMode; glutPostRedisplay(); break;
	default:
		break;
	}
}

void myReshape(int w, int h)
{
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    if (w <= h)
        glOrtho(-2.0, 2.0, -2.0 * (GLfloat) h / (GLfloat) w,
            2.0 * (GLfloat) h / (GLfloat) w, -10.0, 10.0);
    else
        glOrtho(-2.0 * (GLfloat) w / (GLfloat) h,
            2.0 * (GLfloat) w / (GLfloat) h, -2.0, 2.0, -10.0, 10.0);
    glMatrixMode(GL_MODELVIEW);
    glutPostRedisplay();
}

int main(int argc, char **argv)
{
    if(argc > 1) n=atoi(argv[1]); /* or set number of subdivision steps here */
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(500, 500);
    glutCreateWindow("3D Gasket");
    glutReshapeFunc(myReshape);
    glutDisplayFunc(display);
	glutIdleFunc(spinIt); //add spin function
	glutKeyboardFunc(myKey); //add keyboard function
    glEnable(GL_DEPTH_TEST);
    glClearColor (0.0, 0.0, 0.0, 1.0);

    glEnableClientState(GL_VERTEX_ARRAY); 
	glEnableClientState(GL_COLOR_ARRAY);
	gen_boxes_master(vb[0], vb[1], vb[2], vb[3], vb[4], vb[5], vb[6], vb[7], n);

    glutMainLoop();
}
