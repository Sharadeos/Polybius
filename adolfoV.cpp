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
class Enemy {
 public:
        Vec pos;
        float vel;
        float angle;
        float color[3];
        float Vertices[16][2];
        int numVertices;
public:
        Enemy() {
            srand(time(NULL));
            numVertices =(rand() % 14)+3;
            for (int i=0; i<numVertices; i++) {
                Vertices[i][0] = rand()%50;
                if (rand() % 2 == 0)
                        Vertices[i][0] *= -1;
                Vertices[i][1] = rand()%50;
                if (rand() % 2 == 0)
                        Vertices[i][1] *= -1;
            }

            pos[0]=0;
            pos[1]=10000;
            pos[2]=0;
            angle = 0.0;
            color[0] = color[1] = color[2] = 1.0;
         }
};

 int x=gl.xres * .25;
        int y=gl.yres * .25;
        float Red[4];
        Red[0]=1;
        Red[1]=0;
        Red[2]=0;
        Red[3]=1;
        glColor3fv(Red);
        glPushMatrix();
        //glTranslatef(g.enemy.pos[0], g.enemy.pos[1], g.enemy.pos[2]);
        //glRotatef(g.enemy.angle, 0.0f, 0.0f, 1.0f);
        glBegin(GL_POLYGON);
        for (int i=0; i<g.enemy.numVertices; i++) {
        glVertex2f(x + g.enemy.Vertices[i][0], y + g.enemy.Vertices[i][1]);
        }
        /*
        glVertex2f(x +  30.0f, y + 20.0f);
        glVertex2f(x +  35.0f, y + -20.0f);
        glVertex2f(x +  50.0f, y + -10.0f);
        glVertex2f(x +  15.0f, y + 20.0f);
        glVertex2f(x + 12.0f, y + -10.0f);
        */
        glEnd();
        glPopMatrix();
