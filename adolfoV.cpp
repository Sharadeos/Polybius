//Adolfo Valencia
//2/14/19
//CMPS 3350
#include <iostream>
#include <cstdlib>
#include <cstring>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <unistd.h>
#include <time.h>
#include <math.h>
#include <X11/Xlib.h>
#include <GL/glx.h>
#include "log.h"
#include "fonts.h"
#include "classes.h"
void AdolfoValenciaPicture(int x, int y, GLuint textid) {
    Rect r;
    unsigned int c = 0x00ffff44;
    r.bot = y;
    r.left = x;
    r.center = 0;
    ggprint8b(&r, 16, c, "Adolfo Valencia");
    float wid = 120.0f;
    glPushMatrix();
    glTranslatef(x+250, y, 0);
    glEnable(GL_ALPHA_TEST);
    glColor3ub(255,255,255);
    glBindTexture(GL_TEXTURE_2D, textid);
    glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 1.0f); glVertex2i(-wid,-wid);
        glTexCoord2f(0.0f, 0.0f); glVertex2i(-wid, wid);
        glTexCoord2f(1.0f, 0.0f); glVertex2i( wid, wid);
        glTexCoord2f(1.0f, 1.0f); glVertex2i( wid,-wid);
    glEnd();
    glPopMatrix();
}
class Enemy() {
    public:
	vec dir;
	vec pos;
	vec vel;
	float angle;
	float color[3];
	float gravity;
	float sixe;
	Enemy();
	void physics();
	void render();
}	enemy;
Enemy::Enemy() {
    VecZero(dir);
    pos[0]=0;
    pos[1]=1;
    pos[2]=2;
    VecZero(vel);
    angle = 0.0;
    color[0] = color[1] = color[2] =1.0;
    size = 100;
    void physics();
    void render();
};
void Enemy::physics()
{
}

void Enemy::render()
{
DrawEnemy(pos[0]/2, pos[1]/2, size, 100);
}

void createEnemy(Game *g, Global gl)
{
    Enemy *a = new Enemy;
    a->nverts = 6;
    a->radius =rnd()*90.0 + 30.0;
    Flt r2 = a->raduis / 2.0;
    Flt angle = 0.0f;
    Flt inc = (PI * 2.0) / (Flt)a->nverts;
    for (int i=0; i<a->nverts; i++) {
	a->verti[i][0] = sin(angle) * (r2 + rnd() * a->raduis);
	a->verti[i][1] = cos(angle) * (r2 + rnd() * a->radius);
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
	a->next = (*g).ahead;
	if ((*g).ahead != NULL)
		(*g).ahead->prev = a;
		(*g).ahead = a;

	(*g).nEnemy++;
