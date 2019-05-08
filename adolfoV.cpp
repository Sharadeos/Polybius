//Adolfo Valencia
//2/14/19
//CMPS 3350
/*
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
*/
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


void DrawTriangle(float cx, float cy, Game *g) {
    float Blue[4] = {0,1,1,0};
    float angle = atan2((*g).object.projection[1]-cy, (*g).object.projection[0]-cx);
    float rad[4];
    float tri[3][2];
    tri[0][0] = cx + (rad[2] + 15) * cos(angle);
    tri[0][1] = cy + (rad[2] + 15) * sin(angle);
    tri[1][0] = cx + rad[2]*cos(angle+.02);
    tri[1][1] = cy + rad[2]*sin(angle+.02);
    tri[2][0] = cx + rad[2]*cos(angle-.02);
    tri[2][1] = cy + rad[2]*sin(angle-.02);
   glColor4fv(Blue);
   glPushMatrix();
  glBegin(GL_TRIANGLE_STRIP);
 for(int i=0; i<3; i++) {
    glVertex2f(tri[i][0], tri[i][1]);
 }

glEnd();
glPopMatrix();
}
/*void AdolfoRender(Game *g, Global gl) {
	DrawEnemy();
}*/
void DrawEnemy(Game *g, Global gl) {
for (int i=0; i<(*g).nenemies; i++) {
    Enemy *e = & (*g).earr[i];
  float k[3];

  k[0] = 0;
  k[1] = e->polar[1];
  k[2] = e->polar[2];

  if (k[1] < 0) {
    k[1] = 360 + k[1];
  }

  float s[2];
  s[0] = (*g).ship.angle[0];
  s[1] = (*g).ship.angle[1];
  float low, high;
  low = s[0] - 60;
  high = s[0] + 60;
  high -= low;
  k[1] -= low;

  if (k[1] > 360) {
    k[1] -= 360;
   }
   if (k[1] < 0) {
     k[1] += 360;
   }

  e->projection[0] = ((high - k[1])/120)*gl.xres;
  e->projection[1] = ((s[1] + 45 - k[2])/90)*gl.yres;
  float x = e->projection[0];
  float y = e->projection[1];
  float tempValue = 0;

  //Scale max at the right edge of the setup_screen

  e->xScale = ((high - k[1])/60);
  e->yScale = ((s[1] + 45 - k[2])/45);
  if (e->xScale  > 1.0) {
    tempValue = e->xScale - 1.0;
    e->xScale = 1.0;
    e->xScale = e->xScale - tempValue;
  }

  if (e->yScale  > 1.0) {
    tempValue = e->yScale - 1.0;
    e->yScale = 1.0;
    e->yScale = e->yScale - tempValue;
  }


  float distanceScale = 12/e->polar[0];



  float Yellow[3] = {1,1,0};
  glColor3fv(Yellow);
  glPushMatrix();
  glBegin(GL_TRIANGLE_STRIP);

  //Override to different Vertices for different classes?
  glVertex2i(x-(e->radius*e->xScale*distanceScale),y-(e->radius*e->yScale*distanceScale));
  glVertex2i(x-(e->radius*e->xScale*distanceScale),y+(e->radius*e->yScale*distanceScale));
  glVertex2i(x,y);
  glVertex2i(x+(e->radius*e->xScale*distanceScale),y-(e->radius*e->yScale*distanceScale));
  glVertex2i(x+(e->radius*e->xScale*distanceScale),y+(e->radius*e->yScale*distanceScale));


  glVertex2i(x,y);
  glEnd();
  glPopMatrix();

  float cx = gl.xres/2;
  //float cy = gl.yres/2;
  Rect r;
  //
  r.bot = y + 10;
  r.left = x;
  r.center = 0;
 //ggprint8b(&r, 16, 0x00ff0000, "3350 - Asteroids");
  ggprint8b(&r, 16, 0x00ffff00, "%.1i",e->currentHealth);
}
}

void AdolfoRender(Game *g, Global gl) {
	DrawEnemy(g, gl);
}
