#ifndef _LINES_H
#define _LINES_H

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include "glut.h"
#include <vector>
#include <map>
#include <algorithm>
using namespace std;

//#define LINES_INPUT "lines.txt"
//#define ELLIPSE_INPUT "ellipse.txt"
//#define CIRCLE_INPUT "circle.txt"
#define POLYGON_INPUT "polygon.txt"

#define EPS 1e-4

#define min(x, y) ((x) < (y)? (x): (y))
#define max(x, y) ((x) > (y)? (x): (y))


struct Point
{
	int x,y;
};

struct Edge
{
	Point pi,pf;
};

typedef vector<Edge> Polygon;

struct Intersection
{
	int ymax;
	double xmin;
	double ratio;
};

typedef vector<Intersection> Intersections;

typedef vector<Intersections> ET;

Polygon polygon;

class Lines
{
	// number of rows
	unsigned N;

	// number of cols
	unsigned M;

	// margin
	double G;

	// precision of circle
	unsigned P;

	// draw grid
	void writeGrid();

	// draw point
	void writePixel(unsigned, unsigned);

	// switch pixels
	void switchPixels(unsigned &, unsigned &, unsigned &, unsigned &);

	// show symmetric circle points
	void showCircle(unsigned, unsigned, unsigned, unsigned, unsigned);

	ET computeET(Polygon);

	ET computessm(Polygon, ET);

	public:
	void gridToDouble(unsigned,unsigned,double&,double&);
		
	// constructor
	Lines(unsigned, unsigned, double, unsigned);

	// draw line
	void writeLine(unsigned, unsigned, unsigned, unsigned, unsigned);

	// draw circle
	void writeCircle(unsigned, unsigned, unsigned, unsigned);

	void drawPolygon(Polygon);

	void fillEllipse(int, int, int, int);

};

#endif

Lines::Lines(unsigned N, unsigned M, double G, unsigned P)
{
	if (N < 2)
		throw "Number of lines must be [2, ...]";

	if (M < 2)
		throw "Number of columns must be [2, ...]";

	if (P < 3)
		throw "Precision of polygon must be [3, ...]";

	if (G > 1)
		throw "Percent of window for border must be [0, ..., 1]";

	this -> N = N;
	this -> M = M;
	this -> G = G;
	this -> P = P;
	writeGrid();
}

void Lines::writeGrid()
{
	const double dx = (2 - 2 * G) / (M - 1);
	const double dy = (2 - 2 * G) / (N - 1);

	glColor3f(0.9,0.9,0.9);
	glLineWidth(2.0);
	glBegin(GL_LINES);

	// draw columns
	for (int j = 1; j <= M; ++j)
	{
		double X = (j - 1) * dx + G - 1;
		glVertex2f(X, 1 - G); 
		glVertex2f(X, G - 1);
	}

	// draw lines
	for (int i = 1; i <= N; ++i)
	{
		double Y = 1 - G - (i - 1) * dy;
		glVertex2f(G - 1, Y); 
		glVertex2f(1 - G, Y);
	}

	glEnd();
}

void Lines::switchPixels(unsigned &Xi, unsigned &Yi, unsigned &Xf, unsigned &Yf)
{
	int aux;
	aux = Xf, Xf = Xi, Xi = aux;
	aux = Yf, Yf = Yi, Yi = aux;
}

void Lines::writeLine(unsigned Xi, unsigned Yi, unsigned Xf, unsigned Yf, unsigned thickness)
{
	if (Xi < 1 || Xi > M)
		printf("Initial X coordinate must be [1, ..., M]\n");

	if (Xf < 1 || Xf > M)
		printf("Final X coordinate must be [1, ..., M]\n");

	if (Yi < 1 || Yi > N)
		printf("Initial Y coordinate must be [1, ..., N]\n");

	if (Yf < 1 || Yf > N)
		printf("Final Y coordinate must be [1, ..., N]\n");

	glColor3f(0.9,0.0,0.0);
	glBegin(GL_LINES);
		double x,y;
		gridToDouble(Xi,Yi,x,y);
		glVertex2f(x,y);
		gridToDouble(Xf,Yf,x,y);
		glVertex2f(x,y);
	glEnd();

	if (Xi == Xf && Yi == Yf)
		writePixel(Xi, Yi);
	else if (Xi == Xf)
		for (int y = min(Yi, Yf); y <= max(Yi, Yf); ++y)
			for (int s = 0; s < thickness; ++s)
				writePixel(Xi - thickness / 2 + s, y);
	else if (Yi == Yf)
		for (int x = min(Xi, Xf); x <= max(Xi, Xf); ++x)
			for (int s = 0; s < thickness; ++s)
				writePixel(x, Yi - thickness / 2 + s);					
	else
	{
		int dx = Xf - Xi;
		int dy = Yf - Yi;

		// positive slope
		if (dy / abs(dy) == dx / abs(dx))
		{
			// slope between [0 and 1] ======================== OK
			if (abs(dy) <= abs(dx))
			{
				if (Xi > Xf)
					switchPixels(Xi, Yi, Xf, Yf);

				int dx = Xf - Xi;
				int dy = Yf - Yi;
				int d = 2 * dy - dx;
				int dE = 2 * dy;
				int dNE = 2 * (dy - dx);
				
				for (int x = Xi, y = Yi; x <= Xf; ++x)
				{
					for (int s = 0; s < thickness; ++s)
						writePixel(x, y - thickness / 2 + s);					
					if (d <= 0) d += dE;
					else d += dNE, ++y;
				}
			}

			// slope between (1 and oo) ======================= OK
			else
			{
				if (Yi > Yf)
					switchPixels(Xi, Yi, Xf, Yf);

				int dx = Xf - Xi;
				int dy = Yf - Yi;
				int d = 2 * dx - dy;
				int dE = 2 * dx;
				int dNE = 2 * (dx - dy);
				
				for (int y = Yi, x = Xi; y <= Yf; ++y)
				{
					for (int s = 0; s < thickness; ++s)
						writePixel(x - thickness / 2 + s, y);
					if (d <= 0) d += dE;
					else d += dNE, ++x;
				}
			}
		}

		// negative slope
		else
		{
			// slope between [0 and -1] ====================== OK
			if (abs(dy) <= abs(dx))
			{
				if (Xi > Xf)
					switchPixels(Xi, Yi, Xf, Yf);

				int dx = Xf - Xi;
				int dy = -Yf + Yi;
				int d = 2 * dy - dx;
				int dE = 2 * dy;
				int dNE = 2 * (dy - dx);
				
				for (int x = Xi, y = Yi; x <= Xf; ++x)
				{
					for (int s = 0; s < thickness; ++s)
						writePixel(x, y - 2 * (y - Yi) - thickness / 2 + s);					
					if (d <= 0) d += dE;
					else d += dNE, ++y;
				}
			}

			// slope between (-1 and -oo) ==================== OK
			else
			{
				if (Yi > Yf)
					switchPixels(Xi, Yi, Xf, Yf);

				int dx = -Xf + Xi;
				int dy = Yf - Yi;
				int d = 2 * dx - dy;
				int dE = 2 * dx;
				int dNE = 2 * (dx - dy);
				
				for (int y = Yi, x = Xi; y <= Yf; ++y)
				{
					for (int s = 0; s < thickness; ++s)
						writePixel(x - 2 * (x - Xi) - thickness / 2 + s, y);
					if (d <= 0) d += dE;
					else d += dNE, ++x;
				}
			}
		}
	}
}

void Lines::showCircle(unsigned x, unsigned y, unsigned Xi, unsigned Yi, unsigned thickness)
{
	if (thickness > 1)
	{
		writeCircle(Xi + x, Yi + y, thickness / 2, 1);
		writeCircle(Xi + x, Yi - y, thickness / 2, 1);
		writeCircle(Xi - x, Yi + y, thickness / 2, 1);
		writeCircle(Xi - x, Yi - y, thickness / 2, 1);

		if (x != y)
		{
			writeCircle(Xi + y, Yi + x, thickness / 2, 1);
			writeCircle(Xi + y, Yi - x, thickness / 2, 1);
			writeCircle(Xi - y, Yi + x, thickness / 2, 1);
			writeCircle(Xi - y, Yi - x, thickness / 2, 1);
		}
	}
	else
	{
		// AICI, daca punem 0 in loc de x si y in instructiunile for, vor disparea pixelii albi de pe contur!
		for (int xx = x; xx <= x; ++xx)
			for (int yy = y; yy <= y; ++yy)
			{
				writePixel(Xi + xx, Yi + yy);
				writePixel(Xi + xx, Yi - yy);
				writePixel(Xi - xx, Yi + yy);
				writePixel(Xi - xx, Yi - yy);
	
				if (xx != yy)
				{
					writePixel(Xi + yy, Yi + xx);
					writePixel(Xi + yy, Yi - xx);
					writePixel(Xi - yy, Yi + xx);
					writePixel(Xi - yy, Yi - xx);
				}
			}
	}
}

void Lines::writeCircle(unsigned Xi, unsigned Yi, unsigned R, unsigned thickness)
{
	if (Xi < 1 || Xi > M)
		printf("Initial X coordinate must be [1, ..., M]\n");

	if (Yi < 1 || Yi > N)
		printf("Initial Y coordinate must be [1, ..., N]\n");

	// int x = 0, y = R, d = 1 - R, dE = 3, dSE = 5 - 2 * R;
	int x = R, y = 0, d = 1 - R, dE = 3, dSE = 5 - 2 * R;
	
	showCircle(x, y, Xi, Yi, thickness); //

	while (y < x)
	{
		if (d < 0)
		{
			d += dE;
			dSE += 2;
		}
		else
		{
			d += dSE;
			dSE += 4;
			--x;
		}

		++y;
		dE += 2;
		showCircle(x, y, Xi, Yi, thickness);
	}
}

void Lines::gridToDouble(unsigned x, unsigned y, double& xd, double& yd)
{
	double dx = (2 - 2 * G) / (M - 1);
	double dy = (2 - 2 * G) / (N - 1);
	xd = G - 1 + (x - 1) * dx;
	yd = G - 1 + (y - 1) * dy;
}

void Lines::writePixel(unsigned X, unsigned Y)
{
	glColor3f(0.0,0.0,0.0);
	if (!(1 <= X && X <= M && 1 <= Y && Y <= N))
		return;

	float *C, *S;

	const double dx = (2 - 2 * G) / (M - 1);
	const double dy = (2 - 2 * G) / (N - 1);

	const float Xf = G - 1 + (X - 1) * dx;
	const float Yf = G - 1 + (Y - 1) * dy;

	C = (float*)malloc(sizeof(float) * P);
	S = (float*)malloc(sizeof(float) * P);

	for (int i = 0; i < P; ++i)
	{
		S[i] = sin(8 * atan(1.0) * i / P);
		C[i] = cos(8 * atan(1.0) * i / P);
	}

	// trasare poligon convex GL_QUADS : (v0,v1,v2, ..., v_{n-1})
	glBegin(GL_POLYGON);
	for (int i = 0; i < P; ++i)
		glVertex2f(min(dx / 6, G / 2) * S[i] + Xf, min(dy / 6, G / 2) * C[i] + Yf); 
	glEnd();

	free(C);
	free(S);
}

int cmp(Intersection i1, Intersection i2)
{
	return i1.xmin<i2.xmin;
}

ET Lines::computeET(Polygon polygon)
{
	ET et;
	for(int y=0;y<N;++y)
	{
		Intersections inter;
		et.push_back(inter);
	}

	for(int i=0;i<polygon.size();++i)
	{
		Edge m=polygon[i];
		if(m.pi.y == m.pf.y)
			continue;
		int ym,yM,xm,xM;
		ym = min(m.pi.y,m.pf.y);
		yM = max(m.pi.y,m.pf.y);
		xm = (ym == m.pi.y) ? m.pi.x : m.pf.x;
		xM = (yM == m.pi.y) ? m.pi.x : m.pf.x;
		Intersection inter;
		inter.ymax=yM;
		inter.xmin=xm;
		inter.ratio=(double)(xm-xM)/(ym-yM);
		et[ym].push_back(inter);
	}

	for(int y=0;y<N;++y)
		sort(et[y].begin(),et[y].end(),cmp);
	return et;
}

ET Lines::computessm(Polygon polygon, ET et)
{
	ET ssm;
	Intersections aet;

	for(int y=0;y<N;++y)
	{
		for(int i=0;i<et[y].size();++i)
			aet.push_back(et[y][i]);
		Intersections aet2;
		for(int i=0;i<aet.size();++i)
			if(aet[i].ymax!=y)
				aet2.push_back(aet[i]);
		aet=aet2;
		sort(aet.begin(),aet.end(),cmp);
		Intersections inter;
		ssm.push_back(inter);
		for(int i=0;i<aet.size();++i)
		{
			ssm[y].push_back(aet[i]);
			aet[i].xmin+=aet[i].ratio;
		}
	}

	return ssm;
}

void Lines::drawPolygon(Polygon polygon)
{
	ET et = computeET(polygon);
	ET ssm = computessm(polygon,et);

	for(int y=0;y<N;++y)
	{
		for(int i=0;i<ssm[y].size();i+=2)
		{
			for(int x=ceil(ssm[y][i].xmin);x<ssm[y][i+1].xmin;++x)
				writePixel(x+1,y+1);
			if(ssm[y].size()==2 && abs(ssm[y][0].xmin-ssm[y][1].xmin)<EPS)
				writePixel(floor(ssm[y][0].xmin+0.5)+1,y+1);
		}
	}
}

void Lines::fillEllipse(int x0, int y0, int a, int b)
{
	int x=0,y=-b;
	double fxpyp = 0.0;
	double deltaV=b*b*(-2*x+1);
	double deltaN=a*a*(2*y+1);
	double deltaNV=deltaV+deltaN;
	map<int,int> M;

	M[y]=x;

	//regiunea1
	while(-a*a*(y-0.5) > (-x+1)*b*b)
	{
		deltaV=b*b*(-2*x+1);
		deltaN=a*a*(2*y+1);
		deltaNV=deltaV+deltaN;
		if(fxpyp+deltaV <= 0.0)
		{
			fxpyp+=deltaV;
			--x;
			M[y]=x;
		}
		else if(fxpyp + deltaNV <= 0.0)
		{
			fxpyp+=deltaNV;
			--x; ++y;
			M[y]=x;
		}
		else
		{
			fxpyp+=deltaN;
			++y;
		}
	}

	//regiunea2
	while(y<=0)
	{
		deltaN=a*a*(2*y+1);
		deltaNV=deltaV+deltaN;
		if(fxpyp+deltaNV <= 0.0)
		{
			fxpyp+=deltaNV;
			--x; ++y;
		}
		else
		{
			fxpyp+=deltaN;
			++y;
		}
		M[y]=x;
	}

	for(y=-b;y<=0;++y)
		for(x=0;x>=M[y];--x)
			writePixel(x+x0,y+y0);
}


// Daca se doreste utilizarea bibliotecii GLUT trebuie
// inclus fisierul header GL/glut.h (acesta va include
// la GL/gl.h si GL/glu.h, fisierele header pentru
// utilizarea bibliotecii OpenGL). Functiile din biblioteca
// OpenGL sunt prefixate cu gl, cele din GLU cu glu si
// cele din GLUT cu glut.

// #include <GL/glut.h>

#define PI (3.141592653589793)
float *S;
float *C;

unsigned N=16, M=16, P=16, T=0, U=0;
unsigned *Xi, *Xf, *Yi, *Yf, *Th;
unsigned *Xc, *Yc, *Rc, *Tc;
double G=0.05;

#include "glut.h"
unsigned char prevKey;

void DisplayA(int X) {

	/*
	Se tasteaza a, b, c, ..., y, z.
	a => triunghi echilateral
	b => patrat
	c => pentagrama regulata
	etc.
	*/

	float *C, *S;

	C = (float*)malloc(sizeof(float) * X);
	S = (float*)malloc(sizeof(float) * X);

	for (int i = 0; i < X; ++i)
	{
		S[i] = sin(2 * PI * i / X);
		C[i] = cos(2 * PI * i / X);
	}
   // trasare poligon convex GL_QUADS : (v0,v1,v2, ..., v_{n-1})
   glColor3f(0.2,0.15,0.88); // albastru
   glBegin(GL_POLYGON);
		for (int i = 0; i < X; ++i)
			glVertex2f(0.8 * S[i], 0.8 * C[i]); 
   glEnd();
   glColor3f(1.0,1.0,1.0); // alb
   glBegin(GL_POLYGON);
		for (int i = 0; i < X; ++i)
			glVertex2f(0.6 * S[i], 0.6 * C[i]); 
   glEnd();
   glLineWidth(3.0);
   glColor3f(1.0,0.0,0.0); // rosu
   glBegin(GL_POLYGON);
		for (int i = X - 1; i >= 0; --i)
			glVertex2f(0.6 * S[i], 0.6 * C[i]); 
   glEnd(); 

   free(C);
   free(S);
}

void Init(void) {
   // specifica culoarea unui buffer dupa ce acesta
   // a fost sters utilizand functia glClear. Ultimul
   // argument reprezinta transparenta (1 - opacitate
   // completa, 0 - transparenta totala)
   glClearColor(1.0,1.0,1.0,1.0);

   // grosimea liniilor
   glLineWidth(3);

   // dimensiunea punctelor
   glPointSize(4);

   // functia void glPolygonMode (GLenum face, GLenum mode)
   // controleaza modul de desenare al unui poligon
   // mode : GL_POINT (numai vf. primitivei) GL_LINE (numai
   //        muchiile) GL_FILL (poligonul plin)
   // face : tipul primitivei geometrice dpdv. al orientarii
   //        GL_FRONT - primitive orientate direct
   //        GL_BACK  - primitive orientate invers
   //        GL_FRONT_AND_BACK  - ambele tipuri
   glPolygonMode(GL_FRONT, GL_LINE);
}

void randomizeColor()
{
	float red = (float)rand() / RAND_MAX / 1.33;
	float green = (float)rand() / RAND_MAX / 1.33;
	float blue = (float)rand() / RAND_MAX / 1.33;
	glColor3f(red, green, blue); // random color
}

void Display1()
{
	Lines lines(16,16,0.05,16);
	lines.writeLine(1,16,16,11,3);
	lines.writeLine(1,1,16,8,1);
}

void Display2()
{
	Lines lines(32,32,0.05,16);
	lines.writeCircle(16,16,10,2);
}

void Display3()
{
	Lines lines(26,26,0.05,16);
	lines.fillEllipse(14,8,13,7);
}

void readPolyg();

void Display4()
{
	readPolyg();

	double x,y;
	Lines lines(20,20,0.05,16);

	glColor3f(0.9, 0.1, 0.1);
	glBegin(GL_LINE_LOOP);
	for(int i=0;i<polygon.size();++i)
	{
		lines.gridToDouble(polygon[i].pi.x+1,polygon[i].pi.y+1,x,y);
		glVertex2f(x,y);
	}
	glEnd();

	lines.drawPolygon(polygon);
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
    default:
      break;
   }

   glFlush();
}

	// sterge buffer-ul indicat
	//glClear(GL_COLOR_BUFFER_BIT);

	// create lines
	/*
	Lines lines(N, M, G, P);
	srand(time(0));

	// draw lines
	for (int t = 0; t < T; ++t)
	{
		randomizeColor();
		lines.writeLine(Xi[t], Yi[t], Xf[t], Yf[t], Th[t]);
	}

	// draw circles
	for (int u = 0; u < U; ++u)
	{
		randomizeColor();
		lines.writeCircle(Xc[u], Yc[u], Rc[u], Tc[u]);
	}
	*/

	/*
	double dx = (2 - 2 * G) / (M - 1);
	double dy = (2 - 2 * G) / (N - 1);

	glColor3f(0.9, 0.1, 0.1);
	glBegin(GL_LINE_LOOP);
	for(int i=0;i<polygon.size();++i)
	{
		double Xf = G - 1 + polygon[i].pi.x * dx;
		double Yf = G - 1 + polygon[i].pi.y * dy;
		glVertex2f(Xf,Yf);
	}
	glEnd();
	

	glColor3f(0.0, 0.0, 0.0);
	lines.drawPolygon(polygon);
	*/

	//Lines lines(32,32,0.05,16);
	//lines.writeCircle(14,14,12,1);
	
	// forteaza redesenarea imaginii
	//glFlush();
//}

/*
   Parametrii w(latime) si h(inaltime) reprezinta noile
   dimensiuni ale ferestrei
*/
void Reshape(int w, int h) {
   printf("Call Reshape : latime = %d, inaltime = %d\n", w, h);

   // functia void glViewport (GLint x, GLint y,
   //                          GLsizei width, GLsizei height)
   // defineste poarta de afisare : acea suprafata dreptunghiulara
   // din fereastra de afisare folosita pentru vizualizare.
   // x, y sunt coordonatele pct. din stg. jos iar 
   // width si height sunt latimea si inaltimea in pixeli.
   // In cazul de mai jos poarta de afisare si fereastra coincid
   glViewport(0, 0, (GLsizei) w, (GLsizei) h);
}

/* 
   Parametrul key indica codul tastei iar x, y pozitia
   cursorului de mouse
*/
void KeyboardFunc(unsigned char key, int x, int y) {
   printf("Ati tastat <%c>. Mouse-ul este in pozitia %d, %d.\n",
            key, x, y);
   // tasta apasata va fi utilizata in Display ptr.
   // afisarea unor imagini
   prevKey = key;
   if (key == 27) // escape
      exit(0);
   glutPostRedisplay();
}

/* 
   Codul butonului poate fi :
   GLUT_LEFT_BUTTON, GLUT_MIDDLE_BUTTON, GLUT_RIGHT_BUTTON
   Parametrul state indica starea: "apasat" GLUT_DOWN sau
   "eliberat" GLUT_UP
   Parametrii x, y : coordonatele cursorului de mouse
*/
void MouseFunc(int button, int state, int x, int y) {
   printf("Call MouseFunc : ati %s butonul %s in pozitia %d %d\n",
      (state == GLUT_DOWN) ? "apasat" : "eliberat",
      (button == GLUT_LEFT_BUTTON) ? 
      "stang" : 
      ((button == GLUT_RIGHT_BUTTON) ? "drept": "mijlociu"),
      x, y);
}

void allocate()
{
	//freopen(INFILE, "r", stdin);

	scanf("%lf", &G);
	scanf("%u", &N);
	scanf("%u", &M);
	scanf("%u", &P);
	scanf("%u", &T);

	if (!(Xi = (unsigned*)malloc(T * sizeof(unsigned))))
		throw "main: not enough memory";

	if (!(Xf = (unsigned*)malloc(T * sizeof(unsigned))))
		throw "main: not enough memory";

	if (!(Yi = (unsigned*)malloc(T * sizeof(unsigned))))
		throw "main: not enough memory";

	if (!(Yf = (unsigned*)malloc(T * sizeof(unsigned))))
		throw "main: not enough memory";

	if (!(Th = (unsigned*)malloc(T * sizeof(unsigned))))
		throw "main: not enough memory";

	for (int t = 0; t < T; ++t)
	{
		scanf("%u", &Xi[t]);
		scanf("%u", &Yi[t]);
		scanf("%u", &Xf[t]);
		scanf("%u", &Yf[t]);
		scanf("%u", &Th[t]);
	}

	scanf("%u", &U);

	if (!(Xc = (unsigned*)malloc(U * sizeof(unsigned))))
		throw "main: not enough memory";

	if (!(Yc = (unsigned*)malloc(U * sizeof(unsigned))))
		throw "main: not enough memory";

	if (!(Rc = (unsigned*)malloc(U * sizeof(unsigned))))
		throw "main: not enough memory";

	if (!(Tc = (unsigned*)malloc(U * sizeof(unsigned))))
		throw "main: not enough memory";

	for (int u = 0; u < U; ++u)
	{
		scanf("%u", &Xc[u]);
		scanf("%u", &Yc[u]);
		scanf("%u", &Rc[u]);
		scanf("%u", &Tc[u]);
	}
}

void deallocate()
{
	free(Th);
	free(Xi);
	free(Yi);
	free(Xf);
	free(Yf);
	free(Xc);
	free(Yc);
	free(Rc);
	free(Tc);
}

void readPolyg()
{
	int n;
	Point P[1000];

	freopen(POLYGON_INPUT,"r",stdin);
	scanf("%d",&n);

	for(int i=0;i<n;++i)
		scanf("%d%d",&P[i].x,&P[i].y);
	for(int i=0;i<n;++i)
	{
		Edge e;
		e.pi=P[i];
		e.pf=P[(i+1)%n];
		polygon.push_back(e);
	}
}

int main(int argc, char** argv) {

	// read
	//allocate();

   // Initializarea bibliotecii GLUT. Argumentele argc
   // si argv sunt argumentele din linia de comanda si nu 
   // trebuie modificate inainte de apelul functiei 
   // void glutInit(int *argcp, char **argv)
   // Se recomanda ca apelul oricarei functii din biblioteca
   // GLUT sa se faca dupa apelul acestei functii.
   glutInit(&argc, argv);
   
   // Argumentele functiei
   // void glutInitWindowSize (int latime, int latime)
   // reprezinta latimea, respectiv inaltimea ferestrei
   // exprimate in pixeli. Valorile predefinite sunt 300, 300.
   glutInitWindowSize(600, 600);

   // Argumentele functiei
   // void glutInitWindowPosition (int x, int y)
   // reprezinta coordonatele varfului din stanga sus
   // al ferestrei, exprimate in pixeli. 
   // Valorile predefinite sunt -1, -1.
   glutInitWindowPosition(100, 100);

   // Functia void glutInitDisplayMode (unsigned int mode)
   // seteaza modul initial de afisare. Acesta se obtine
   // printr-un SAU pe biti intre diverse masti de display
   // (constante ale bibliotecii GLUT) :
   // 1. GLUT_SINGLE : un singur buffer de imagine. Reprezinta
   //    optiunea implicita ptr. nr. de buffere de
   //    de imagine.
   // 2. GLUT_DOUBLE : 2 buffere de imagine.
   // 3. GLUT_RGB sau GLUT_RGBA : culorile vor fi afisate in
   //    modul RGB.
   // 4. GLUT_INDEX : modul indexat de selectare al culorii.
   // etc. (vezi specificatia bibliotecii GLUT)
   glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB);

   // Functia int glutCreateWindow (char *name)
   // creeaza o fereastra cu denumirea data de argumentul
   // name si intoarce un identificator de fereastra.
   glutCreateWindow (argv[0]);

   Init();

   // Functii callback : functii definite in program si 
   // inregistrate in sistem prin intermediul unor functii
   // GLUT. Ele sunt apelate de catre sistemul de operare
   // in functie de evenimentul aparut

   // Functia 
   // void glutReshapeFunc (void (*Reshape)(int width, int height))
   // inregistreaza functia callback Reshape care este apelata
   // oridecate ori fereastra de afisare isi modifica forma.
   glutReshapeFunc(Reshape);
   
   // Functia 
   // void glutKeyboardFunc (void (*KeyboardFunc)(unsigned char,int,int))
   // inregistreaza functia callback KeyboardFunc care este apelata
   // la actionarea unei taste.
   glutKeyboardFunc(KeyboardFunc);
   
   // Functia 
   // void glutMouseFunc (void (*MouseFunc)(int,int,int,int))
   // inregistreaza functia callback MouseFunc care este apelata
   // la apasarea sau la eliberarea unui buton al mouse-ului.
   glutMouseFunc(MouseFunc);

   // Functia 
   // void glutDisplayFunc (void (*Display)(void))
   // inregistreaza functia callback Display care este apelata
   // oridecate ori este necesara desenarea ferestrei: la 
   // initializare, la modificarea dimensiunilor ferestrei
   // sau la apelul functiei
   // void glutPostRedisplay (void).
   glutDisplayFunc(Display);
   
   // Functia void glutMainLoop() lanseaza bucla de procesare
   // a evenimentelor GLUT. Din bucla se poate iesi doar prin
   // inchiderea ferestrei aplicatiei. Aceasta functie trebuie
   // apelata cel mult o singura data in program. Functiile
   // callback trebuie inregistrate inainte de apelul acestei
   // functii.
   // Cand coada de evenimente este vida atunci este executata
   // functia callback IdleFunc inregistrata prin apelul functiei
   // void glutIdleFunc (void (*IdleFunc) (void))
   glutMainLoop();

   // dealloc
   //deallocate();

   return 0;
}