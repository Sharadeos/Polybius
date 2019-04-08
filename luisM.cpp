//author:  Luis Manahan
//modified: 3/14/19 11:25AM
//
#include <GL/glx.h>
#include "fonts.h"
#include "classes.h"

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


void modify_singleton(Num n)
{
	cout << n.getNum() << endl;

}
