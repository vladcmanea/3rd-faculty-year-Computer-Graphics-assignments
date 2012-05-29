#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <windows.h>
#include "glut.h"

// dimensiunea ferestrei in pixeli
#define dim 700
#define PI acos(-1.0)

unsigned char prevKey;

enum EObiect {cubw, cubs, sferaw, sferas, trig, cubr} ob = cubw;
int step = 0, step2 = 0;

class Point
{
private:
	double x, y, z;

public:
	Point(double x, double y, double z)
	{
		this->x = x;
		this->y = y;
		this->z = z;
	}

	double getX()
	{
		return x;
	}

	double getY()
	{
		return y;
	}

	double getZ()
	{
		return z;
	}

	void translate(double dx, double dy, double dz)
	{
		x += dx;
		y += dy;
		z += dz;
	}

	void rotateZ(double angle)
	{
		double x2, y2;
		x2 = x * cos(angle) - y * sin(angle);
		y2 = x * sin(angle) + y * cos(angle);
		x = x2; y = y2;
	}

	void rotateX(double angle)
	{
		double y2, z2;
		y2 = y * cos(angle) - z * sin(angle);
		z2 = y * sin(angle) + z * cos(angle);
		y = y2; z = z2;
	}

	void rotateY(double angle)
	{
		double x2, z2;
		x2 = x * cos(angle) + z * sin(angle);
		z2 = -x * sin(angle) + z * cos(angle);
		x = x2; z = z2;
	}
};

void DisplayAxe() {
  int X, Y, Z;
  X = Y = 200;
  Z = 200;

  glLineWidth(2);

  // axa Ox - verde
  glColor3f(0, 1, 0);
  glBegin(GL_LINE_STRIP); 
    glVertex3f(0,0,0);
    glVertex3f(X,0,0);
  glEnd();

  // axa Oy - albastru
  glColor3f(0, 0, 1);
  glBegin(GL_LINE_STRIP); 
    glVertex3f(0,0,0);
    glVertex3f(0,Y,0);
  glEnd();

  // axa Oz - rosu
  glColor3f(1, 0, 0);
  glBegin(GL_LINE_STRIP); 
    glVertex3f(0,0,0);
    glVertex3f(0,0,Z);
  glEnd();

  glLineWidth(1);
}

// cub wireframe
void Display1() {
   glColor3f(1,0,0);
   glutWireCube(1);
}

// cub solid
void Display2() {
   glColor3f(1,0,0);
   glutSolidCube(1);
}

// sfera wireframe
void Display3() {
   glColor3f(0,0,1);
   glutWireSphere(1, 10, 10);
}

// sfera solida
void Display4() {
   glColor3f(0,0,1);
   glutSolidSphere(1, 10, 10);
}

void translate(Point *P[3], double dx, double dy, double dz)
{
	for(int i = 0; i < 3; ++i)
		P[i]->translate(dx, dy, dz);
}

void rotateX(Point *P[3], double angle)
{
	for(int i = 0; i < 3; ++i)
		P[i]->rotateX(angle);
}

void rotateY(Point *P[3], double angle)
{
	for(int i = 0; i < 3; ++i)
		P[i]->rotateY(angle);
}

void rotateZ(Point *P[3], double angle)
{
	for(int i = 0; i < 3; ++i)
		P[i]->rotateZ(angle);
}

void DisplayTrig(Point *P[3])
{
	glBegin(GL_LINES);
		for(int i = 0; i < 3; ++i)
		{
			glVertex3f(P[i]->getX(), P[i]->getY(), P[i]->getZ());
			glVertex3f(P[(i + 1) % 3]->getX(), P[(i + 1) % 3]->getY(), P[(i + 1) % 3]->getZ());
		}
	glEnd();
}

void doXZero(Point *P[3], double x, double y, double z)
{
	rotateZ(P, atan(-y / x) + PI / 2);
}

void doYZero(Point *P[3], double x, double y, double z)
{
	rotateX(P, atan(-z / y) + PI / 2);
}

void Display5() {
	Point *P[3];
	P[0] = new Point(0, 0, 0);
	P[1] = new Point(0, 0, 6);
	P[2] = new Point(0, 8, 0);

	translate(P, 2, -1, 3);
	rotateX(P, 0.8);
	rotateY(P, 1.1);
	rotateZ(P, 1.3);

	glColor3f(1,0.5,0);
	if(step == 0)
		DisplayTrig(P);

	translate(P, -P[0]->getX(), -P[0]->getY(), -P[0]->getZ());
	if(step == 1)
		DisplayTrig(P);

	doXZero(P, P[1]->getX(), P[1]->getY(), P[1]->getZ());
	if(step == 2)
		DisplayTrig(P);

	doYZero(P, P[1]->getX(), P[1]->getY(), P[1]->getZ());
	if(step == 3)
		DisplayTrig(P);

	doXZero(P, P[2]->getX(), P[2]->getY(), P[2]->getZ());
	if(step == 4)
		DisplayTrig(P);
}

void DisplayCube(Point *P[8])
{
	glBegin(GL_LINES);
		for(int i = 0; i < 4; ++i)
		{
			glVertex3f(P[i]->getX(), P[i]->getY(), P[i]->getZ());
			glVertex3f(P[(i + 1) % 4]->getX(), P[(i + 1) % 4]->getY(), P[(i + 1) % 4]->getZ());

			glVertex3f(P[i + 4]->getX(), P[i + 4]->getY(), P[i + 4]->getZ());
			glVertex3f(P[(i + 1) % 4 + 4]->getX(), P[(i + 1) % 4 + 4]->getY(), P[(i + 1) % 4 + 4]->getZ());

			glVertex3f(P[i]->getX(), P[i]->getY(), P[i]->getZ());
			glVertex3f(P[i + 4]->getX(), P[i + 4]->getY(), P[i + 4]->getZ());
		}
	glEnd();
}

void Display6() {
	Point *P[8];
	P[0] = new Point(0, 0, 0);
	P[1] = new Point(1, 0, 0);
	P[2] = new Point(1, 1, 0);
	P[3] = new Point(0, 1, 0);
	P[4] = new Point(0, 0, 1);
	P[5] = new Point(1, 0, 1);
	P[6] = new Point(1, 1, 1);
	P[7] = new Point(0, 1, 1);

	glColor3f(0.5,0,0.5);

	double rotangle = PI / 4;
	
	for(int i = 0; i < 8; ++i)
	{
		if(step2 <= 1)
			P[i]->rotateY(PI / 4);
		if(step2 <= 2)
			P[i]->rotateZ(-atan(1 / sqrt(2.0)));
		if(step2 <= 3)
			P[i]->rotateX(rotangle);
		if(step2 <= 4)
			P[i]->rotateZ(atan(1 / sqrt(2.0)));
		if(step2 <= 5)
			P[i]->rotateY(-PI / 4);
	}

	glColor3f(0,0,1);
	DisplayCube(P);
}

void DisplayObiect()
{
  switch (ob)
  {
  case cubw:
    Display1();
    break;
  case cubs:
    Display2();
    break;
  case sferaw:
    Display3();
    break;
  case sferas:
    Display4();
    break;
  case trig:
	Display5();
    break;
  case cubr:
	Display6();
    break;
  default:
    break;
  }
}

// rotatia cu un unghi de 10 grade in raport cu axa x
void DisplayX() {
  glMatrixMode(GL_MODELVIEW);
  glRotated(10,1,0,0);
}

// rotatia cu un unghi de 10 grade in raport cu axa y
void DisplayY() {
  glMatrixMode(GL_MODELVIEW);
  glRotated(10,0,1,0);
}

// rotatia cu un unghi de 10 grade in raport cu axa z
void DisplayZ() {
  glMatrixMode(GL_MODELVIEW);
  glRotated(10,0,0,1);
}

// Translatia cu 0.2, 0.2, 0.2
void DisplayT() {
  glMatrixMode(GL_MODELVIEW);
  glTranslatef(0.2, 0.2, 0.2);
}

// Scalarea cu 1.2, 1.2, 1.2
void DisplayS() {
  glMatrixMode(GL_MODELVIEW);
  glScalef(1.2, 1.2, 1.2);
}

void Init(void) {
  glClearColor(1, 1, 1, 1);
  glLineWidth(2);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(-10, 10, -10, 10, 30, -30);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glRotated(20, 1, 0, 0);
  glRotated(-20, 0, 1, 0);
}

void Display(void) {
  switch(prevKey) 
  {
  case 'a':
    DisplayAxe();
    break;
  case '0':
    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glRotated(20, 1, 0, 0);
    glRotated(-20, 0, 1, 0);
    break;
  case '1':
    Display1();
    ob = cubw;
    break;
  case '2':
    Display2();
    ob = cubs;
    break;
  case '3':
    Display3();
    ob = sferaw;
    break;
  case '4':
    Display4();
    ob = sferas;
    break;
  case '5':
    Display5();
	ob = trig;
	step = (step + 1) % 5;
    break;
  case '6':
    Display6();
	ob = cubr;
	step2 = (step2 + 1) % 6;
    break;
  case 'x':
    glClear(GL_COLOR_BUFFER_BIT);
    DisplayX();
    DisplayAxe();
    DisplayObiect();
    break;
  case 'y':
    glClear(GL_COLOR_BUFFER_BIT);
    DisplayY();
    DisplayAxe();
    DisplayObiect();
    break;
  case 'z':
    glClear(GL_COLOR_BUFFER_BIT);
    DisplayZ();
    DisplayAxe();
    DisplayObiect();
    break;
  case 't':
    glClear(GL_COLOR_BUFFER_BIT);
    DisplayT();
    DisplayAxe();
    DisplayObiect();
    break;
  case 's':
    glClear(GL_COLOR_BUFFER_BIT);
    DisplayS();
    DisplayAxe();
    DisplayObiect();
    break;
  default:
    break;
  }
  glutSwapBuffers();
}

void Reshape(int w, int h) {
   glViewport(0, 0, (GLsizei) w, (GLsizei) h);
}

void KeyboardFunc(unsigned char key, int x, int y) {
   prevKey = key;
   if (key == 27) // escape
      exit(0);
   glutPostRedisplay();
}

void MouseFunc(int button, int state, int x, int y) {
}

int main(int argc, char** argv) {

   glutInit(&argc, argv);
   
   glutInitWindowSize(dim, dim);

   glutInitWindowPosition(100, 100);

   glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB);

   glutCreateWindow (argv[0]);

   Init();

   glutReshapeFunc(Reshape);
   
   glutKeyboardFunc(KeyboardFunc);
   
   glutMouseFunc(MouseFunc);

   glutDisplayFunc(Display);
   
   glutMainLoop();

   return 0;
}
