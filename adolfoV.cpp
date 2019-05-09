//Adolfo Valencia
//5/7/19
//CMPS 3350

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

void drawDestroyer(float cx, float cy, float radius, float xViewScale, float yViewScale) {
  float Red[4]= {1,0,0,1};
  float r=(radius*3.5)/10;
  int points = radius * 5;
  float l=(radius*7)/10;
  float h=(radius*8)/10;
  float theta = (2 * PI) / (points);
  float colors[4] = {1,0,0,1};
  glColor4fv(colors);
  glPushMatrix();
  glBegin(GL_LINE_LOOP);
  for(int i=0; i< points; i++) {
    glVertex2f(cx + r*cos(i*theta)*xViewScale, cy + r*sin(i*theta)*yViewScale);
  }
  glEnd();
  glPopMatrix();
  float tri[3][2];
  tri[0][0] = cx + l*xViewScale;
  tri[0][1] = cy;
  tri[1][0] = cx + r*cos(.4*PI)*xViewScale;
  tri[1][1] = cy + r*sin(.4*PI)*yViewScale;
  tri[2][0] = cx + r*cos(1.6*PI)*xViewScale;
  tri[2][1] = cy + r*sin(1.6*PI)*yViewScale;
  glColor4fv(Red);
  glPushMatrix();
  glBegin(GL_LINE_LOOP);
  for(int i=0; i<3; i++) {
    glVertex2f(tri[i][0], tri[i][1]);
  }
  glEnd();
  glPopMatrix();
  glColor4fv(Red);
  glPushMatrix();
  glBegin(GL_LINE_LOOP);
    glVertex2f(tri[0][0], tri[0][1]-h*yViewScale);
    glVertex2f(tri[0][0], tri[0][1]+h*yViewScale);
  glEnd();
  glPopMatrix();

  tri[0][0] = cx - l*xViewScale;
  tri[0][1] = cy;
  tri[1][0] = cx + r*cos(.6*PI)*xViewScale;
  tri[1][1] = cy + r*sin(.6*PI)*yViewScale;
  tri[2][0] = cx + r*cos(1.4*PI)*xViewScale;
  tri[2][1] = cy + r*sin(1.4*PI)*yViewScale;
  glColor4fv(Red);
  glPushMatrix();
  glBegin(GL_LINE_LOOP);
  for(int i=0; i<3; i++) {
    glVertex2f(tri[i][0], tri[i][1]);
  }
  glEnd();
  glPopMatrix();

  glColor4fv(Red);
  glPushMatrix();
  glBegin(GL_LINE_LOOP);
    glVertex2f(tri[0][0], tri[0][1]-h*yViewScale);
    glVertex2f(tri[0][0], tri[0][1]+h*yViewScale);
  glEnd();
  glPopMatrix();
 }



void DrawTriangle(float cx, float cy, float xViewScale, float yViewScale) {
	float Blue[4] = {0,1,1,0};
	float tri[3][2];
	tri[0][0] = cx;
	tri[0][1] = cy;
	tri[1][0] = cx - 15;
	tri[1][1] = cy - 15;
	tri[2][0] = cx + 15;
	tri[2][1] = cy - 15;
	glColor4fv(Blue);
	glPushMatrix();
	glBegin(GL_TRIANGLE_STRIP);
	for(int i=0; i<3; i++) {
		glVertex2f(tri[i][0], tri[i][1]);
	}
	glEnd();
	glPopMatrix();
}

void DrawRectangle(float cx, float cy, float x, float y, float xViewScale, float yViewScale) {
	float orange[4] = {1,0.5,0,1};
	float rec[4][2];
	rec[0][0] = cx - x*xViewScale;
	rec[0][1] = cy - y*yViewScale;
	rec[1][0] = cx - x*xViewScale;
	rec[1][1] = cy + y*yViewScale;
	rec[2][0] = cx + x*xViewScale;
	rec[2][1] = cy + y*yViewScale;
	rec[3][0] = cx + x*xViewScale;
	rec[3][1] = cy - y*yViewScale;
	glColor4fv(orange);
	glPushMatrix();
	glBegin(GL_LINE_LOOP);
	for(int i=0; i<4; i++) {
	   glVertex2f(rec[i][0], rec[i][1]);
	}
	glEnd();
	glPopMatrix();

}

void DrawCircle(float cx, float cy, float r, int num_segments, float xViewScale, float yViewScale) {
	float theta = (2 * 3.1415926) / float(num_segments);
	float s = sinf(theta);
	float c = cosf(theta);
	float t;
  float x = r;
  float y = 0;
	float colors[4] = {1,0.5,0,1};
	glColor4fv(colors);
    	glPushMatrix();
	glBegin(GL_LINE_LOOP);
	for(int i=0; i< num_segments; i++) {
	   glVertex2f(cx + r*cos(i*theta)*xViewScale, cy + r*sin(i*theta)*yViewScale);
	}
    	glEnd();
	glPopMatrix();
}

void DrawCarrier(float cx, float cy, float r, float xViewScale, float yViewScale ) {
	DrawCircle(cx, cy, r, 360, xViewScale, yViewScale);
	DrawRectangle(cx+(.75*r*xViewScale), cy, r/2, r*7.5/100, xViewScale, yViewScale);
	DrawRectangle(cx-(.75*r*xViewScale), cy, r/2, r*7.5/100, xViewScale, yViewScale);
  DrawRectangle(cx+(.90*r*xViewScale), cy+(.10*r*yViewScale), r/4, r*7.5/100, xViewScale, yViewScale);
  DrawRectangle(cx-(.90*r*xViewScale), cy+(.10*r*yViewScale), r/4, r*7.5/100, xViewScale, yViewScale);
}

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
    float xViewScale = e->xScale*distanceScale;
    float yViewScale = e->yScale*distanceScale;

    if(e->enemyType == 0) {
      float Purple[4] = {1,0,1,0};
      glColor3fv(Purple);
      glPushMatrix();
      glBegin(GL_LINE_LOOP);

      //Override to different Vertices for different classes?
      glVertex2i(x-(e->radius*xViewScale),y-(e->radius*yViewScale));
      glVertex2i(x-(e->radius*xViewScale),y+(e->radius*yViewScale));
      glVertex2i(x,y);
      glVertex2i(x+(e->radius*xViewScale),y-(e->radius*yViewScale));
      glVertex2i(x+(e->radius*xViewScale),y+(e->radius*yViewScale));
      glVertex2i(x,y);
      glEnd();
      glPopMatrix();
    }
    if(e->enemyType == 1) {
      DrawCarrier(x, y, e->radius*16, xViewScale, yViewScale);

    }

    if(e->enemyType == 2) {

      drawDestroyer(x, y, e->radius*2, xViewScale, yViewScale);
    }

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
