//
//program: andrewH.cpp
//author: Andrew (Joey) Hubbard
//Last Modified: 2-18-2019 10:45am
//
#include <GL/glx.h>
#include "fonts.h"
#include <math.h>
static float spin = 20;
void andrewH(int x, int y, GLuint textid, int move) 
{
    if(!move)
	spin = 20;

    Rect r;
    unsigned int c = 0x00ffff44;
    r.bot = y;
    r.left = x;
    r.center = 0;
    ggprint8b(&r, 16, c, "Andrew Hubbard");

    Rect q; 
    q.bot = y;
    q.left = x/2;
    q.center = 0;
    ggprint16(&q, 16, c, "CREATED BY");
    
    float wid = 110.0f; 
    float rad = move * -.05;
    glPushMatrix();
    glTranslatef(x + 250, y, 0);
    glRotatef(spin*sin(rad),0,0,1);
    glBindTexture(GL_TEXTURE_2D, textid);
    glEnable(GL_ALPHA_TEST);
    glAlphaFunc(GL_GREATER, 0.0f);
    glColor4ub(255,255,255,255);
    glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 1.0f); glVertex2i(-wid,-wid);
        glTexCoord2f(0.0f, 0.0f); glVertex2i(-wid, wid);
        glTexCoord2f(1.0f, 0.0f); glVertex2i( wid, wid);
        glTexCoord2f(1.0f, 1.0f); glVertex2i( wid,-wid);
    glEnd();
    glPopMatrix();
    if (sin(rad) > .98 || sin(rad) < -.98)
    	spin *= 1.01;
}
