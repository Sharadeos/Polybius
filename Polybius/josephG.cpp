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
//progress done in lab8 session
//very very basic pathfinding for asteroids/enemies
void fighterPF(float* a,float* b,int x,int y) 
{
    int c = *a - x;
    int z = *b - y;
    if (c <= 300 || z <= 300) {
        if (*a <= x) 
                *a += (float)1;            
        if (*a >= x) 
            *a -= (float)1;            
        if (*b <= y) 
            *b += (float)1;            
        if (*b >= y) 
            *b -= (float)1;             
        if (c == 0 && z >= y)
            *b -= (float)1;            
        if (c == 0 && z <= y)
            *b += (float)1;            
        if (z == 0 && c >= x)
            *a -= (float)1;            
        if (z == 0 && c <= x)
            *a += (float)1;   
        if (c == 70) 
            *a -= 0;
        if (z == 70)
            *b -= 0;
    }
}
void squadronPF(float* a,float* b,int x,int y)
{
    int c = *a - x;
    int z = *b - y;
    if (c <= 300 || z <= 300) {
        if (*a <= x)
            *a += (float)1;
        if (*a >= x)
            *a -= (float)1;
        if (*b <= y)
            *b += (float)1;
        if (*b >= y)
            *b -= (float)1;
        if (c == 0 && z >= y)
            *b -= (float)1;
        if (c == 0 && z <= y)
            *b += (float)1;
        if (z == 0 && c >= x)
            *a -= (float)1;
        if (z == 0 && c <= x)
            *a += (float)1;
        if (c == 70) 
            *a -= 0;
        if (z == 70)              
            *b -= 0;                                                                                                     
    }
}
void frigatePF(float* a, float* b,int x)
{
    int c = *a - x;
    int missiles = 6;
    *b += 0;
    if (c <= 400) 
        *a -= (float)1;
    if (c>= 400)
        *a += (float)1;
}
//spawned by firgate
void missilePF(float* a,float* b, int x, int y)
{
    int c = *a - x;
    int z = *b - y;
    if (c <= 300 || z <= 300) {
        if (*a <= x)
            *a += (float)1;
        if (*a >= x)
            *a -= (float)1;
        if (*b <= y)
            *b += (float)1;
        if (*b >= y)
            *b -= (float)1;
        if (c == 0 && z >= y)
            *b -= (float)1;
        if (c == 0 && z <= y)
            *b += (float)1;
        if (z == 0 && c >= x)
            *a -= (float)1;
        if (z == 0 && c <= x)
            *a += (float)1;
    }
}
void carrierPF(float* a,float* b,int x,int y)
{
    int c = *a - x;
    int z = *b - y;
    //number of docked ships
    int SD = 4;
    if (c <= 300 || z <= 300) {
        if (*a <= x)
            *a += (float)1;
        if (*a >= x)
            *a -= (float)1;
        if (*b <= y)
            *b += (float)1;
        if (*b >= y)
            *b -= (float)1;
        if (c == 0 && z >= y)
            *b -= (float)1;
        if (c == 0 && z <= y)
            *b += (float)1;
        if (z == 0 && c >= x)
            *a -= (float)1;
        if (z == 0 && c <= x)
            *a += (float)1;
        if (c <= 120)
            *a -= 0;
        if (z <= 120)
            *b -= 0;
    }
}


