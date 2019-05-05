//author:  Luis Manahan
//modified: 3/14/19 11:25AM
//
#include <GL/glx.h>
#include "fonts.h"
#include "classes.h"
#include <string>

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

void createAsteroid(Game *g, Global gl)
{
	Asteroid *a = new Asteroid;
	a->nverts = 8;
	a->radius = rnd()*80.0 + 40.0;
	Flt r2 = a->radius / 2.0;
	Flt angle = 0.0f;
	Flt inc = (PI * 2.0) / (Flt)a->nverts;
	for (int i=0; i<a->nverts; i++) {
		a->vert[i][0] = sin(angle) * (r2 + rnd() * a->radius);
		a->vert[i][1] = cos(angle) * (r2 + rnd() * a->radius);
		angle += inc;
	}
	a->pos[0] = (Flt)(rand() % gl.xres);
	a->pos[1] = (Flt)(rand() % gl.yres);
	a->pos[2] = 0.0f;
	a->angle = 0.0;
	a->rotate = rnd() * 4.0 - 2.0;
	a->color[0] = 0.8;
	a->color[1] = 0.8;
	a->color[2] = 0.7;
	a->vel[0] = (Flt)(rnd()*2.0-1.0);
	a->vel[1] = (Flt)(rnd()*2.0-1.0);
	//std::cout << "asteroid" << std::endl;
	//add to front of linked list
	a->next = (*g).ahead;
	if ((*g).ahead != NULL)
		(*g).ahead->prev = a;
		(*g).ahead = a;

	(*g).nasteroids++;
}
/*
void createBullet(Game *g, Global gl, Object object)
{
	struct timespec bt;
	clock_gettime(CLOCK_REALTIME, &bt);
	double ts = timeDiff(&(*g).bulletTimer, &bt);
	if (ts > 0.1) {
		timeCopy(&(*g).bulletTimer, &bt);
		//shoot a bullet...
		if ((*g).nbullets < object.maxBullets) {
			Flt rad;
			Flt xdir;
			Flt ydir;
			Bullet *b;
			switch(object.powerLevel)
			{
			case 1:

				b = &(*g).barr[(*g).nbullets];
				timeCopy(&b->time, &bt);
				b->pos[0] = object.pos[0];
				b->pos[1] = object.pos[1];
				b->vel[0] = object.vel[0];
				b->vel[1] = object.vel[1];
				//convert ship angle to radians
				rad = ((object.angle+90.0) / 360.0f) * PI * 2.0;
				//convert angle to a vector
				xdir = cos(rad);
				ydir = sin(rad);
				b->pos[0] += xdir*20.0f;
				b->pos[1] += ydir*20.0f;
				b->vel[0] += xdir*6.0f + rnd()*0.1;
				b->vel[1] += ydir*6.0f + rnd()*0.1;
				b->color[0] = 1.0f;
				b->color[1] = 1.0f;
				b->color[2] = 1.0f;
				++(*g).nbullets;

			break;
			case 2:

			for( int i = 45; i <= 135; i+=45 )
			{
				b = &(*g).barr[(*g).nbullets];
				timeCopy(&b->time, &bt);
				b->pos[0] = object.pos[0];
				b->pos[1] = object.pos[1];
				b->vel[0] = object.vel[0];
				b->vel[1] = object.vel[1];
				//convert ship angle to radians
				rad = ((object.angle + i) / 360.0f) * PI * 2.0;
				//convert angle to a vector
				xdir = cos(rad);
				ydir = sin(rad);
				b->pos[0] += xdir*20.0f;
				b->pos[1] += ydir*20.0f;
				b->vel[0] += xdir*6.0f + rnd()*0.1;
				b->vel[1] += ydir*6.0f + rnd()*0.1;
				b->color[0] = 1.0f;
				b->color[1] = 1.0f;
				b->color[2] = 1.0f;
				++(*g).nbullets;
			}


			break;
		}
	}

	}
}
*/
void difficultyScaling(Game *g, Global gl)
{
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




}
