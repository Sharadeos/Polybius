//Adolfo Valencia
//2/14/19
//CMPS 3350
#include <iostream>
#include <cstdlib>
#include <cstring>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <math.h>
#include <X11/Xlib.h>
#include <GL/glx.h>
#include "log.h"
#include "fonts.h"
/*
class Image {
    public:
        int width, height;
        unsigned char *data;
        ~Image() { delete [] data;}
        Image(const char *frame) {
            if(fname[0] == '\0')
                return;
            int ppmFlag=0;
            char name[40];
            strcpy(name, fname);
            int slen = strlen(name);
            char ppname[80];
            if (strncmp(name+(slen-4), 
            */
void AdolfoValenciaPicture(int x, int y, GLuint AdolfoTexture) {
    Rect r;
    unsigned int c = 0x00ffff44;
    r.bot = y;
    r.left = x;
    r.center = 0;
    ggprint8b(&r, 16, c, "Adolfo Valencia");
    float wid = 120.0f;
  //glColor3ub(255, 255, 255);
   // static float angle = 0.0f;
   // static int wid = 30;
   // wid += sin(angle) * 10;
   // float fa = (float)a;
   // float fv = (float)v;
   // float x = 0;
   // x += sin(angle) * 10;
   // fa += x;
   // angle += sin(angle) * 0.2f;
    glPushMatrix();
    glTranslatef(x+100, y, 0);
    glEnable(GL_ALPHA_TEST);
    glColor3ub(255,255,255);
   // glRotatef(x, 0, 0, 1.0);
    glBindTexture(GL_TEXTURE_2D, AdolfoTexture);
    glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 1.0f); glVertex2i(-wid,-wid);
        glTexCoord2f(0.0f, 0.0f); glVertex2i(-wid, wid);
        glTexCoord2f(1.0f, 0.0f); glVertex2i( wid, wid);
        glTexCoord2f(1.0f, 1.0f); glVertex2i( wid,-wid);
    glEnd();
    glPopMatrix();
}
