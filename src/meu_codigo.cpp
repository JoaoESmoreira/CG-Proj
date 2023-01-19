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
int material;

bool light_bulb_on;
GLfloat light_bulb_intensity = 1;
GLfloat light_bulb_r, light_bulb_g, light_bulb_b;

GLfloat light_bulb_position[4] = {0.0f, 5.0f, 0.0f, 1.0f};
GLfloat light_bulb_ambient[4] = {1.0f, 1.0f,  1.0f, 0.0f};
GLfloat light_bulb_specular[4] = {light_bulb_r, light_bulb_g, light_bulb_b, 1.0f};
GLfloat light_bulb_difuse[4] = {light_bulb_r, light_bulb_g, light_bulb_b, 1.0f};

// Spot Light - GL_LIGHT1
bool spotlight_on;
GLfloat spotlight_intensity = 1;
GLfloat spotlight_r = 0.5, spotlight_g = 0.5, spotlight_b = 0.5;

GLfloat spotlight_position[4] = {.0f, 5.0f, .0f, 1.0f};
GLfloat spotlight_direction[4] = {.0f, -1.0f, .0f, 0.0f};

GLfloat spotlight_ambient[4] = {0.0f, 0.0f, 0.0f, 1.0f};
GLfloat spotlight_difuse[4] = {spotlight_r, spotlight_g, spotlight_b, 1.0f};
GLfloat spotlight_specular[4] = {spotlight_r, spotlight_g, spotlight_b, 1.0f};

GLfloat spot_cutoff = 20;
GLfloat spot_exponent = 0;

GLfloat spotlight_kc = 0.2;
GLfloat spotlight_kl = 0.05f;
GLfloat spotlight_kq = 0.0f;

GLfloat ks = 0.25;
bool use_grid = true;
void update_light(GLenum light) {

    switch (light) {
        case GL_LIGHT0: {

            light_bulb_ambient[0] = light_bulb_r * light_bulb_intensity;
            light_bulb_ambient[1] = light_bulb_g * light_bulb_intensity;
            light_bulb_ambient[2] = light_bulb_b * light_bulb_intensity;

            light_bulb_specular[0] = light_bulb_r * light_bulb_intensity;
            light_bulb_specular[1] = light_bulb_g * light_bulb_intensity;
            light_bulb_specular[2] = light_bulb_b * light_bulb_intensity;

            light_bulb_difuse[0] = light_bulb_r * light_bulb_intensity;
            light_bulb_difuse[1] = light_bulb_g * light_bulb_intensity;
            light_bulb_difuse[2] = light_bulb_b * light_bulb_intensity;

            glLightfv(GL_LIGHT0, GL_POSITION, light_bulb_position);
            glLightfv(GL_LIGHT0, GL_AMBIENT, light_bulb_ambient);
            glLightfv(GL_LIGHT0, GL_DIFFUSE, light_bulb_difuse);
            glLightfv(GL_LIGHT0, GL_SPECULAR, light_bulb_specular);

            glDisable(GL_LIGHTING);
            glPushMatrix();
            {
                glColor3f(light_bulb_r, light_bulb_g, light_bulb_b);
                glTranslatef(light_bulb_position[0], light_bulb_position[1], light_bulb_position[2]);
                glutSolidSphere(0.5f, 100, 100);
            }
            glPopMatrix();
            glEnable(GL_LIGHTING);

            if (light_bulb_on)
                glDisable(GL_LIGHT0);
            else
                glEnable(GL_LIGHT0);
            break;
        }

        case GL_LIGHT1: {
            // Spot Light
            glLightfv(GL_LIGHT1, GL_POSITION, (const GLfloat *)spotlight_position);
            glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, (const GLfloat *)spotlight_direction);

            glLightfv(GL_LIGHT1, GL_AMBIENT, spotlight_ambient);
            glLightfv(GL_LIGHT1, GL_DIFFUSE, spotlight_difuse);
            glLightfv(GL_LIGHT1, GL_SPECULAR, spotlight_specular);

            glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, spot_cutoff);
            glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, spot_exponent);

            glLightf(GL_LIGHT1, GL_LINEAR_ATTENUATION, spotlight_kl);
            glLightf(GL_LIGHT1, GL_QUADRATIC_ATTENUATION, spotlight_kq);
            glLightf(GL_LIGHT1, GL_CONSTANT_ATTENUATION, spotlight_kc);

            glDisable(GL_LIGHTING);
            glPushMatrix();
            {
                glColor3f(spotlight_r, spotlight_g, spotlight_b);
                glTranslatef(spotlight_position[0], spotlight_position[1], spotlight_position[2]);
                glutSolidSphere(0.5f, 100, 100);
            }
            glPopMatrix();
            glEnable(GL_LIGHTING);

            if (spotlight_on)
                glDisable(GL_LIGHT1);
            else
                glEnable(GL_LIGHT1);
            break;
        }
    }
}

GLUquadricObj* esfera = gluNewQuadric();

void drawEsfera() {
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture[1]);
    glPushMatrix();
    //glTranslatef(2, 4, 2);
    glRotatef(-90, 1, 0, 0);
    gluQuadricDrawStyle(esfera, GLU_FILL);
    gluQuadricNormals(esfera, GLU_SMOOTH);
    gluQuadricTexture(esfera, GL_TRUE);
    gluSphere(esfera, 60.0, 100, 100);
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
}

void initTexturas() {
	glGenTextures(1, &texture[0]);
	glBindTexture(GL_TEXTURE_2D, texture[0]);
	imag.LoadBmpFile("textures/pneu.bmp");
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexImage2D(GL_TEXTURE_2D, 0, 3,
		imag.GetNumCols(),
		imag.GetNumRows(), 0, GL_RGB, GL_UNSIGNED_BYTE,
		imag.ImageData());

	glGenTextures(1, &texture[1]);
	glBindTexture(GL_TEXTURE_2D, texture[1]);
	imag.LoadBmpFile("textures/try.bmp");
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

void drawAxis() {
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

bool malha;

void draw() {


    glTranslated(0 + xCarTrans, 0, 0);

    GLfloat y = -0.5;
    glPushMatrix();

    initMaterials(10);
    int dim;
    if (malha)
	dim = 1024;
    else
	dim = 1;

    glPushMatrix();
    glScalef(50, 1, 5);
    glTranslatef(-1, -1, -1);  //meio do poligono 
    float med_dim = (float)dim / 2;
    glBegin(GL_QUADS);
    int i, j;
    for (i = 0; i < dim; i++)
        for (j = 0; j < dim; j++) {
            glVertex3d((float)j / med_dim, 0, (float)(i + 1) / med_dim);
            glVertex3d((float)(j + 1) / med_dim, 0, (float)(i + 1) / med_dim);
            glVertex3d((float)(j + 1) / med_dim, 0, (float)i / med_dim);
            glVertex3d((float)j / med_dim, 0, (float)i / med_dim);
        }
    glEnd();
    glPopMatrix();


    initMaterials(material);





    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    // body car
    glPushMatrix();
      glScalef(2, 1.2, 1);
      glDisableClientState(GL_COLOR_ARRAY);
      glColor4f(ORANGE);
      drawCube();
    glPopMatrix();

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture[0]);
    initMaterials((material+1)%18);
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
    // chemney
    glColor4f(1, 1, 1, 0.4);
    glEnable(GL_BLEND);
    glEnable(GL_COLOR_MATERIAL);
        glColorMaterial(GL_FRONT_AND_BACK, GL_DIFFUSE);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glPushMatrix();
      glTranslated(1, yChimTrans, 0.5);
     glScaled(0.3, 0.9, 0.3);
      drawCube();
    glPopMatrix();
    glDisable(GL_COLOR_MATERIAL);
    glDisable(GL_BLEND);



  glPopMatrix();
}

void display(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );


	// glViewport(0,0, 300, 300);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective( 85, (float)height / width, 0.1, 9999);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();


	// gluLookAt(0,10,0, 0, 0, 0, -1, 0, 0);
	gluLookAt(PersonPosition[0], PersonPosition[1], PersonPosition[2], 0, 0, 0, 0, 1, 0);
	

	drawEsfera();

	drawAxis();
	draw();
	update_light(GL_LIGHT0);
	update_light(GL_LIGHT1);

	/*glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective( 85, (float)height / width, 0.1, 9999);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();


	gluLookAt(PersonPosition[0], PersonPosition[1], PersonPosition[2], 0, 0, 0, 0, 1, 0);
	

	drawAxis();
	draw();*/
	// update_light(GL_LIGHT0);
	// update_light(GL_LIGHT1);

	glutSwapBuffers();
	glutPostRedisplay();
}


void init(void) {
  glEnable(GL_DEPTH_TEST);
  glShadeModel(GL_SMOOTH);

  // glEnable(GL_CULL_FACE);
  // glCullFace(GL_BACK);

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

  material = 0;
  initMaterials(material);

  glEnable(GL_NORMALIZE);

  light_bulb_on = spotlight_on = false;
  light_bulb_r = 0.5f, light_bulb_g = 0.5f, light_bulb_b = 0.5f;

  malha = true;
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
  case 'm': case 'M':
      material = (material + 1) % 18;
      initMaterials(material);
      glutPostRedisplay();
      break;

  // Spotlight ON/OFF
  case 'P': case 'p':
      spotlight_on = !spotlight_on;
      if (spotlight_on)
	  cout << "Sportlight desativated\n";
      else
	  cout << "Sportlight activcated\n";
      break;
  // Light Bulb ON/OFF
  case 'O': case 'o':
      light_bulb_on = !light_bulb_on;
      if (light_bulb_on)
	  cout << "bulb desativated\n";
      else
	  cout << "bulb activcated\n";
      break;
  case 'H': case 'h':
      spotlight_position[0] -= 0.2;
      break;
  case 'L': case 'l':
      spotlight_position[0] += 0.2;
      break;
  case 'J': case 'j':
      spotlight_position[2] += 0.2;
      break;
  case 'K': case 'k':
      spotlight_position[2] -= 0.2;
      break;

  case 'R': case 'r':
      light_bulb_r += 0.1;
      if (light_bulb_r > 1.1) {
	  light_bulb_r = 0;
      }
      cout << "red: " << light_bulb_r << endl;
      break;
  case 'G': case 'g':
      light_bulb_g += 0.1;
      if (light_bulb_g > 1.1) {
	  light_bulb_g = 0;
      }
      cout << "green: " << light_bulb_g << endl;
      break;
  case 'B': case 'b':
      light_bulb_b += 0.1;
      if (light_bulb_b > 1.1) {
	  light_bulb_b = 0;
      }
      cout << "blue: " << light_bulb_b << endl;
      break;
  case 'Y': case 'y':
      malha = !malha;
      break;

  case 'i':
      light_bulb_intensity += 0.1;
      spotlight_intensity += 0.1;
      if (light_bulb_intensity > 1.1) {
	  spotlight_intensity = 0;
	  light_bulb_intensity = 0;
      } 
      cout << "intencity: " << light_bulb_intensity << endl;
      
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
  initTexturas();
  glLightModelfv(GL_LIGHT_MODEL_AMBIENT, (const GLfloat[4]){0.2f, 0.2f, 0.2f, 0.2f});
    update_light(GL_LIGHT0);
    update_light(GL_LIGHT1);

  glutSpecialFunc(teclasNotAscii);
  glutDisplayFunc(display);
  glutKeyboardFunc(keyboard);

  glutMainLoop();

  return 0;
}
