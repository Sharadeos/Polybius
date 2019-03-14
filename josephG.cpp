//
//program: josephG.cpp
//author: Joseph Gaede
//
#include <GL/glx.h>
#include "fonts.h"
   

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
//done during lab8 session
//adds very basic pathfinding to asteroids/enemies
void pathFinding(float* a,float* b,int x,int y) 
{
    int c = *a - x;
    int z = *b - y;
    if (c > 30 || z > 30) 
    {
        if (*a < x)
            *a += (float)1;
        if (*a > x)
            *a -= (float)1;
        if (*b < y)
            *b += (float)1;
        if (*b > y)
            *b -= (float)1;
    }

}
