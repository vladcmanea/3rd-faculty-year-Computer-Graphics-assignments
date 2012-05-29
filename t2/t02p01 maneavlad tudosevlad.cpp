#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "glut.h"

// dimensiunea ferestrei in pixeli
#define dim 300

unsigned char prevKey;

// concoida lui Nicomede (concoida dreptei)
// $x = a + b \cdot cos(t), y = a \cdot tg(t) + b \cdot sin(t)$. sau
// $x = a - b \cdot cos(t), y = a \cdot tg(t) - b \cdot sin(t)$. unde
// $t \in (-\pi / 2, \pi / 2)$
void Display1() {
double xmax, ymax, xmin, ymin;
double a = 1, b = 2;
double pi = 4 * atan(1.0);
double ratia = 0.05;
double t;

// calculul valorilor maxime/minime ptr. x si y
// aceste valori vor fi folosite ulterior la scalare
xmax = a - b - 1;
xmin = a + b + 1;
ymax = ymin = 0;
for (t = - pi/2 + ratia; t < pi / 2; t += ratia) {
double x1, y1, x2, y2;
x1 = a + b * cos(t);
xmax = (xmax < x1) ? x1 : xmax;
xmin = (xmin > x1) ? x1 : xmin;

x2 = a - b * cos(t);
xmax = (xmax < x2) ? x2 : xmax;
xmin = (xmin > x2) ? x2 : xmin;

y1 = a * tan(t) + b * sin(t);
ymax = (ymax < y1) ? y1 : ymax;
ymin = (ymin > y1) ? y1 : ymin;

y2 = a * tan(t) - b * sin(t);
ymax = (ymax < y2) ? y2 : ymax;
ymin = (ymin > y2) ? y2 : ymin;
}

xmax = (fabs(xmax) > fabs(xmin)) ? fabs(xmax) : fabs(xmin);
ymax = (fabs(ymax) > fabs(ymin)) ? fabs(ymax) : fabs(ymin);

// afisarea punctelor propriu-zise precedata de scalare
glColor3f(1,0.1,0.1); // rosu
glBegin(GL_LINE_STRIP); 
for (t = - pi/2 + ratia; t < pi / 2; t += ratia) {
double x1, y1, x2, y2;
x1 = (a + b * cos(t)) / xmax;
x2 = (a - b * cos(t)) / xmax;
y1 = (a * tan(t) + b * sin(t)) / ymax;
y2 = (a * tan(t) - b * sin(t)) / ymax;

glVertex2f(x1,y1);
}
glEnd();

glBegin(GL_LINE_STRIP); 
for (t = - pi/2 + ratia; t < pi / 2; t += ratia) {
double x1, y1, x2, y2;
x1 = (a + b * cos(t)) / xmax;
x2 = (a - b * cos(t)) / xmax;
y1 = (a * tan(t) + b * sin(t)) / ymax;
y2 = (a * tan(t) - b * sin(t)) / ymax;

glVertex2f(x2,y2);
}
glEnd();
}

// graficul functiei 
// $f(x) = \bar sin(x) \bar \cdot e^{-sin(x)}, x \in \langle 0, 8 \cdot \pi \rangle$, 
void Display2() {
double pi = 4 * atan(1.0);
double xmax = 8 * pi;
double ymax = exp(1.1);
double ratia = 0.05;

// afisarea punctelor propriu-zise precedata de scalare
glColor3f(1,0.1,0.1); // rosu
glBegin(GL_LINE_STRIP); 
for (double x = 0; x < xmax; x += ratia) {
double x1, y1;
x1 = x / xmax;
y1 = (fabs(sin(x)) * exp(-sin(x))) / ymax;

glVertex2f(x1,y1);
}
glEnd();
}

// 2.1. Functia
void Display3() {
double xmax = 100;
double ratia = 0.05;

glColor3f(1, 0, 0);
glBegin(GL_LINE_STRIP);

for (double x = 0; x < xmax; x += ratia) {
double x1, y1;
x1 = x / xmax;
if (x1 == 0) {
y1 = 1;
} else {
double d = (x - floor(x) < ceil(x) - x? x - floor(x): ceil(x) - x);
y1 = d / x;
}
glVertex2f(x1, y1);
}

glEnd();
}

// 2.2.1. Melcul lui Pascal
void Display4() {
double pi = 4 * atan(1.0);
double ratia = 0.01;
double a = 0.3, b = 0.2;
/*
printf("Dati a: ");
scanf("%lf", &a);
printf("Dati b: ");
scanf("%lf", &b);
*/

glColor3f(0, 0, 0);
char message[100];

sprintf(message, "Melcul lui Pascal");
glRasterPos2f(-0.8, 0.8);
for (char *c = message; *c != '\0'; c++) {
glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, *c);
}

sprintf(message, "a = %.1lf", a);
glRasterPos2f(-0.8, 0.7);
for (char *c = message; *c != '\0'; c++) {
glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, *c);
}

sprintf(message, "b = %.1lf", b);
glRasterPos2f(-0.8, 0.6);
for (char *c = message; *c != '\0'; c++) {
glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, *c);
}

glColor3f(1, 0, 0);
glBegin(GL_LINE_STRIP);

for (double t = -pi + ratia; t < pi; t += ratia) {
double x1 = 2 * (a * cos(t) + b) * cos(t);
double y1 = 2 * (a * cos(t) + b) * sin(t);
glVertex2f(x1, y1);
}

glEnd();
}

// 2.2.2. Trisectoarea lui Longchamps
void Display5() {
double pi2 = 2 * atan(1.0);
double ratia = 0.05;
double a = 0.2;
/*
printf("Dati a: ");
scanf("%lf", &a);
*/

glColor3f(0, 0, 0);
char message[100];

sprintf(message, "Trisectoarea lui Longchamps");
glRasterPos2f(0, 0.8);
for (char *c = message; *c != '\0'; c++) {
glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, *c);
}

sprintf(message, "a = %.1lf", a);
glRasterPos2f(0, 0.7);
for (char *c = message; *c != '\0'; c++) {
glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, *c);
}

glColor3f(1, 0, 0);
glBegin(GL_LINE_STRIP);

for (double t = -pi2 + ratia; t < pi2; t += ratia) {
if (abs(t) != pi2 / 3) {
double x1 = a / (4 * pow(cos(t), 2) - 3);
double y1 = a * tan(t) / (4 * pow(cos(t), 2) - 3);
glVertex2f(x1, y1);
}
}

glEnd();
}

// 2.2.3. Cicloida
void Display6() {
double ratia = 0.05;
double xmax = 10;
double a = 0.1;
double b = 0.2;

/*
printf("Dati a: ");
scanf("%lf", &a);
printf("Dati b: ");
scanf("%lf", &b);
*/

glColor3f(0, 0, 0);
char message[100];

sprintf(message, "Cicloida");
glRasterPos2f(-0.5, -0.5);
for (char *c = message; *c != '\0'; c++) {
glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, *c);
}

sprintf(message, "a = %.1lf", a);
glRasterPos2f(-0.5, -0.6);
for (char *c = message; *c != '\0'; c++) {
glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, *c);
}

sprintf(message, "b = %.1lf", b);
glRasterPos2f(-0.5, -0.7);
for (char *c = message; *c != '\0'; c++) {
glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, *c);
}

glColor3f(0.9, 0.3, 0);
glBegin(GL_LINE_STRIP);

for (double t = -xmax; t <= xmax; t += ratia) {
double x1 = a * t - b * sin(t);
double y1 = a - b * cos(t);
glVertex2f(x1, y1);
}

glEnd();
}

// 2.2.4. Epicicloida
void Display7() {
double _2pi = 8 * atan(1.0);
double ratia = 0.05;
double R = 0.1;
double r = 0.3;

/*
printf("Dati R: ");
scanf("%lf", &R);
printf("Dati r: ");
scanf("%lf", &r);
*/

glColor3f(0, 0, 0);
char message[100];

sprintf(message, "Epicicloida");
glRasterPos2f(0.1, -0.8);
for (char *c = message; *c != '\0'; c++) {
glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, *c);
}

sprintf(message, "R = %.1lf   r = %.1lf", R, r);
glRasterPos2f(0.1, -0.9);
for (char *c = message; *c != '\0'; c++) {
glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, *c);
}

glColor3f(0.9, 0.3, 0);
glBegin(GL_LINE_STRIP);

for (double t = 0; t <= _2pi; t += ratia) {
double pr = r * t / R;
double x1 = (R + r) * cos(pr) - r * cos(t + pr);
double y1 = (R + r) * sin(pr) - r * sin(t + pr);
glVertex2f(x1, y1);
}

glEnd();
}

// 2.2.5. Hipocicloida
void Display8() {
double _2pi = 8 * atan(1.0);
double ratia = 0.05;
double R = 0.1;
double r = 0.3;

/*
printf("Dati R: ");
scanf("%lf", &R);
printf("Dati r: ");
scanf("%lf", &r);
*/

glColor3f(0, 0, 0);
char message[100];

sprintf(message, "Hipocicloida");
glRasterPos2f(0.1, -0.6);
for (char *c = message; *c != '\0'; c++) {
glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, *c);
}

sprintf(message, "R = %.1lf", R);
glRasterPos2f(0.1, -0.7);
for (char *c = message; *c != '\0'; c++) {
glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, *c);
}

sprintf(message, "r = %.1lf", r);
glRasterPos2f(0.1, -0.8);
for (char *c = message; *c != '\0'; c++) {
glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, *c);
}

glColor3f(1, 0, 0);
glBegin(GL_LINE_STRIP);

for (double t = 0; t <= _2pi; t += ratia) {
double pr = r * t / R;
double x1 = (R - r) * cos(pr) - r * cos(t - pr);
double y1 = (R - r) * sin(pr) - r * sin(t - pr);
glVertex2f(x1, y1);
}

glEnd();
}

// 3.1. Lemniscata lui Bernoulli
void Display9() {
double pi4 = atan(1.0);
double ratia = 0.001;
double a = 0.4;

/*
printf("Dati a: ");
scanf("%lf", &a);
*/

glColor3f(0, 0, 0);
char message[100];

sprintf(message, "Leminiscata lui Bernoulli");
glRasterPos2f(-0.5, -0.7);
for (char *c = message; *c != '\0'; c++) {
glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, *c);
}

sprintf(message, "a = %.1lf", a);
glRasterPos2f(-0.5, -0.8);
for (char *c = message; *c != '\0'; c++) {
glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, *c);
}

glColor3f(1, 0.5, 0.5);
glBegin(GL_LINE_STRIP);

for (double t = -pi4 + ratia; t < pi4; t += ratia) {
double r = a * sqrt(2 * cos(2 * t));
double x1 = r * cos(t);
double y1 = r * sin(t);
glVertex2f(x1, y1);
}

for (double t = pi4 - ratia; t > -pi4; t -= ratia) {
double r = -a * sqrt(2 * cos(2 * t));
double x1 = r * cos(t);
double y1 = r * sin(t);
glVertex2f(x1, y1);
}

glEnd();
}

// 3.2. Spirala logaritmica
void Display0() {
double ratia = 0.05;
double xmax = 10;
double a = 0.02;

/*
printf("Dati a: ");
scanf("%lf", &a);
*/

glColor3f(0, 0, 0);
char message[100];

sprintf(message, "Spirala logaritmica");
glRasterPos2f(-0.5, -0.6);
for (char *c = message; *c != '\0'; c++) {
glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, *c);
}

sprintf(message, "a = %.2lf", a);
glRasterPos2f(-0.5, -0.7);
for (char *c = message; *c != '\0'; c++) {
glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, *c);
}

glColor3f(1, 0.5, 0.5);
glBegin(GL_LINE_STRIP);

for (double t = ratia; t < xmax; t += ratia) {
double r = a * exp(1 + t);
double x1 = r * cos(t);
double y1 = r * sin(t);
glVertex2f(x1, y1);
}

glEnd();
}

void f(int cadran, double x, double y, double r) {
	double ratia = 0.01;
	switch(cadran)
	{
	// cadranul 0, nord est
	case 0:
		for (double xx = 0; xx <= r; xx += ratia) {
			double yy = sqrt(r * r - xx * xx);
			glVertex2f(xx + x, yy + y);
		}
		break;
	// cadranul 1, sud est
	case 1:
		for (double xx = r; xx >= 0; xx -= ratia) {
			double yy = sqrt(r * r - xx * xx);
			glVertex2f(xx + x, -yy + y);
		}
		break;
	// cadranul 2, sud vest
	case 2:
		for (double xx = 0; xx <= r; xx += ratia) {
			double yy = sqrt(r * r - xx * xx);
			glVertex2f(-xx + x, -yy + y);
		}
		break;
	// cadranul 3, nord vest
	case 3:
		for (double xx = r; xx >= 0; xx -= ratia) {
			double yy = sqrt(r * r - xx * xx);
			glVertex2f(-xx + x, yy + y);
		}
		break;
	}
}

void c(int i, double &X, double &Y, double v)
{
	switch(i)
		{
		case 0:
			Y -= v;
			break;
		case 1:
			X -= v;
			break;
		case 2:
			Y += v;
			break;
		case 3:
			X += v;
			break;
		}
}

// Fibonacci curve
void Displayf()
{
	double nmax = 20;
	double F0 = 0.01;
	double F1 = 0.01;
	double F2;

	glColor3f(0, 0, 0);
	char message[100];

	sprintf(message, "Curba lui Fibonacci");
	glRasterPos2f(-0.9, 0.9);
	for (char *c = message; *c != '\0'; c++) {
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, *c);
	}

	glBegin(GL_LINE_STRIP);
	glColor3f(1, 0.5, 0.5);

	double X = 0, Y = 0;

	f(0, X, Y, F0);
	f(1, X, Y, F1);

	for (int i = 2; i <= nmax; ++i)
	{
		c(i % 4, X, Y, F0);

		F2 = F0 + F1;
		F0 = F1;
		F1 = F2;
		
		f(i % 4, X, Y, F2);
	}


	glEnd();
}

void Init(void) {

glClearColor(1.0,1.0,1.0,1.0);

glLineWidth(1);

//   glPointSize(4);

glPolygonMode(GL_FRONT, GL_LINE);
}

void Display(void) {
glClear(GL_COLOR_BUFFER_BIT);

switch(prevKey) {
case '1':
Display1();
break;
case '2':
Display2();
break;
case '3':
Display3();
break;
case '4':
Display4();
break;
case '5':
Display5();
break;
case '6':
Display6();
break;
case '7':
Display7();
break;
case '8':
Display8();
break;
case '9':
Display9();
break;
case '0':
Display0();
break;
case 'f':
Displayf();
break;
default:
break;
}

glFlush();
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

glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB);

glutCreateWindow (argv[0]);

Init();

glutReshapeFunc(Reshape);

glutKeyboardFunc(KeyboardFunc);

glutMouseFunc(MouseFunc);

glutDisplayFunc(Display);

glutMainLoop();

return 0;
}
