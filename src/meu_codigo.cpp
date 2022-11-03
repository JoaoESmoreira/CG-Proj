/*

  João Moreira
  2022/2023
*/


#include <GL/freeglut_std.h>
#include <GL/gl.h>
#include <GL/glut.h>
#include <cmath>
#include <iostream>

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
static GLuint  DONW[] = { 20, 21, 22, 23 };

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

  // glTranslated(2.5 + xCarTrans, 1, 1);

  glTranslated(0 + xCarTrans, 0, 0);

  // car
  glPushMatrix();

    // chemney
    glPushMatrix();
      glTranslated(1, yChimTrans, 0.5);
      glScaled(0.1, 1, 0.1);
      drawCube();
    glPopMatrix();

    // body car
    glPushMatrix();
      glScalef(3, 1, 1);
      glDisableClientState(GL_COLOR_ARRAY);
      glColor4f(RED);
      drawCube();
    glPopMatrix();

    // wheel
    glPushMatrix();
      glTranslated(1.25, 0, 1.5);
      glRotated(angWheel, 0, 0, 1);
      glScalef(0.75, 0.75, 0.5);
      glDisableClientState(GL_COLOR_ARRAY);
      glColor4f(GREEN);
      drawCube();
    glPopMatrix();
    // wheel
    glPushMatrix();
      glTranslated(1.25, 0, -1.5);
      glRotated(angWheel, 0, 0, 1);
      glScalef(0.75, 0.75, 0.5);
      glDisableClientState(GL_COLOR_ARRAY);
      glColor4f(GREEN);
      drawCube();
    glPopMatrix();
    // wheel
    glPushMatrix();
      glTranslated(-1.25, 0, 1.5);
      glRotated(angWheel, 0, 0, 1);
      glScalef(0.75, 0.75, 0.5);
      glDisableClientState(GL_COLOR_ARRAY);
      glColor4f(GREEN);
      drawCube();
    glPopMatrix();
    // wheel
    glPushMatrix();
      glTranslated(-1.25, 0, -1.5);
      glRotated(angWheel, 0, 0, 1);
      glScalef(0.75, 0.75, 0.5);
      glDisableClientState(GL_COLOR_ARRAY);
      glColor4f(GREEN);
      drawCube();
    glPopMatrix();


  glPopMatrix();

}

void display(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective( 85, (float)height / width, 0.1, 9999);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();


	gluLookAt(PersonPosition[0], PersonPosition[1], PersonPosition[2], 0, 0, 0, 0, 1, 0);
	
	// Objectos
	drawAxis();
	draw();



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
  glEnableClientState(GL_COLOR_ARRAY);


  xCarTrans = yChimTrans = angWheel = yAngPersonPosition = AngPersonPosition = 0;
  PersonPosition[0] = SIZE * cos(yAngPersonPosition/(2*PI));
  PersonPosition[1] = SIZE * sin(AngPersonPosition/(2*PI));
  PersonPosition[2] = SIZE * sin(yAngPersonPosition/(2+PI));
}

void keyboard(unsigned char key, int x, int y) {
  switch(key) {

  case 'a':
  case 'A':
      yAngPersonPosition -= 1;
      PersonPosition[0] = SIZE * cos(yAngPersonPosition/(2*PI));
      PersonPosition[2] = SIZE * sin(yAngPersonPosition/(2*PI));
    break;

  case 'd':
  case 'D':
      yAngPersonPosition += 1;
      PersonPosition[0] = SIZE * cos(yAngPersonPosition/(2*PI));
      PersonPosition[2] = SIZE * sin(yAngPersonPosition/(2*PI));
    break;

  case 'w':
  case 'W':
      AngPersonPosition += 1;
      PersonPosition[1] = SIZE * sin(AngPersonPosition/(2*PI));
      break;

  case 's':
  case 'S':
      AngPersonPosition -= 1;
      PersonPosition[1] = SIZE * sin(AngPersonPosition/(2*PI));
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

int main(int argc, char** argv) {
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
  glutInitWindowSize(width, height);
  glutInitWindowPosition(350, 0);
  glutCreateWindow("My car project");

  init();

  glutSpecialFunc(teclasNotAscii);
  glutDisplayFunc(display);
  glutKeyboardFunc(keyboard);

  glutMainLoop();

  return 0;
}
