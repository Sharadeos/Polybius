#ifndef Classes_H
#define Classes_H

#include <iostream>
#include <iostream>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <ctime>
#include <cmath>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <X11/keysym.h>
#include <GL/glx.h>
#include "log.h"
#include "fonts.h"
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <bits/stdc++.h>
using namespace std;
#ifdef USE_OPENAL_SOUND
#include </usr/include/AL/alut.h>
#endif

//defined types
typedef float Flt;
typedef float Vec[3];
typedef Flt	Matrix[4][4];

//macros
#define rnd() (((Flt)rand())/(Flt)RAND_MAX)
#define random(a) (rand()%a)
#define VecZero(v) (v)[0]=0.0,(v)[1]=0.0,(v)[2]=0.0
#define MakeVector(x, y, z, v) (v)[0]=(x),(v)[1]=(y),(v)[2]=(z)
#define VecCopy(a,b) (b)[0]=(a)[0];(b)[1]=(a)[1];(b)[2]=(a)[2]
#define VecDot(a,b)	((a)[0]*(b)[0]+(a)[1]*(b)[1]+(a)[2]*(b)[2])
#define VecSub(a,b,c) (c)[0]=(a)[0]-(b)[0]; \
						(c)[1]=(a)[1]-(b)[1]; \
						(c)[2]=(a)[2]-(b)[2]
//constants
const float timeslice = 1.0f;
const float gravity = -0.2f;
#define PI 3.141592653589793
#define MAX 2^20
#define ALPHA 1
const Flt MINIMUM_ASTEROID_SIZE = 60.0;
const float PITCH = .6;
const float TURN = .6;
const int KEYS = 100;
const int MAX_THRUST = 1;
const int MAX_BULLETS = 11;


//-----------------------------------------------------------------------------
//Setup timers
const double physicsRate = 1.0 / 60.0;
const double oobillion = 1.0 / 1e9;
extern struct timespec timeStart, timeCurrent;
extern struct timespec timePause;
extern double physicsCountdown;
extern double timeSpan;
extern double timeDiff(struct timespec *start, struct timespec *end);
extern void timeCopy(struct timespec *dest, struct timespec *source);
//-----------------------------------------------------------------------------



class Global {
  public:
    Global();
	  int xres, yres;
  	  //char keys[65536];
	  int keyhits[KEYS];
      // declare GLuint textid for each png

	  GLuint bigfootTexture;
	  GLuint luisTexture;
	  GLuint AdolfoTexture;
	  GLuint chrisTexture;
      GLuint josephTexture;
      GLuint blackholeTexture;
};

class Image {
  public:
    int width, height;
    unsigned char *data;
    Image(const char *);
    ~Image() {
      delete [] data;
    }
};

class Base {
public:
	Vec angle;
	Vec polar; //rho angle phi
	Vec xyz;
	Vec xyz2; // x^2 , y^2, z^2
	Vec dir;
	Vec pos;
	Vec vec;
	Vec projection;
	float vel;
	float color[4];

	int maxHealth;
	int currentHealth;
	int maxBullets;
	int powerLevel;


	float xScale;
	float yScale;

	void updatePolar(Vec);
	Base();
};


class Object: public Base {

public:
	Object(int x, int y, int z);

};

class Ship: public Base {
public:
	int number;
	bool invert;

	Ship(int x, int y, int z);

};


class Bullet: public Base {
public:
	struct timespec time;

	Bullet();
};


class Asteroid {
public:
	Vec dir;
	Vec pos;
	Vec vel;
	float angle;
	int maxHealth;
	int currentHealth;
	int maxBullets;
	int powerLevel;

	int nverts;
	Flt radius;
	Vec vert[8];
	float rotate;
	float color[3];
	struct Asteroid *prev;
	struct Asteroid *next;
  int shipClass;
  Asteroid();

};

class Game {

private:
	static Game* instance;

public:
	Ship ship;
	Asteroid *ahead;
	Bullet *barr;
	int nasteroids;
	int nbullets;
	Object object;

	struct timespec bulletTimer;
	struct timespec mouseThrustTimer;
	bool mouseThrustOn;
	bool show_credits;
	float mtext;
  float difficulty;
	Game(int xWindowSize, int yWindowSize, const Ship & ship, const Object & object);

	int num_stars;
	float stars[32000][3];
	float debris[500][3];

	static Game* getInstance();
	~Game() {
		delete [] barr;
	}

};

#endif
