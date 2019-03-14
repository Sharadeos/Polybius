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
/*
class Enemies {
  public:
      Vec dir;
      Vec pos;
      Vec vel;
      float angle;
      float color[3];
      Flt radius;
      float rotate;
      struct Enemy *prev;
      struct Enemy *next;
  public:
      Enemies() {
          pos[0] = (Flt)(gl.xres/2);
          pos[1] = (Flt)(gl.yres/2);
          pos[2] = 0.0f;
          VecZero(vel);
          angle = 0.0;
          color[0] = color[1] = color[2] - 1.0;
          prev = NULL;
          next = NULL;
      }
  };
*/
