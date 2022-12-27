/*

  João Moreira
  2022/2023
*/


#include <GL/freeglut_std.h>
#include <GL/gl.h>
#include <GL/glext.h>
#include <GL/glut.h>
#include <cmath>
#include <iostream>
#include <iterator>
#include "RgbImage.h"


using namespace std;


/* -------Cores------- */
#define BLUE     0.0, 0.0, 1.0, 1.0 
#define YELLOW	 1.0, 1.0, 0.0, 1.0
#define GREEN    0.0, 1.0, 0.0, 1.0
#define ORANGE   1.0, 0.5, 0.1, 1.0
#define CYAN     0.0, 1.0, 1.0, 1.0
#define WHITE    1.0, 1.0, 1.0, 1.0
#define BLACK    0.0, 0.0, 0.0, 1.0
#define GRAY     0.3, 0.3, 0.3, 1.0
#define RED      1.0, 0.0, 0.0, 1.0

#define SIZE   20.0
#define height 1000
#define width  1200
#define tam    1
#define PI 3.14
#define increment 1 

static GLfloat vertex[] = {
    // TOP
    -tam, tam, -tam,  // 0
    -tam, tam, tam,   // 1
    tam, tam, tam,    // 2
    tam, tam, -tam,   // 3
                      // LEFT
    -tam, tam, tam,   // 4
    -tam, tam, -tam,  // 5
    -tam, -tam, -tam, // 6
    -tam, -tam, tam,  // 7
                      // RIGHT
    tam, tam, tam,    // 8
    tam, -tam, tam,   // 9
    tam, -tam, -tam,  // 10
    tam, tam, -tam,   // 11
                      // FRONT
    -tam, tam, tam,   // 12
    -tam, -tam, tam,  // 13
    tam, -tam, tam,   // 14
    tam, tam, tam,    // 15
                      // BACK
    -tam, tam, -tam,  // 16
    tam, tam, -tam,   // 17
    tam, -tam, -tam,  // 18
    -tam, -tam, -tam, // 19
                      // DOWN
    -tam, -tam, -tam,  // 20
    tam, -tam, -tam,  // 21
    tam, -tam, tam,   // 22
    -tam, -tam, tam,  // 23
};

static GLfloat normals[] {
  0,1,0,
  0,1,0,
  0,1,0,
  0,1,0,

  -1,0,0,
  -1,0,0,
  -1,0,0,
  -1,0,0,

  1,0,0,
  1,0,0,
  1,0,0,
  1,0,0,

  0,0,1,
  0,0,1,
  0,0,1,
  0,0,1,

  0,0,-1,
  0,0,-1,
  0,0,-1,
  0,0,-1,

  0,-1,0,
  0,-1,0,
  0,-1,0,
  0,-1,0,
};

static GLfloat color[] = {

  GRAY, GRAY, GRAY, GRAY, 
  GRAY, GRAY, GRAY, GRAY,
  GRAY, GRAY, GRAY, GRAY,
  GRAY, GRAY, GRAY, GRAY,
  GRAY, GRAY, GRAY, GRAY,
  GRAY, GRAY, GRAY, GRAY,

};



GLfloat PersonPosition[3];
GLfloat yAngPersonPosition;  // angle arround yy
GLfloat AngPersonPosition;  // angle arround yy
GLfloat xCarTrans; // x pos car translation
GLfloat yChimTrans; // y pos of chimney translation
GLfloat angWheel;   // rotation angle of wheels

static GLuint   TOP[] = { 0, 1, 2, 3 };
static GLuint  LEFT[] = { 4, 5, 6, 7 };
static GLuint RIGHT[] = { 8, 9, 10, 11 };
static GLuint FRONT[] = { 12, 13, 14, 15 };
static GLuint  BACK[] = { 16, 17, 18, 19 };
static GLuint DONW[] = {20, 21, 22, 23};

static GLfloat texturas[] = {
    0, 0, 
    1, 0, 
    1, 1, 
    0, 1,

    0, 0,
    1, 0,
    1, 1,
    0, 1,

    0, 0,
    1, 0,
    1, 1,
    0, 1,

    0, 0, 
    1, 0, 
    1, 1, 
    0, 1,

    0, 0,
    1, 0,
    1, 1,
    0, 1,

    0, 0, 
    1, 0, 
    1, 1, 
    0, 1,
};

GLuint texture[5];
RgbImage imag;

void initMaterials(int material);

GLfloat intensidadeDia = 0.0;
GLfloat luzGlobalCorAmb[4] = { intensidadeDia, intensidadeDia,intensidadeDia, 1.0 };   // 

//---------------------------------------------------- Luz pontual no TETO (eixo Y)
GLint   ligaTeto = 1;		 //:::   'T'  
GLfloat intensidadeT = 0.3;  //:::   'I'  
GLint   luzR = 1;		 	 //:::   'R'  
GLint   luzG = 1;			 //:::   'G'  
GLint   luzB = 1;			 //:::   'B'  
GLfloat localPos[4] = { 0.0, 5.0, 0.0, 1.0 };
GLfloat localCorAmb[4] = { 0, 0, 0, 0.0 };
GLfloat localCorDif[4] = { luzR, luzG, luzB, 1.0 };
GLfloat localCorEsp[4] = { luzR, luzG, luzB, 1.0 };
GLint   material = 10;
char texto[30];

void initLights(void) {
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, luzGlobalCorAmb);
	glLightfv(GL_LIGHT0, GL_POSITION, localPos);
	glLightfv(GL_LIGHT0, GL_AMBIENT, localCorAmb);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, localCorDif);
	glLightfv(GL_LIGHT0, GL_SPECULAR, localCorEsp);
}

void iluminacao() {
	glLightfv(GL_LIGHT0, GL_POSITION, localPos);
	glLightfv(GL_LIGHT0, GL_AMBIENT, localCorAmb);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, localCorDif);
	glLightfv(GL_LIGHT0, GL_SPECULAR, localCorEsp);
	if (ligaTeto)  
		glEnable(GL_LIGHT0);
	else   		  
		glDisable(GL_LIGHT0);
}

void initTexturas(){
	glGenTextures(1, &texture[0]);
	glBindTexture(GL_TEXTURE_2D, texture[0]);
	imag.LoadBmpFile("textures/pneu.bmp");
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexImage2D(GL_TEXTURE_2D, 0, 3,
		imag.GetNumCols(),
		imag.GetNumRows(), 0, GL_RGB, GL_UNSIGNED_BYTE,
		imag.ImageData());

}

void updateLuz() {
	localCorAmb[0] = luzR * intensidadeT;
	localCorAmb[1] = luzG * intensidadeT;
	localCorAmb[2] = luzB * intensidadeT;
	localCorDif[0] = luzR * intensidadeT;
	localCorDif[1] = luzG * intensidadeT;
	localCorDif[2] = luzB * intensidadeT;
	localCorEsp[0] = luzR * intensidadeT;
	localCorEsp[1] = luzG * intensidadeT;
	localCorEsp[2] = luzB * intensidadeT;;
	glLightfv(GL_LIGHT0, GL_AMBIENT, localCorAmb);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, localCorDif);
	glLightfv(GL_LIGHT0, GL_SPECULAR, localCorEsp);
}

void drawAxis()
{
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Eixo
	glColor4f(ORANGE);
	glBegin(GL_LINES);
		glVertex3f(0, 0, 0);
		glVertex3f(SIZE, 0, 0);
	glEnd();
	glColor4f(GREEN);
	glBegin(GL_LINES);
		glVertex3f(0, 0, 0);
		glVertex3f(0, SIZE, 0);
	glEnd();
	glColor4f(BLUE);
	glBegin(GL_LINES);
		glVertex3f(0, 0, 0);
		glVertex3f(0, 0, SIZE);
	glEnd();
}

void drawCube() {
      glDrawElements(GL_POLYGON, 4, GL_UNSIGNED_INT, TOP);
      glDrawElements(GL_POLYGON, 4, GL_UNSIGNED_INT, LEFT);
      glDrawElements(GL_POLYGON, 4, GL_UNSIGNED_INT, RIGHT);
      glDrawElements(GL_POLYGON, 4, GL_UNSIGNED_INT, FRONT);
      glDrawElements(GL_POLYGON, 4, GL_UNSIGNED_INT, BACK);
      glDrawElements(GL_POLYGON, 4, GL_UNSIGNED_INT, DONW);
}

void draw() {


    glTranslated(0 + xCarTrans, 0, 0);
    GLfloat y = -0.5;
  // car
  glPushMatrix();

    // chemney
    glPushMatrix();
      glTranslated(1, yChimTrans, 0.5);
     glScaled(0.1, 0.9, 0.1);
      drawCube();
    glPopMatrix();

    // body car
    glPushMatrix();
      glScalef(2, 1.2, 1);
      glDisableClientState(GL_COLOR_ARRAY);
      glColor4f(ORANGE);
      drawCube();
    glPopMatrix();



    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture[0]);
      // wheel
      glPushMatrix();
        glTranslated(1.25, y, 1.5);
        glRotated(angWheel, 0, 0, 1);
        glScalef(0.75, 0.75, 0.5);
        glDisableClientState(GL_COLOR_ARRAY);
        glColor4f(YELLOW);
        drawCube();
      glPopMatrix();
      // wheel
      glPushMatrix();
        glTranslated(1.25, y, -1.5);
        glRotated(angWheel, 0, 0, 1);
        glScalef(0.75, 0.75, 0.5);
        glDisableClientState(GL_COLOR_ARRAY);
        glColor4f(YELLOW);
        drawCube();
      glPopMatrix();
      // wheel
      glPushMatrix();
        glTranslated(-1.25, y, 1.5);
        glRotated(angWheel, 0, 0, 1);
        glScalef(0.75, 0.75, 0.5);
        glDisableClientState(GL_COLOR_ARRAY);
        glColor4f(YELLOW);
        drawCube();
      glPopMatrix();
      // wheel
      glPushMatrix();
        glTranslated(-1.25, y, -1.5);
        glRotated(angWheel, 0, 0, 1);
        glScalef(0.75, 0.75, 0.5);
        glDisableClientState(GL_COLOR_ARRAY);
        glColor4f(YELLOW);
        drawCube();
      glPopMatrix();

    glDisable(GL_TEXTURE_2D);

  glPopMatrix();

}

void desenhaTexto(char* string, GLfloat x1, GLfloat y1, GLfloat z1) {
	glRasterPos3f(x1, y1, z1);
	while (*string)
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, *string++);
}

void display(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	sprintf(texto, "   %d%d%d - Luz 'RGB' ", luzR, luzG, luzB);
	desenhaTexto(texto, 12, 1, 5);

	glViewport(0,0, 300, 300);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective( 85, (float)height / width, 0.1, 9999);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();


	gluLookAt(0,10,0, 0, 0, 0, -1, 0, 0);
	

	drawAxis();
	draw();

	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective( 85, (float)height / width, 0.1, 9999);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();


	gluLookAt(PersonPosition[0], PersonPosition[1], PersonPosition[2], 0, 0, 0, 0, 1, 0);
	

	glEnable(GL_LIGHTING);
	drawAxis();
	draw();
  iluminacao();


	glutSwapBuffers();
	glutPostRedisplay();
}


void init(void) {
  glEnable(GL_DEPTH_TEST);
  glShadeModel(GL_SMOOTH);

  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);

  glVertexPointer(3, GL_FLOAT, 0, vertex);
  glEnableClientState(GL_VERTEX_ARRAY);
  glColorPointer(4, GL_FLOAT, 0, color);
  glNormalPointer(GL_FLOAT, 0, normals);
  glEnableClientState(GL_COLOR_ARRAY);
  glTexCoordPointer(2, GL_FLOAT, 0, texturas);
  glEnableClientState(GL_TEXTURE_COORD_ARRAY);


  xCarTrans = yChimTrans = angWheel = yAngPersonPosition = AngPersonPosition = 0;
  PersonPosition[0] = SIZE * cos(yAngPersonPosition/(2*PI));
  PersonPosition[1] = SIZE * sin(AngPersonPosition/(2*PI));
  PersonPosition[2] = SIZE * sin(yAngPersonPosition/(2+PI));

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	
	initLights();
	initMaterials(0);   // gold
}

void keyboard(unsigned char key, int x, int y) {
  switch(key) {

  case 'a':
  case 'A':
      yAngPersonPosition += 3;
      PersonPosition[0] = SIZE * cos((PI*yAngPersonPosition)/(180));
      PersonPosition[2] = SIZE * sin((PI*yAngPersonPosition)/(180));
    break;

  case 'd':
  case 'D':
      yAngPersonPosition -= 3;
      PersonPosition[0] = SIZE * cos((PI*yAngPersonPosition)/(180));
      PersonPosition[2] = SIZE * sin((PI*yAngPersonPosition)/(180));
    break;

  case 'w':
  case 'W':
      AngPersonPosition += 3;
      PersonPosition[1] = SIZE * sin((PI*AngPersonPosition)/(180));
      break;

  case 's':
  case 'S':
      AngPersonPosition -= 3;
      PersonPosition[1] = SIZE * sin((PI*AngPersonPosition)/(180));
      break;

      // position of camera
  case '1':
      // x direction
      PersonPosition[0] = SIZE;
      PersonPosition[1] = 1;
      PersonPosition[2] = 0;
      yAngPersonPosition = AngPersonPosition = 0;
      break;
  case '2':
      // y direction
      PersonPosition[0] = 0;
      PersonPosition[1] = 1;
      PersonPosition[2] = SIZE;
      yAngPersonPosition = 90; AngPersonPosition = 0;
      break;
  case '3':
      // x direction
      PersonPosition[0] = -SIZE;
      PersonPosition[1] = 1;
      PersonPosition[2] = 0;
      yAngPersonPosition = 180; AngPersonPosition = 0;
      break;
  case '4':
      // y direction
      PersonPosition[0] = 0;
      PersonPosition[1] = 1;
      PersonPosition[2] = -SIZE;
      yAngPersonPosition = 270; AngPersonPosition = 0;
      break;
	case 'i': case 'I':
		intensidadeT = intensidadeT + 0.1;
		if (intensidadeT > 1.1) intensidadeT = 0;
		updateLuz();
		glutPostRedisplay();
		break;
	case 'r':case 'R':
		luzR = (luzR + 1) % 2;
		updateLuz();
		glutPostRedisplay();
		break;
	case 'g':case 'G':
		luzG = (luzG + 1) % 2;
		updateLuz();
		glutPostRedisplay();
		break;
	case 'b':case 'B':
		luzB = (luzB + 1) % 2;
		updateLuz();
		glutPostRedisplay();
		break;

		//--------------------------- MAterial
	case 'm': case 'M':
		material = (material + 1) % 18;
		initMaterials(material);
		glutPostRedisplay();
		break;

  case 27:
    exit(0);
    break;
  }
}

void teclasNotAscii(int key, int x, int y) {
  if (key == GLUT_KEY_LEFT) {
    xCarTrans -= increment;
    angWheel += 20;
  } else if (key == GLUT_KEY_RIGHT) {
    xCarTrans += increment;
    angWheel -= 20;
    
  }
  if (key == GLUT_KEY_UP && yChimTrans + 0.2 <= 2) {
    yChimTrans += 0.2;
  } else if (key == GLUT_KEY_DOWN && yChimTrans - 0.2>= 0) {
    yChimTrans -= 0.2;
  }
	
  glutPostRedisplay();
}

// void initCube() {
//     GLfloat difColor[] = { 1, 1, 1};

//     glMaterialfv(GL_FRONT, GL_DIFFUSE, difColor);
//     glMaterialf(GL_FRONT, GL_SHININESS, 2);
// }

int main(int argc, char** argv) {
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
  glutInitWindowSize(width, height);
  glutInitWindowPosition(350, 0);
  glutCreateWindow("My car project");

  init();
  initTexturas();
  // initCube();

  glutSpecialFunc(teclasNotAscii);
  glutDisplayFunc(display);
  glutKeyboardFunc(keyboard);

  glutMainLoop();

  return 0;
}
