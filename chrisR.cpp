//Program: chrisR.cpp
//Author:  Christopher Ramirez
//Date:    2019
#include <GL/glx.h>
#include "log.h"
#include "fonts.h"
void showChrisRamirez(int x, int y, GLuint textid)
{
	Rect r;
	unsigned int c = 0x00ffff44;
	r.bot = y;
	r.left = x;
	r.center = 0;
	ggprint8b(&r, 16, c, "Chris Ramirez");
	
	static int wid = 70;
	glPushMatrix();
	glTranslatef(x+250, y, 0);
	glBindTexture(GL_TEXTURE_2D, textid);
	glEnable(GL_ALPHA_TEST);
    glColor3ub(255, 255, 255);
	glBegin(GL_QUADS);
 	       glTexCoord2f(0.0f, 1.0f); glVertex2i(-wid,-wid);
	       glTexCoord2f(0.0f, 0.0f); glVertex2i(-wid, wid);
	       glTexCoord2f(1.0f, 0.0f); glVertex2i( wid, wid);
	       glTexCoord2f(1.0f, 1.0f); glVertex2i( wid,-wid);
	glEnd();
	glPopMatrix();
}
 
