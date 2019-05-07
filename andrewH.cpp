//
//program: andrewH.cpp
//author: Andrew (Joey) Hubbard
//Last Modified: 2-18-2019 10:45am
//
#include <GL/glx.h>
#include "fonts.h"
#include <math.h>
#include "classes.h"
static float spin = 20;
void andrewH(int x, int y, GLuint textid, float move)
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
void credit(Game *g, Global gl) {
    if (gl.keyhits[99]) {
        (*g).show_credits = !(*g).show_credits;
        usleep(200000);
        gl.keyhits[99] = 0;
    }
}
#ifdef USE_OPENAL_SOUND
extern void setActionFlag();
extern void checkAction(timespec *t);
extern void pewPew();
extern void ALPlayerUpdate(ALenum param, ALfloat v1, ALfloat v2, ALfloat v3);
extern void ALPlayerVel(ALenum param, ALfloat v1);
extern void alShipLocation(ALenum param, ALfloat v1, ALfloat v2, ALfloat v3);
extern void orientationVec(ALfloat v1, ALfloat v2, ALfloat v3, ALfloat v4,
	ALfloat v5, ALfloat v6);
#endif
void joeyPhysics(Game *g, Global gl)
{

    float a_xy, a_z;
    a_xy = (*g).ship.angle[0];
    a_xy *= PI/180;
    a_z = (*g).ship.angle[1];
    a_z *= PI/180;
    (*g).ship.pos[0] += .04*(*g).ship.vel*cos(a_xy)*sin(a_z);
    (*g).ship.pos[1] += .04*(*g).ship.vel*sin(a_xy)*sin(a_z);
    (*g).ship.pos[2] += .04*(*g).ship.vel*cos(a_z);
    alShipLocation(AL_POSITION, (*g).object.pos[0], (*g).object.pos[1],
	    (*g).object.pos[2]);
    (*g).object.updatePolar((*g).ship.pos);
#ifdef USE_OPENAL_SOUND
    checkAction(&timeCurrent);
    /*alShipLocation(AL_POSITION, (*g).earr->pos[0], (*g).earr->pos[1],
	    (*g).earr->pos[2]);*/
    ALPlayerUpdate(AL_POSITION, (*g).object.pos[0], (*g).ship.pos[1],
	    (*g).ship.pos[2]);
    ALPlayerVel(AL_VELOCITY, (*g).ship.vel);
    float v_xy, v_z;
    float abc[6];
    v_xy = ((*g).ship.angle[0] * (PI/180));
    v_z = ((*g).ship.angle[1] * (PI/180));
    abc[0] = cos(v_xy)*sin(v_z);
    abc[1] = sin(v_xy)*sin(v_z);
    abc[2] = cos(v_z);
    v_z = ((*g).ship.angle[1] - 90) * (PI/180);
    abc[3] = cos(v_xy)*sin(v_z);
    abc[4] = cos(v_xy)*sin(v_z);
    abc[5] = cos(v_z);
    orientationVec(abc[0], abc[1], abc[2], abc[3], abc[4], abc[5]);
#endif
    //Update bullet positions
    struct timespec bt;
    clock_gettime(CLOCK_REALTIME, &bt);
    int i = 0;
    while (i < (*g).nbullets) {
        Bullet *b = &(*g).barr[i];
        //How long has bullet been alive?
        double ts = timeDiff(&b->time, &bt);
        if (ts > 2.5) {
            //time to delete the bullet.
            memcpy(&(*g).barr[i], &(*g).barr[(*g).nbullets-1],
                    sizeof(Bullet));
            (*g).nbullets--;
            //do not increment i.
            continue;
        }
        float a_xy, a_z;
        a_xy = b->angle[0];
        a_xy *= PI/180;
        a_z = b->angle[1];
        a_z *= PI/180;
        //move the bullet
        b->pos[0] += b->vel*cos(a_xy)*sin(a_z);
        b->pos[1] += b->vel*sin(a_xy)*sin(a_z);
        b->pos[2] += b->vel*cos(a_z);
        b->updatePolar((*g).ship.pos);
        /*Check for collision with window edges
          if (b->pos[0] < 0.0) {
          b->pos[0] += (float)gl.xres;
          }
          else if (b->pos[0] > (float)gl.xres) {
          b->pos[0] -= (float)gl.xres;
          }
          else if (b->pos[1] < 0.0) {
          b->pos[1] += (float)gl.yres;
          }
          else if (b->pos[1] > (float)gl.yres) {
          b->pos[1] -= (float)gl.yres;
          }
         */
        i++;
    }
    //object updating position

    /*
    //Check for collision with window edges
    object.drawSize[0] -= ((*g).ship.vel*cos(a_xy)*sin(a_z));
    object.drawSize[1] -=  (*g).ship.vel*cos(a_z);
    object.pos[0] += ((*g).ship.vel*cos(a_xy)*sin(a_z));
    object.pos[1] += ((*g).ship.vel*sin(a_xy)*sin(a_z));

    (*g).ship.pos[0] += (*g).ship.vel*cos(a_xy)*sin(a_z);
    (*g).ship.pos[1] += (*g).ship.vel*sin(a_xy)*sin(a_z);
    (*g).ship.pos[2] += (*g).ship.vel*cos(a_z);
    if ((*g).ship.pos[0] < 0.0) {
    (*g).ship.pos[0] += (float)gl.xres;
    }
    else if ((*g).ship.pos[0] > (float)gl.xres) {
    (*g).ship.pos[0] -= (float)gl.xres;
    }
    else if ((*g).ship.pos[1] < 0.0) {
    (*g).ship.pos[1] += (float)gl.yres;
    }
    else if ((*g).ship.pos[1] > (float)gl.yres) {
    (*g).ship.pos[1] -= (float)gl.yres;
    }

    //Asteroid collision with bullets?
    //If collision detected:
    //     1. delete the bullet
    //     2. break the asteroid into pieces
    //        if asteroid small, delete it
    a = (*g).ahead;
    while (a) {
    //is there a bullet within its radius?
    int i=0;
    while (i < (*g).nbullets) {
    Bullet *b = &(*g).barr[i];
    d0 = b->pos[0] - a->pos[0];
    d1 = b->pos[1] - a->pos[1];
    dist = (d0*d0 + d1*d1);
    if (dist < (a->radius*a->radius)) {
    //std::cout << "asteroid hit." << std::endl;
    //this asteroid is hit.
    if (a->radius > MINIMUM_ASTEROID_SIZE) {
    //break it into pieces.
    Asteroid *ta = a;
    buildAsteroidFragment(ta, a);
    int r = rand()%10+5;
    for (int k=0; k<r; k++) {
    //get the next asteroid position in the array
    Asteroid *ta = new Asteroid;
    buildAsteroidFragment(ta, a);
    //add to front of asteroid linked list
    ta->next = (*g).ahead;
    if ((*g).ahead != NULL)
    (*g).ahead->prev = ta;
    (*g).ahead = ta;
    (*g).nasteroids++;
    }
    } else {
    a->color[0] = 1.0;
    a->color[1] = 0.1;
    a->color[2] = 0.1;
    //asteroid is too small to break up
    //delete the asteroid and bullet
    Asteroid *savea = a->next;
    deleteAsteroid(&g, a);
    a = savea;
    (*g).nasteroids--;
    }
    //delete the bullet...
    memcpy(&(*g).barr[i], &(*g).barr[(*g).nbullets-1], sizeof(Bullet));
    (*g).nbullets--;
    if (a == NULL)
    break;
}
i++;
}
if (a == NULL)
    break;
    a = a->next;
    }
*/
//---------------------------------------------------
//check keys pressed now

// 1
if (gl.keyhits[49]) {
    (*g).ship.weaponType = 1;
}

// 2
if (gl.keyhits[50]) {
    (*g).ship.weaponType = 2;
}

// 3
if (gl.keyhits[51]) {
    (*g).ship.weaponType = 3;
}

// q

if (gl.keyhits[13]) {
    (*g).ship.vel -= .5;
    if ((*g).ship.vel <= 0) {
        (*g).ship.vel = 0;
    }
    //if ((*g).ship.angle >= 360.0f)
    //    (*g).ship.angle -= 360.0f;
}
// e
if (gl.keyhits[1]) {
    (*g).ship.vel += .5;
    if ((*g).ship.vel >= MAX_THRUST) {
        (*g).ship.vel = MAX_THRUST;
    }
    //if ((*g).ship.angle < 0.0f)
    //(*g).ship.angle += 360.0f;
}
// r
if (gl.keyhits[14]) {
    (*g).ship.vel = MAX_THRUST*2;
    (*g).ship.boost -= .2;
    if( (*g).ship.boost <= 0) {
	(*g).ship.boost = 0;
    }
} else {
	if ((*g).ship.vel > MAX_THRUST) {
		(*g).ship.vel = MAX_THRUST;
	}
}
	
	


// w
if (gl.keyhits[19]) {
    (*g).ship.angle[1] -= PITCH;

    if ((*g).ship.angle[1] < 0.0f) {
        (*g).ship.angle[1] = 0.0f;
    }
}
// s
if (gl.keyhits[15]) {
    (*g).ship.angle[1] += PITCH;
    if ((*g).ship.angle[1] > 180.0f) {
        (*g).ship.angle[1] = 180.0f;
    }
}
// a
if (gl.keyhits[97]) {
    (*g).ship.angle[0] += TURN;
    if ((*g).ship.angle[0] >= 360.0f)
        (*g).ship.angle[0] -= 360.0f;

    //object.drawSize[0] += 0.008333*gl.xres*TURN;
}
// d
if (gl.keyhits[0]) {
    (*g).ship.angle[0] -= TURN;
    if ((*g).ship.angle[0] < 0.0f)
        (*g).ship.angle[0] += 360.0f;

    //object.drawSize[0] -= 0.008333*gl.xres*TURN;
}
//left
if (gl.keyhits[61]) {
    //(*g).ship.pos[0] -= 5;
#ifdef USE_OPENAL_SOUND
    setActionFlag();
#endif
}

if (gl.keyhits[62]) {
}
//right
if (gl.keyhits[63]) {
    //(*g).ship.pos[0] += 5;

}
if (gl.keyhits[64]) {
}

// spacebar
if (gl.keyhits[32]) {
    //a little time between each bullet
    struct timespec bt;
    clock_gettime(CLOCK_REALTIME, &bt);
    double ts = timeDiff(&(*g).bulletTimer, &bt);
    if (ts > 0.1) {
        timeCopy(&(*g).bulletTimer, &bt);
        if ((*g).nbullets < MAX_ARRAY) {
            //shoot a bullet...
            //Bullet *b = new Bullet;
            Bullet *b = &(*g).barr[(*g).nbullets];
            timeCopy(&b->time, &bt);
            b->pos[0] = (*g).ship.pos[0];
            b->pos[1] = (*g).ship.pos[1];
            b->pos[2] = (*g).ship.pos[2];
#ifdef USE_OPENAL_SOUND
	    pewPew();
#endif
	    b->vel = (*g).ship.vel + 25;
            //convert ship angle to radians
            b->angle[0] = (*g).ship.angle[0];
            b->angle[1] = (*g).ship.angle[1];
            b->color[0] = 0.0f;
            b->color[1] = 1.0f;
            b->color[2] = 0.0f;
            (*g).nbullets++;
        }
    }
}
/*
   if (gl.keys[XK_Up]) {
//apply thrust
//convert ship angle to radians
Flt rad = (((*g).ship.angle+90.0) / 360.0f) * PI * 2.0;
//convert angle to a vector
Flt xdir = cos(rad);
Flt ydir = sin(rad);
(*g).ship.vel[0] += xdir*0.02f;
(*g).ship.vel[1] += ydir*0.02f;
Flt speed = sqrt((*g).ship.vel[0]*(*g).ship.vel[0]+
(*g).ship.vel[1]*(*g).ship.vel[1]);
if (speed > 10.0f) {
speed = 10.0f;
normalize2d((*g).ship.vel);
(*g).ship.vel[0] *= speed;
(*g).ship.vel[1] *= speed;
}
}


 */
}

void joeyRender(Game *g, Global gl)
{
    //stars
    float cx = gl.xres/2;
    float cy = gl.yres/2;
    float stars[4] = {1,1,1,.9};
    glColor4fv(stars);
    glPushMatrix();
    glBegin(GL_POINTS);
    for (int i = 0; i < (*g).num_stars; i++) {
        //float s = (*g).ship.angle[2];
        //s *= PI/180;
        //s = sin(s);
        //float c = (*g).ship.angle[2];
        //c *= PI/180;
        //c = cos(c);
        float x = (*g).ship.angle[0];
        float y = (*g).ship.angle[1];
        // converts to a x and y coordinate
        x = (*g).stars[i][0]+x;
        if (x >= 360.0f)
            x -= 360.0f;
        if (x <= 0)
            x += 360.0f;
        x = (x/120)*gl.xres;
        y = (*g).stars[i][1]+y;
        if (y >= 180.0f)
            y -= 180.0f;
        if (y <= 0)
            y += 180.0f;
        y = (y/90)*gl.yres;
        /*
           x -= cx;
           y -= cy;
           float xnew = x * c - y * s;
           float ynew = x * s + y * c;
           x = xnew + cx;
           y = ynew + cy;
         */
        glVertex2f(x,y);
    }
    glEnd();
    glPopMatrix();

    /*debris
      float tan[3];
      tan[0] = .8;
      tan[1] = .7;
      tan[2] = .55;
      for (int i = 0; i < 500; i++) {
      float dx = (*g).debris[i][0];
      float dy = (*g).debris[i][1];
      float z = (*g).debris[i][2]-((*g).thrust/10);
      if (z < 1) {
      (*g).debris[i][0] = rand() % gl.xres + 1;
      (*g).debris[i][1] = rand() % gl.yres + 1;
      (*g).debris[i][2] = rand() % gl.xres + 1;
      dx = (*g).debris[i][0];
      dy = (*g).debris[i][1];
      z = (*g).debris[i][2];
      }
      float rad = 50/z;
    //map from -1 to 1
    dx = ((dx - .5*cx)/(.5*cx));
    dy = ((dy - .5*cy)/(.5*cy));
    z = ((z - .5*cx)/(.5*cx));

    float cdx = ((*g).thrust/25)*(dx/z);
    float cdy = ((*g).thrust/25)*(dy/z);
    (*g).debris[i][0] = (cdx+1)*(cx);
    (*g).debris[i][1] = (cdy+1)*(cy);
    (*g).debris[i][2] = z;
    glColor3fv(tan);
    glPushMatrix();
    glBegin(GL_POLYGON);
    for (int j = 0; j < 5; j++) {
    float a = 2*PI/5;
    dx = (*g).debris[i][0] + rad*cos(j*a);
    dy = (*g).debris[i][1] + rad*sin(j*a);
    glVertex2f(dx,dy);
    }
    glEnd();
    glPopMatrix();
    }
     */


    // setup variables for cockpit
    float rad[4];           // radius
    rad[0] = gl.yres*.1;   // inner oct
    rad[1] = gl.yres*.105;  // octagon 2
    rad[2] = gl.yres*.35;   // ring
    rad[3] = gl.yres*.44;   // outer hex
    // 8 vertices( 0=x 1=y)
    int v8[8][2];           // inner oct
    int v8_1[8][2];         // octagon 2
    int v8_2[8][2];         // outer hex

    float chs[3][2]; //crosshairs
    chs[0][0] = gl.yres*0.04;
    chs[0][1] = gl.yres*0.01;

    if ((*g).ship.weaponType == 1) {
      	glColor3fv((*g).ship.color);
      	glPushMatrix();
  	    glBegin(GL_LINE_LOOP);
      	for (int i = 0; i < 8; i++) {
  			float x = cx + 6*cos(i*(PI/4));
  			float y = cy + 6*sin(i*(PI/4));
  			glVertex2f(x,y);
  		}
  		glEnd();
  	    glPopMatrix();
  	}
  	if ((*g).ship.weaponType == 2) {
      	glColor3fv((*g).ship.color);
      	glPushMatrix();
  	    glBegin(GL_LINES);
      	glVertex2f(cx - chs[0][0], cy);
  	    glVertex2f(cx - chs[0][1], cy);
      	glEnd();
  	    glPopMatrix();

  	    glColor3fv((*g).ship.color);
      	glPushMatrix();
  	    glBegin(GL_LINES);
      	glVertex2f(cx + chs[0][0], cy);
  	    glVertex2f(cx + chs[0][1], cy);
      	glEnd();
  	    glPopMatrix();

      	glColor3fv((*g).ship.color);
  	    glPushMatrix();
  	    glBegin(GL_LINES);
      	glVertex2f(cx, cy - chs[0][0]);
  	    glVertex2f(cx, cy - chs[0][1]);
      	glEnd();
  	    glPopMatrix();

      	glColor3fv((*g).ship.color);
  	    glPushMatrix();
  	    glBegin(GL_LINES);
  	    glVertex2f(cx, cy + chs[0][0]);
  	    glVertex2f(cx, cy + chs[0][1]);
  		glEnd();
  	    glPopMatrix();
  	}
  	if ((*g).ship.weaponType == 3) {
      	glColor3fv((*g).ship.color);
      	glPushMatrix();
  	    glBegin(GL_LINE_LOOP);
      	glVertex2f(cx - 6, cy);
  	    glVertex2f(cx - 18, cy + 6);
  	    glVertex2f(cx - 18, cy - 6);
      	glEnd();
  	    glPopMatrix();

  	    glColor3fv((*g).ship.color);
      	glPushMatrix();
  	    glBegin(GL_LINE_LOOP);
      	glVertex2f(cx, cy + 6);
      	glVertex2f(cx - 6, cy + 18);
      	glVertex2f(cx + 6, cy + 18);
      	glEnd();
  	    glPopMatrix();

      	glColor3fv((*g).ship.color);
      	glPushMatrix();
  	    glBegin(GL_LINE_LOOP);
      	glVertex2f(cx + 6, cy);
  	    glVertex2f(cx + 18, cy + 6);
  	    glVertex2f(cx + 18, cy - 6);
      	glEnd();
  	    glPopMatrix();

  	    glColor3fv((*g).ship.color);
      	glPushMatrix();
  	    glBegin(GL_LINE_LOOP);
      	glVertex2f(cx, cy - 6);
      	glVertex2f(cx - 6, cy - 18);
      	glVertex2f(cx + 6, cy - 18);
      	glEnd();
  	    glPopMatrix();
  	}

    //inner octagon
    for (int i = 0; i < 8; i++) {
        float theta = ((2.0*PI)/8.0)*i + (PI/8);
        v8[i][0] = cx + rad[0]*cos(theta);
        v8[i][1] = cy + rad[0]*sin(theta);
    }

    glColor3fv((*g).ship.color);
    glPushMatrix();
    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < 8; i++)
        glVertex2f(v8[i][0],v8[i][1]);
    glVertex2f(v8[0][0],v8[0][1]);
    glEnd();
    glPopMatrix();

    //octagon 2
    for (int i = 0; i < 8; i++) {
        float theta = ((2.0*PI)/8.0)*i + (PI/8);
        v8_1[i][0] = cx + rad[1]*cos(theta);
        v8_1[i][1] = cy + rad[1]*sin(theta);
    }

    glColor3fv((*g).ship.color);
    glPushMatrix();
    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < 8; i++)
        glVertex2f(v8_1[i][0],v8_1[i][1]);
    glVertex2f(v8_1[0][0],v8_1[0][1]);
    glEnd();
    glPopMatrix();

    for (int i = 0; i < 6; i++) {
        float theta = ((2.0*PI)/6.0)*i;
        v8_2[i][0] = cx + rad[3]*cos(theta);
        v8_2[i][1] = cy + rad[3]*sin(theta);
    }
    //top
    float HUD[3];
    HUD[0] = .6;
    HUD[1] = .65;
    HUD[2] = .6;
    glColor3fv(HUD);
    glPushMatrix();
    glBegin(GL_POLYGON);
    glVertex2f(0,gl.yres);
    glVertex2f(v8_2[2][0],v8_2[2][1]);
    glVertex2f(v8_2[1][0],v8_2[1][1]);
    glVertex2f(gl.xres,gl.yres);
    glEnd();
    glPopMatrix();


    //bottom
    glColor3fv(HUD);
    glPushMatrix();
    glBegin(GL_POLYGON);
    glVertex2f(0,0);
    glVertex2f(v8_2[4][0],v8_2[4][1]);
    glVertex2f(v8_2[5][0],v8_2[5][1]);
    glVertex2f(gl.xres,0);
    glEnd();
    glPopMatrix();

    //ring
    //float thickness = 25;
    int pts = 48;
    float a = rad[2];
    float vel[4];
    vel[0] = .3;
    vel[1] = .9;
    vel[2] = .3;
    vel[3] = (*g).ship.vel/MAX_THRUST;
    glEnable(GL_BLEND);
    glBlendFunc( GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
    glColor4fv(vel);
    glPushMatrix();
    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < pts; i++) {
        float theta = ((2*PI)/pts)*i;
        float vertx = cx + (a*cos(theta));
        float verty = cy + (a*sin(theta));
        glVertex2f(vertx,verty);
    }
    glEnd();
    glPopMatrix();

    float thrust = a + (*g).ship.vel;
    if (thrust > a + MAX_THRUST) {
		thrust = a + MAX_THRUST;
		vel[0] = vel[3] = 1;
	    vel[1] = vel[2] = 0;
	}
	while (a < thrust) {
        glColor4fv(vel);
        glPushMatrix();
        glBegin(GL_LINE_LOOP);
        for (int i = 0; i < pts; i++) {
            float theta = ((2*PI)/pts)*i;
            float vertx = cx + (a*cos(theta));
            float verty = cy + (a*sin(theta));
            glVertex2f(vertx,verty);
        }
        a += .4;
        glEnd();
        glPopMatrix();
    }

    // red triangle detector

    float Red[4] = {1,0,0,1};
    float angle = atan2((*g).object.projection[1]-cy,(*g).object.projection[0]-cx);
    float tri[3][2];
    tri[0][0] = cx + (a + 15)*cos(angle);
    tri[0][1] = cy + (a + 15)*sin(angle);
    tri[1][0] = cx + a*cos(angle+.02);
    tri[1][1] = cy + a*sin(angle+.02);
    tri[2][0] = cx + a*cos(angle-.02);
    tri[2][1] = cy + a*sin(angle-.02);
    /*
    tri[0][0] = cx + (rad[2] + 15)*cos(angle);
    tri[0][1] = cy + (rad[2] + 15)*sin(angle);
    tri[1][0] = cx + rad[2]*cos(angle+.02);
    tri[1][1] = cy + rad[2]*sin(angle+.02);
    tri[2][0] = cx + rad[2]*cos(angle-.02);
    tri[2][1] = cy + rad[2]*sin(angle-.02);
*/
    glColor4fv(Red);
    glPushMatrix();
    glBegin(GL_TRIANGLE_STRIP);
    for (int i = 0; i < 3; i++) {
        glVertex2f(tri[i][0],tri[i][1]);
    }
    glEnd();
    glPopMatrix();

    for (int i=0; i< (*g).nenemies; i++) {

        angle = atan2((*g).earr[i].projection[1]-cy,(*g).earr[i].projection[0]-cx);
        tri[0][0] = cx + (a + 15)*cos(angle);
        tri[0][1] = cy + (a + 15)*sin(angle);
        tri[1][0] = cx + a*cos(angle+.02);
        tri[1][1] = cy + a*sin(angle+.02);
        tri[2][0] = cx + a*cos(angle-.02);
        tri[2][1] = cy + a*sin(angle-.02);
	/*
	tri[0][0] = cx + (rad[2] + 15)*cos(angle);
        tri[0][1] = cy + (rad[2] + 15)*sin(angle);
        tri[1][0] = cx + rad[2]*cos(angle+.02);
        tri[1][1] = cy + rad[2]*sin(angle+.02);
        tri[2][0] = cx + rad[2]*cos(angle-.02);
        tri[2][1] = cy + rad[2]*sin(angle-.02);
        */
	Red[3] = (RANGE - (*g).earr[i].polar[0])/RANGE;
        glColor4fv(Red);
        glPushMatrix();
        glBegin(GL_TRIANGLE_STRIP);
        for (int i = 0; i < 3; i++) {
            glVertex2f(tri[i][0],tri[i][1]);
        }
        glEnd();
        glPopMatrix();
    }


    //  (*g).object.polar[1]
    //cross beams
    float tribase = 0.02;//measured in radians
    for (int i = 0; i < 8; i++) {
        float theta = ((2.0*PI)/8.0)*i + (PI/8);
        float vx1, vy1, vx2, vy2;
        vx1 = cx + (rad[2]*cos(theta-tribase));
        vy1 = cy + (rad[2]*sin(theta-tribase));
        vx2 = cx + (rad[2]*cos(theta+tribase));
        vy2 = cy + (rad[2]*sin(theta+tribase));
        glEnable(GL_BLEND);
        glBlendFunc( GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
        glColor4fv((*g).ship.color);
        glPushMatrix();
        glBegin(GL_TRIANGLES);
        glVertex2f(v8_1[i][0], v8_1[i][1]);
        glVertex2f(vx1,vy1);
        glVertex2f(vx2,vy2);
        glEnd();
        glPopMatrix();
    }

  	// boost indicator
    for (int i = 0; i < 8; i++) {
        float theta = ((2.0*PI)/8.0)*i + (PI/8);
        float vx1, vy1, vx2, vy2;
		// boost percent of full
		float bp = (*g).ship.boost/(*g).ship.maxBoost;
		if (bp <= 0)
	    	bp = 0;
		vx1 = cx + (bp*rad[2])*cos(theta-tribase);
    	vy1 = cy + (bp*rad[2])*sin(theta-tribase);
	    vx2 = cx + (bp*rad[2])*cos(theta+tribase);
    	vy2 = cy + (bp*rad[2])*sin(theta+tribase);
	    glEnable(GL_BLEND);
    	glBlendFunc( GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
		float boost[4] = {1,1,0,1};
        glColor4fv(boost);
        glPushMatrix();
        glBegin(GL_TRIANGLES);
        glVertex2f(v8_1[i][0], v8_1[i][1]);
        glVertex2f(vx1,vy1);
        glVertex2f(vx2,vy2);
        glEnd();
        glPopMatrix();
    }
    
    // fill octagon ring
    for (int i = 0; i < 8; i++) {
        int j = (i + 1) % 8;
        glEnable(GL_BLEND);
        glBlendFunc( GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
        glColor4fv((*g).ship.color);
        glPushMatrix();
        glBegin(GL_POLYGON);
        glVertex2f(v8[i][0], v8[i][1]);
        glVertex2f(v8_1[i][0], v8_1[i][1]);
        glVertex2f(v8_1[j][0], v8_1[j][1]);
        glVertex2f(v8[j][0], v8[j][1]);
        glVertex2f(v8[i][0], v8[i][1]);
        glEnd();
        glPopMatrix();
    }

    float blue[4];
    blue[0] = 0;
    blue[1] = 0;
    blue[2] = .4;
    blue[3] = .7;

    // radar
    float radar[3]; // 0,1 = x,y of center of radar, 2 = + radius
    radar[0] = gl.xres*.50;
    radar[1] = radar[2] = 100;
    glEnable(GL_BLEND);
    glBlendFunc( GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
    glColor4fv(blue);
    glPushMatrix();
    glBegin(GL_POLYGON);
    int p = 50;
    for (int i = 0; i < p; i++) {
        float x = radar[0] + radar[2]*cos(i*2*PI/p);
        float y = radar[1] + radar[2]*sin(i*2*PI/p);
        glVertex2f(x,y);
    }
    glEnd();
    glPopMatrix();



    //green pizza slice inside radar
    float green[4];
    green[0] = 0;
    green[1] = .5;
    green[2] = 0;
    green[3] = (1-(.9*(*g).ship.angle[1]/180));
    glEnable(GL_BLEND);
    glBlendFunc( GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
    glColor4fv(green);
    glPushMatrix();
    glBegin(GL_POLYGON);
    p = 12;
    glVertex2f(radar[0],radar[1]);
    for (int i = 0; i <= p; i++) {
        float c;
        c = (*g).ship.angle[0];
        c *= PI/180;
        c -= PI/3;
        float x = radar[0] + radar[2]*cos(c + i*(PI/18));
        float y = radar[1] + radar[2]*sin(c + i*(PI/18));
        glVertex2f(x,y);
    }
    glVertex2f(radar[0],radar[1]);
    glEnd();
    glPopMatrix();

    // yellow blips on radar
    float Yellow[4] = {1, 1, 0, 1};
    for (int i=0; i< (*g).nenemies; i++) {
        float dist = (*g).earr[i].polar[0];
        if (dist < RANGE) {
            float ang = (*g).earr[i].polar[1];
            ang *= PI/180;
            float xblip = radar[0] + (dist/RANGE)*radar[2]*cos(ang);
            float yblip = radar[1] + (dist/RANGE)*radar[2]*sin(ang);
            Yellow[3] = (180 - (*g).earr[i].polar[2])/180;
            glEnable(GL_BLEND);
            glBlendFunc( GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
            glColor4fv(Yellow);
            glPushMatrix();
            glBegin(GL_POLYGON);
            glVertex2f(xblip - 4, yblip);
            glVertex2f(xblip, yblip + 4);
            glVertex2f(xblip + 4, yblip);
            glVertex2f(xblip, yblip - 4);
            glEnd();
            glPopMatrix();
        }
    }
    //shield
    float lt_edge = cx*.99;
    float rt_edge = cx*.94;
    float shield[4] = {0,0,1,1};
    float bAngle = PI + .1;
    int bar = 10;
    for (int i = 0; i < 10; i++) {
	float sz = .02; // height of bars in radian sections
	if ((*g).ship.currentShield > bar) {
	    shield[3] = 1;
	}
	else
	    shield[3] = (*g).ship.currentShield/bar;
    	glColor4fv(shield);
	glPushMatrix();
    	glBegin(GL_POLYGON);
	glVertex2f(cx + rt_edge*cos(bAngle - i*sz) , 100 + rt_edge*sin(bAngle - i*sz));
	glVertex2f(cx + rt_edge*cos(bAngle - (i+1)*sz) , 100 + rt_edge*sin(bAngle - (i+1)*sz));
	glVertex2f(cx + lt_edge*cos(bAngle - (i+1)*sz) , 100 + lt_edge*sin(bAngle - (i+1)*sz));
	glVertex2f(cx + lt_edge*cos(bAngle - i*sz) ,100 + lt_edge*sin(bAngle - i*sz));
	rt_edge += 1;
	bAngle -= .01;
	glEnd();
	glPopMatrix();
	if ((*g).ship.currentShield < bar) {
	    break;
	}
	bar += 10;
	
    }
    
    //health
    lt_edge = cx*.94;
    rt_edge = cx*.99;
    float health[4] = {0,1,0,1};
    bAngle = -.1;
    for (int i = 0; i < 10; i++) {
	float sz = .02;

    	glColor4fv(health);
	glPushMatrix();
    	glBegin(GL_POLYGON);
	glVertex2f(cx + rt_edge*cos(bAngle + i*sz) , 100 + rt_edge*sin(bAngle + i*sz));
	glVertex2f(cx + rt_edge*cos(bAngle + (i+1)*sz) , 100 + rt_edge*sin(bAngle + (i+1)*sz));
	glVertex2f(cx + lt_edge*cos(bAngle + (i+1)*sz) , 100 + lt_edge*sin(bAngle + (i+1)*sz));
	glVertex2f(cx + lt_edge*cos(bAngle + i*sz) , 100 + lt_edge*sin(bAngle + i*sz));
	lt_edge += 1;
	bAngle +=.01;
	glEnd();
	glPopMatrix();
    }


/*
    // 2nd radar
     //float radar[3]; // 0,1 = x,y of center of radar, 2 = + radius
     radar[0] = gl.xres*.90;
     radar[1] = radar[2] = 100;
     glEnable(GL_BLEND);
     glBlendFunc( GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
     glColor4fv(blue);
     glPushMatrix();
     glBegin(GL_POLYGON);
     p = 50;
     for (int i = 0; i < p; i++) {
         float x = radar[0] + radar[2]*cos(i*2*PI/p);
         float y = radar[1] + radar[2]*sin(i*2*PI/p);
         glVertex2f(x,y);
     }
     glEnd();
     glPopMatrix();

    // Gyroscope
    float sd = 40;
    float cen = 80;
    //float lft,rt;
    float vertices[5][3];
    vertices[0][0] = cx-sd;
    vertices[0][1] = cen-sd;
    vertices[1][0] = cx-sd;
    vertices[1][1] = cen+sd;
    vertices[2][0] = cx+sd;
    vertices[2][1] = cen+sd;
    vertices[3][0] = cx+sd;
    vertices[3][1] = cen-sd;
    vertices[4][0] = cx;
    vertices[4][1] = cen;
    glColor4fv(blue);
    glPushMatrix();
    glBegin(GL_POLYGON);
    vertices[4][0] = cx;
    vertices[4][1] = cen;
    for (int i = 0; i < 5; i++) {
        glVertex2f(vertices[i][0],vertices[i][1]);
    }
    glEnd();
    glPopMatrix();

    Rect r;
    //
    r.bot = 100;
    r.left = cx-34;
    r.center = 0;
    //ggprint8b(&r, 16, 0x00ff0000, "3350 - Asteroids");
    ggprint8b(&r, 16, 0x00ffff00, "xScale = %.1f, yScale = %.1f",(*g).object.xScale, (*g).object.yScale);
    ggprint8b(&r, 16, 0x00ffff00, "n bullets: %i", (*g).nbullets);
    ggprint8b(&r, 16, 0x00ffff00, "%.1f : %.1f ", (*g).ship.angle[0], (*g).ship.angle[1]);
    //ggprint8b(&r, 16, 0x00ffff00, "%.1f:%.1f,roll=%.1f)", (*g).ship.angle[0], (*g).ship.angle[1], (*g).ship.angle[2]);
    //ggprint8b(&r, 16, 0x00ffff00, "(x=%.1f,y=%.1f)", (*g).ship.pos[0], (*g).ship.pos[1]);
    ggprint8b(&r, 16, 0x00ffff00, "(x=%.1f,y=%.1f,z=%.1f)", (*g).ship.pos[0], (*g).ship.pos[1], (*g).ship.pos[2]);

    ggprint8b(&r, 16, 0x00ffff00, "(object x=%.1f,object y=%.1f,object z=%.1f)", (*g).object.pos[0], (*g).object.pos[1], (*g).object.pos[2]);

    //ggprint8b(&r, 16, 0x00ffff00, "(distance x=%.1f)", sqrt((pow((*g).ship.pos[0] - object.pos[0],2)) + (pow((*g).ship.pos[1] - object.pos[1],2))) );

    //ggprint8b(&r, 16, 0x00ffff00, "(object render x=%.1f,object render y=%.1f)", object.polar[1],object.polar[2]);

    ggprint8b(&r, 16, 0x00ffff00, "distance p0 = %.1f",(*g).object.polar[0]);
    ggprint8b(&r, 16, 0x00ffff00, "p1:xy = %.1f, fov calc = %.1f",(*g).object.polar[1], (*g).ship.angle[0] + 60 - (*g).object.polar[1]);
    ggprint8b(&r, 16, 0x00ffff00, "p2:z = %.1f",(*g).object.polar[2]);
    */
}
