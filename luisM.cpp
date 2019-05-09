//author:  Luis Manahan
//modified: 3/14/19 11:25AM
//
#include <GL/glx.h>
#include "fonts.h"
#include "classes.h"
#include <string>

Enemy spawnEnemy(Game *g, Global gl, Vec pos, int squadNumber, int enemyType);
void score(Game *g, int i);

void DrawCircle(float cx, float cy, float r, int num_segments)
{
	float theta = 2 * 3.1415926 / float(num_segments);
	float c = cosf(theta);//precalculate the sine and cosine
	float s = sinf(theta);
	float t;
	float x = r;//we start at angle = 0
	float y = 0;
	glBegin(GL_LINE_LOOP);
	for(int ii = 0; ii < num_segments; ii++)
	{
		glVertex2f(x + cx, y + cy);//output vertex
		//apply the rotation matrix
		t = x;
		x = c * x - s * y;
		y = s * t + c * y;
	}
	glEnd();
}
/*
class Scrap {
public:
	Vec dir;
  Vec pos;
  Vec vel;
	Scrap();

	} *scraps = new Scrap[MAX];

Scrap::Scrap() {
	VecZero(dir);
	VecZero(pos);
	VecZero(vel);

}



class Blackhole {
public:
	Vec dir;
	Vec pos;
	Vec vel;
	float angle;
	float color[3];
	float gravity;
	float size;
	Blackhole();
	void physics();
	void render();
} blackhole;

Blackhole::Blackhole() {
  VecZero(dir);
  pos[0] = 0;
  pos[1] = 0;
  pos[2] = 0.0f;
  VecZero(vel);
  angle = 0.0;
  color[0] = color[1] = color[2] = 1.0;
	size = 100;
	void physics();
	void render();

};


void Blackhole::physics()
{

}

void Blackhole::render()
{
	DrawCircle(pos[0]/2, pos[1]/2, size, 100);
}

*/


void creditsLuis(int x, int y, GLuint luisTexture)
{
	Rect r;
	unsigned int c = 0x00ffff44;
	r.bot = y;
	r.left = x;
	r.center = 0;
	ggprint8b(&r, 16, c, "Luis Manahan");

	float wid = 120.0f;

	glPushMatrix();
	glTranslatef(x + 250, y - 50, 0);
	glEnable(GL_ALPHA_TEST);
	glColor3ub(255,255,255);
	glBindTexture(GL_TEXTURE_2D, luisTexture);
	glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 0.0f); glVertex2f(-wid,  wid);
		glTexCoord2f(1.0f, 0.0f); glVertex2f( wid,  wid);
		glTexCoord2f(1.0f, 1.0f); glVertex2f( wid, -wid);
		glTexCoord2f(0.0f, 1.0f); glVertex2f(-wid, -wid);
	glEnd();
	glPopMatrix();
}

bool collisionDetection(Base object1, Base object2)
{

  // we are using multiplications because it's faster than calling Math.pow
  float distance = sqrt((object1.pos[0] - object2.pos[0]) * (object1.pos[0] - object2.pos[0]) +
                           (object1.pos[1] - object2.pos[1]) * (object1.pos[1] - object2.pos[1]) +
                           (object1.pos[2] - object2.pos[2]) * (object1.pos[2] - object2.pos[2]));
  return distance  < (object1.radius/20 + object2.radius/20);

}




int even_random_number() {
    return 2 * ( rand() % 5 + 1 );
}


// 3, 5, 7
int odd_random_number() {
    return (1 + even_random_number()) % 8;
}


extern void alShipLocation(ALenum param, ALfloat v1, ALfloat v2, ALfloat v3);

void difficultyScaling(Game *g, Global gl)
{


//difficulty handler


switch((*g).level)
{
	// level 1
	case 1:
	struct timespec dt;
	clock_gettime(CLOCK_REALTIME, &dt);
	double ds = timeDiff(&(*g).difficultyTimer, &dt);
	(*g).difficulty += 0.001;
	if((*g).nenemies <= 5) {
		if (ds > 10.0) {
			Vec location;
			location[0] = rand() % 500 - 250;
			location[1] = rand() % 500 - 250;
			location[2] = rand() % 500 - 250;
			spawnEnemy(g, gl, location, false, 0);
			clock_gettime(CLOCK_REALTIME, &(*g).difficultyTimer);
			}
		}

	break;
}

	int decrement = 0;
	// Enemy Loop
	for (int i =0; i < (*g).nenemies; i++) {

			Enemy *e= &(*g).earr[i];
			//(*g).earr[i].updatePolar((*g).ship.pos);
			if (collisionDetection(*e, (*g).ship)) {
					//time to delete the bullet.
				memcpy(&(*g).earr[i], &(*g).earr[(*g).nenemies-1], sizeof(Enemy));
				(*g).nenemies--;
				//do not increment i.
				continue;
					}

			// Bullet Loop
			for (int j=0; j< (*g).nbullets; j++) {
				Bullet *b = & (*g).barr[j];

				if (collisionDetection(*e, *b)) {
						//time to delete the bullet.

						memcpy(&(*g).barr[j], &(*g).barr[(*g).nbullets-1], sizeof(Bullet));

						(*g).earr[i].currentHealth--;
						decrement++;

						if((*g).earr[i].currentHealth <= 0) {
							memcpy(&(*g).earr[i], &(*g).earr[(*g).nenemies-1], sizeof(Enemy));
							(*g).nenemies--;
							 score(g,i);

							continue;

						}
					//do not increment i.

						}

				}
				(*g).nbullets-=decrement;


		// if it not a squadron
		if(e->squadNumber == 0) {
			float a_xy, a_z;
			a_xy = e->angle[0];
			a_xy *= PI/180;
			a_z = e->angle[1];
			a_z *= PI/180;

			e->pos[0] += e->vel*cos(a_xy)*sin(a_z);
			e->pos[1] += e->vel*sin(a_xy)*sin(a_z);
			e->pos[2] += e->vel*cos(a_z);
			e->updatePolar((*g).ship.pos);

		}

		if(e->squadNumber > 0) {
			float a_xy, a_z;
			a_xy = e->angle[0];
			a_xy *= PI/180;
			a_z = e->angle[1];
			a_z *= PI/180;

			e->pos[0] += e->vel*cos(a_xy)*sin(a_z);
			e->pos[1] += e->vel*sin(a_xy)*sin(a_z);
			e->pos[2] += e->vel*cos(a_z);
			e->updatePolar((*g).ship.pos);

		}

#ifdef USE_OPENAL_SOUND
		//playEngine();
		//alShipLocation(AL_POSITION, e->pos[0], e->pos[1], e->pos[2]);
#endif


	}

struct timespec thrust;
double thrustCounter;
// h key thrust
if (gl.keyhits[4]) {

	clock_gettime(CLOCK_REALTIME, &thrust);
	thrustCounter = timeDiff(&(*g).difficultyTimer, &thrust);
	(*g).ship.boost -= 0.1;
	(*g).ship.vel = 2;
}

// resets the thrust
if (thrustCounter > 5.0 && 	(*g).ship.boost <= 100.0) {

	(*g).ship.boost += 0.1;
		timeCopy(&(*g).bulletTimer, &thrust);

}


// b key
if (gl.keyhits[98]) {
	struct timespec bt;
		clock_gettime(CLOCK_REALTIME, &bt);
		double ts = timeDiff(&(*g).bulletTimer, &bt);
		if (ts > 0.5) {
			timeCopy(&(*g).bulletTimer, &bt);
				Vec location;

				location[0] = rand() % 500 - 250;
				location[1] = rand() % 500 - 250;
				location[2] = rand() % 500 - 250;
				spawnEnemy(g, gl, location, 0, 0);
			}
}
// f key squadron test

if (gl.keyhits[2]) {
	struct timespec bt;

	clock_gettime(CLOCK_REALTIME, &bt);
	double ts = timeDiff(&(*g).bulletTimer, &bt);

		if (ts > 0.5) {
			timeCopy(&(*g).bulletTimer, &bt);


			Squadron *s= &(*g).sarr[(*g).nsquadrons];
			(*g).nsquadrons++;
			s->size = 3;// odd_random_number();

			// setting up squadron and placing the enemies on them

				Vec squadronLocation;
				squadronLocation[0] = rand() % 500 - 250;
				squadronLocation[1] = rand() % 500 - 250;
				squadronLocation[2] = rand() % 500 - 250;
				// spawns at the center 1 [2] 3
				spawnEnemy(g, gl, squadronLocation, (*g).nsquadrons, 0);



				float lowx = 25;
				float lowy = 25;
				for(int i = (s->size/2) - 1; i >= 0; i--) {
					//printf("lowbar ran %i\n", i);
					squadronLocation[0] += lowx;
					squadronLocation[1] += lowy;
					spawnEnemy(g, gl, squadronLocation, (*g).nsquadrons, 0);
					lowx+=lowx;
					lowy+=lowy;
				}
				lowx = -25;
				lowy = 25;
				for(int i = (s->size/2) + 1; i < s->size; i++) {
					//printf("highbar ran %i\n", i);
					squadronLocation[0] += lowx;
					squadronLocation[1] += lowy;
					spawnEnemy(g, gl, squadronLocation, (*g).nsquadrons, 0);
					lowx+=lowx;
					lowy+=lowy;
				}

		}
	}

	// g key
	// make object fire a bullet
	if (gl.keyhits[3]) {

		struct timespec bt;
		 clock_gettime(CLOCK_REALTIME, &bt);
		 double ts = timeDiff(&(*g).bulletTimer, &bt);
		 if (ts > 0.1) {
				 timeCopy(&(*g).bulletTimer, &bt);
				 if ((*g).nbullets < MAX_ARRAY) {
						 //shoot a bullet...
						 //Bullet *b = new Bullet;
						 Bullet *b = &(*g).barr[(*g).nbullets];
						 timeCopy(&b->time, &bt);
						 b->pos[0] = (*g).object.pos[0];
						 b->pos[1] = (*g).object.pos[1];
						 b->pos[2] = (*g).object.pos[2];
						 //b->vel = (*g).ship.vel + 25;
						 b->vel = (*g).ship.vel + 25;
						 //convert ship angle to radians
						 b->angle[0] = (*g).object.angle[0];
						 b->angle[1] = (*g).object.angle[1];
						 b->color[0] = 0.0f;
						 b->color[1] = 1.0f;
						 b->color[2] = 0.0f;
						 (*g).nbullets++;
				 }



	}
	}
}


void luisRender(Game *g, Global gl)
{
	/*
	Rect r;
	unsigned int c = 0x00ffff44;
	r.bot = (*g).ship.pos[1];
	r.left = (*g).ship.pos[0];
	r.center = -50;
	(*g).difficulty += 0.001;
	ggprint8b(&r, 16, c, "%f", (*g).difficulty);

	blackhole.pos[0] = gl.xres;
	blackhole.pos[1] = gl.yres;
	blackhole.render();
*/


	float cx = gl.xres/2;
	//float cy = gl.yres/2;
	Rect r;
	//
	r.bot = 100;
	r.left = cx-200;
	r.center = 0;
	//ggprint8b(&r, 16, 0x00ff0000, "3350 - Asteroids");
	ggprint8b(&r, 16, 0x00ffff00, "num Enemies = %.1i",(*g).nenemies);
	ggprint8b(&r, 16, 0x00ffff00, "ship velocity = %.1f",(*g).ship.vel);
	ggprint8b(&r, 16, 0x00ffff00, "level= %.1i",(*g).level);
	ggprint8b(&r, 16, 0x00ffff00, "difficulty= %.1f",(*g).difficulty);
	ggprint8b(&r, 16, 0x00ffff00, "object xy angle= %.1f",(*g).object.angle[0]);
	ggprint8b(&r, 16, 0x00ffff00, "object z angle= %.1f",(*g).object.angle[1]);
	ggprint8b(&r, 16, 0x00ffff00, "ship thrust = %.1f",(*g).ship.boost);

}

extern void alShipLocation(ALenum param, ALfloat v1, ALfloat v2, ALfloat v3);


Enemy spawnEnemy(Game *g, Global gl, Vec pos, int squadNumber, int enemyType)
{
  Enemy *e = &(*g).earr[(*g).nenemies];
  e->pos[0] = (*g).ship.pos[0] + pos[0];
  e->pos[1] = (*g).ship.pos[1] + pos[1];
  e->pos[2] = (*g).ship.pos[2] + pos[2];
  e->vel = 0;
	e->currentHealth = 3;
	e->enemyType = enemyType;
	e->squadNumber = squadNumber;
#ifdef USE_OPENAL_SOUND
	alShipLocation(AL_POSITION, e->pos[0], e->pos[1], e->pos[2]);
#endif
  //convert ship angle to radians
  e->angle[0] = (*g).ship.angle[0];
  e->angle[1] = (*g).ship.angle[1];
  e->color[0] = 0.0f;
  e->color[1] = 1.0f;
  e->color[2] = 0.0f;
  (*g).nenemies++;

	return *e;

}

//object test function
void enemyTargeting(Game *g, Global gl)
{

	if ((*g).object.angle[0] < 0.0f) {
		(*g).object.angle[0] += 360.0f;
}
		if ((*g).object.angle[0] > 360.0f) {
			(*g).object.angle[0] -= 360.0f;
	}

		if ((*g).object.angle[1] < 0.1f) {
			(*g).object.angle[1] = 0.1f;
	}

		if ((*g).object.angle[1] > 179.9f) {
			(*g).object.angle[1] = 179.9f;
	}

// change it based on the ships position not its angle


	(*g).object.angle[0] = (*g).object.polar[1] + 180;


	(*g).object.angle[1] = 180 - (*g).object.polar[2];

}
