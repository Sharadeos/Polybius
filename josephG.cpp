//
//program: josephG.cpp
//author: Joseph Gaede
//
#include <GL/glx.h>
#include "fonts.h"
#include "classes.h"
#include <math.h>
typedef float Vec[3];
void josephG(int x, int y, GLuint textid)
{
    Rect r;
    unsigned int c = 0x00ffff44;
    r.bot = y;
    r.left = x;
    r.center = 0;
    ggprint8b(&r, 16, c, "Joseph Gaede");

    glPushMatrix();
    glTranslatef(x + 200, y, 0);
    glBindTexture(GL_TEXTURE_2D, textid);
    glEnable(GL_ALPHA_TEST);
    glAlphaFunc(GL_GREATER, 0.0f);
    glColor4ub(255,255,255,255);
    float wid = 120.0f;
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 1.0f); glVertex2i(-wid,-wid);
    glTexCoord2f(0.0f, 0.0f); glVertex2i(-wid, wid);
    glTexCoord2f(1.0f, 0.0f); glVertex2i( wid, wid);
    glTexCoord2f(1.0f, 1.0f); glVertex2i( wid,-wid);
    glEnd();
    glPopMatrix();
}



void fighterPF(Game *g, int e)
{
        if ((*g).earr[e].enemyType == 0)
        {
        float a_xy, a_z;
        a_xy = (*g).earr[e].angle[0];
        a_xy *= PI/180;
        a_z = (*g).earr[e].angle[1];
        a_z *= PI/180;
        (*g).earr[e].pos[0] += (*g).earr[e].vel*cos(a_xy)*sin(a_z);
        (*g).earr[e].pos[1] += (*g).earr[e].vel*sin(a_xy)*sin(a_z);
        (*g).earr[e].pos[2] += (*g).earr[e].vel*cos(a_z);

            if ((*g).earr[e].polar[0] > RANGE) {
                if((*g).earr[e].vel > 0 )
                   (*g).earr[e].vel -= .01;
                if((*g).earr[e].vel < 0)
                   (*g).earr[e].vel = 0;
            }

            if ((*g).earr[e].polar[0] < RANGE) {
                if((*g).earr[e].vel < 1 )
                   (*g).earr[e].vel += .01;
                if((*g).earr[e].vel > .5)
                   (*g).earr[e].vel = .5;
            }
            float xy =(*g).earr[e].polar[1]+180;
            if (xy > 360)
                xy -= 360;
            float z = (*g).earr[e].polar[2]+90;
            if (z > 180)
                z -= 180;

            if ((*g).earr[e].angle[0] < xy-1)
                (*g).earr[e].angle[0] += .5;

            if ((*g).earr[e].angle[0] > xy+1)
                (*g).earr[e].angle[0] -= .5;

            if ((*g).earr[e].angle[1] < z-1)
                (*g).earr[e].angle[1] += .1;

            if ((*g).earr[e].angle[1] > z+1)
                (*g).earr[e].angle[1] -= .1;
        }
	for (int i = 0; i < (*g).nenemies; i++){
	    if ((*g).earr[i].angle[0] == (*g).earr[e].angle[0] && i != e) {
		(*g).earr[i].angle[0] -=.5;
	    }
	    if ((*g).earr[i].angle[1] == (*g).earr[e].angle[1] && i != e) {
		(*g).earr[i].angle[1]-=.5;
	    }

	}

}
void carrierPF(Game *g, int i)
{
        if ((*g).earr[1].enemyType == 1) {
            float a_xy, a_z;
            a_xy = (*g).earr[i].angle[0];
            a_xy *= PI/180;
            a_z = (*g).earr[i].angle[1];
            a_z *= PI/180;
            (*g).earr[i].pos[0] += (*g).earr[i].vel*cos(a_xy)*sin(a_z);
            (*g).earr[i].pos[1] += (*g).earr[i].vel*sin(a_xy)*sin(a_z);
            (*g).earr[i].pos[2] += (*g).earr[i].vel*cos(a_z);

            if ((*g).earr[i].polar[0] > RANGE) {
                if((*g).earr[i].vel > 0 )
                   (*g).earr[i].vel -= .01;
                if((*g).earr[i].vel < 0)
                   (*g).earr[i].vel = 0;
            }

            if ((*g).earr[i].polar[0] < RANGE) {
                if((*g).earr[i].vel < 1 )
                   (*g).earr[i].vel += .01;
                if((*g).earr[i].vel > .5)
                   (*g).earr[i].vel = .5;
            }
            float xy =(*g).earr[i].polar[1]+180;
            if (xy > 360)
                xy -= 360;
            float z = (*g).earr[i].polar[2]+90;
            if (z > 180)
                z -= 180;

            if ((*g).earr[i].angle[0] < xy-1)
                (*g).earr[i].angle[0] += .01;

            if ((*g).earr[i].angle[0] > xy+1)
                (*g).earr[i].angle[0] -= .01;

            if ((*g).earr[i].angle[1] < z-1)
                (*g).earr[i].angle[1] += .01;

            if ((*g).earr[i].angle[1] > z+1)
                (*g).earr[i].angle[1] -= .01;
        }

}
void frigatePF(Game *g, int i)
{
        if ((*g).earr[2].enemyType == 2) {
           float a_xy, a_z;
           a_xy = (*g).earr[i].angle[0];
           a_xy *= PI/180;
           a_z = (*g).earr[i].angle[1];
           a_z *= PI/180;
           (*g).earr[i].pos[0] += (*g).earr[i].vel*cos(a_xy)*sin(a_z);
           (*g).earr[i].pos[1] += (*g).earr[i].vel*sin(a_xy)*sin(a_z);
           (*g).earr[i].pos[2] += (*g).earr[i].vel*cos(a_z);

            if ((*g).earr[i].polar[0] > RANGE) {
                if((*g).earr[i].vel > 0 )
                   (*g).earr[i].vel -= .01;
                if((*g).earr[i].vel < 0)
                   (*g).earr[i].vel = 0;
            }

            if ((*g).earr[i].polar[0] < RANGE) {
                if((*g).earr[i].vel < 1 )
                   (*g).earr[i].vel += .01;
                if((*g).earr[i].vel > .1)
                   (*g).earr[i].vel = .1;
            }
            float xy =(*g).earr[i].polar[1]+180;
            if (xy > 360)
                xy -= 360;
            float z = (*g).earr[i].polar[2]+90;
            if (z > 180)
                z -= 180;

            if ((*g).earr[i].angle[0] < xy-1)
                (*g).earr[i].angle[0] += .01;

            if ((*g).earr[i].angle[0] > xy+1)
                (*g).earr[i].angle[0] -= .01;

            if ((*g).earr[i].angle[1] < z-1)
                (*g).earr[i].angle[1] += .01;

            if ((*g).earr[i].angle[1] > z+1)
                (*g).earr[i].angle[1] -= .01;
        }
}
void pathFindingTest2(Game *g,Global gl)
{
    Rect r;
    r.bot = 1000;
    r.left = (gl.yres/2)-34;
    r.center = 0;
    ggprint8b(&r,16,0x00ffff00,"score:  %i",(*g).score);
    for (int i = 0; i < (*g).nenemies; i++) {
    	//fighter
	if ((*g).earr[i].enemyType == 0) {
	    fighterPF(g,i);
	}
	if ((*g).earr[i].enemyType == 1) {
	    carrierPF(g,i);
	}
	if ((*g).earr[i].enemyType == 2) {
	    frigatePF(g,i);

	}
    }
}
void score(Game *g,int i)
{
        if ((*g).earr[i].enemyType == 0)
	    (*g).score += 100;
	if ((*g).earr[i].enemyType == 1)
	    (*g).score += 500;
	if ((*g).earr[i].enemyType == 2)
	    (*g).score += 1000;
}
void scoreBoard(Game *g, Global gl)
{
    Rect r;
    r.bot = gl.yres-100;
    r.left = (gl.xres/2)-34;
    r.center = 0;
    ggprint8b(&r,16,0x00ffff00,"score:  %i",(*g).score);


}
