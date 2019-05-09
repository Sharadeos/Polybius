//
//program: andrewH.cpp
//author: Andrew (Joey) Hubbard
//Last Modified: 5-7-2019
//
//#include <GL/glx.h>
//#include "fonts.h"
//#include <math.h>
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
        if (ts > 5 && ( b->type==1 || b->type == 2)) {
            //time to delete the bullet.
            memcpy(&(*g).barr[i], &(*g).barr[(*g).nbullets-1],
                    sizeof(Bullet));
            (*g).nbullets--;
            //do not increment i.
            continue;
        }
        if (ts > 1.5 && b->type == 3) {
            b->vel = 0;
            b->type = 4;
        }
        if (ts > 1.5 && b->type == 4) {
            b->radius *= 1.14;
        }
        if (ts > 2.5 && b->type == 4) {
            //time to delete the bullet.
            memcpy(&(*g).barr[i], &(*g).barr[(*g).nbullets-1],
                    sizeof(Bullet));
            (*g).nbullets--;
            //do not increment i.
            continue;
        }
        a_xy = b->angle[0];
        a_xy *= PI/180;
        a_z = b->angle[1];
        a_z*= PI/180;
        b->pos[0] += b->vel*cos(a_xy)*sin(a_z);
        b->pos[1] += b->vel*sin(a_xy)*sin(a_z);
        b->pos[2] += b->vel*cos(a_z);
        b->updatePolar((*g).ship.pos);
        i++;
    }
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
    }

    // e
    if (gl.keyhits[1]) {
        (*g).ship.vel += .5;
        if ((*g).ship.vel >= MAX_THRUST) {
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
    }
    // d
    if (gl.keyhits[0]) {
        (*g).ship.angle[0] -= TURN;
        if ((*g).ship.angle[0] < 0.0f)
            (*g).ship.angle[0] += 360.0f;

    }
    //left
    if (gl.keyhits[61]) {
#ifdef USE_OPENAL_SOUND
        setActionFlag();
#endif
    }

    if (gl.keyhits[62]) {
    }
    //right
    if (gl.keyhits[63]) {

    }
    if (gl.keyhits[64]) {
    }

    // spacebar
    if (gl.keyhits[32]) {
        if ((*g).ship.weaponType == 1) {
            //a little time between each bullet
            struct timespec bt;
            clock_gettime(CLOCK_REALTIME, &bt);
            double ts = timeDiff(&(*g).ship.bulletTimer, &bt);
            if (ts > 0.1) {
                timeCopy(&(*g).ship.bulletTimer, &bt);
                if ((*g).nbullets < MAX_ARRAY) {
                    //shoot a bullet...
                    Bullet *b = &(*g).barr[(*g).nbullets];
                    timeCopy(&b->time, &bt);
                    b->pos[0] = (*g).ship.pos[0];
                    b->pos[1] = (*g).ship.pos[1];
                    b->pos[2] = (*g).ship.pos[2];
#ifdef USE_OPENAL_SOUND
                    pewPew();
#endif
                    b->vel = (*g).ship.vel + 25;
                    b->angle[0] = (*g).ship.angle[0];
                    b->angle[1] = (*g).ship.angle[1];
                    b->type = 1;
                    b->radius = 15;
										b->enemyBullet = false;
                    (*g).nbullets++;
                }
            }
        }
        if ((*g).ship.weaponType == 2) {
            //a little time between each bullet
            struct timespec bt;
            clock_gettime(CLOCK_REALTIME, &bt);
            double ts = timeDiff(&(*g).ship.bulletTimer, &bt);
            if (ts > 0.1) {
                timeCopy(&(*g).ship.bulletTimer, &bt);
                if ((*g).nbullets < MAX_ARRAY - 1) {
                    float a1, a2, a3;
                    a1 = (*g).ship.angle[0] + 90;
                    a2 = (*g).ship.angle[0] - 90;
                    a3 = (*g).ship.angle[1];
                    a1 *= PI/180;
                    a2 *= PI/180;
                    a3 *= PI/180;
                    float xo,yo;
                    xo = 8*cos(a1)*sin(a3);
                    yo = 8*sin(a1)*sin(a3);
                    //shoot a bullet...
                    Bullet *b = &(*g).barr[(*g).nbullets];
                    timeCopy(&b->time, &bt);
                    b->pos[0] = (*g).ship.pos[0] + xo;
                    b->pos[1] = (*g).ship.pos[1] + yo;
                    b->pos[2] = (*g).ship.pos[2];
                    b->vel = (*g).ship.vel + 25;
                    b->angle[0] = (*g).ship.angle[0];
                    b->angle[1] = (*g).ship.angle[1];
                    b->type = 1;
                    b->radius = 15;
										b->enemyBullet = false;
                    (*g).nbullets++;
                    //shoot a bullet...
                    xo = 8*cos(a2)*sin(a3);
                    yo = 8*sin(a2)*sin(a3);
                    Bullet *c = &(*g).barr[(*g).nbullets];
                    timeCopy(&c->time, &bt);
                    c->pos[0] = (*g).ship.pos[0] + xo;
                    c->pos[1] = (*g).ship.pos[1] + yo;
                    c->pos[2] = (*g).ship.pos[2];
                    //b->vel = (*g).ship.vel + 25;
                    c->vel = (*g).ship.vel + 25;
                    //convert ship angle to radians
                    c->angle[0] = (*g).ship.angle[0];
                    c->angle[1] = (*g).ship.angle[1];
                    c->type = 1;
                    c->radius = 15;
										c->enemyBullet = false;
                    (*g).nbullets++;
                }
            }
        }
        if ((*g).ship.weaponType == 3) {
            //a little time between each bullet
            struct timespec bt;
            clock_gettime(CLOCK_REALTIME, &bt);
            double ts = timeDiff(&(*g).ship.bulletTimer, &bt);
            if (ts > 0.5) {
                timeCopy(&(*g).ship.bulletTimer, &bt);
                if ((*g).nbullets < MAX_ARRAY) {
                    //shoot a bullet...
                    Bullet *b = &(*g).barr[(*g).nbullets];
                    timeCopy(&b->time, &bt);
                    b->pos[0] = (*g).ship.pos[0];
                    b->pos[1] = (*g).ship.pos[1];
                    b->pos[2] = (*g).ship.pos[2];
#ifdef USE_OPENAL_SOUND
                    pewPew();
#endif
                    b->vel = 25;
                    b->angle[0] = (*g).ship.angle[0];
                    b->angle[1] = (*g).ship.angle[1];
                    b->type = 3;
                    b->radius = 15;
										b->enemyBullet = false;
                    (*g).nbullets++;
                }
            }
        }
    }
}
void joeyRender(Game *g, Global gl)
{
    //stars
    float cx = gl.xres/2;
    float cy = gl.yres/2;
    float stars[4] = {1,1,1,1};

    glPushMatrix();
    glBegin(GL_POINTS);
    for (int i = 0; i < (*g).num_stars; i++) {

        stars[3] = (*g).stars[i][2];
        glColor4fv(stars);

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
        glVertex2f(x,y);
    }
    glEnd();
    glPopMatrix();

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
        float cursor[3] = {1,1,1};
        glColor3fv(cursor);
        glPushMatrix();
        glBegin(GL_LINE_LOOP);
        glVertex2f(cx - 6, cy);
        glVertex2f(cx - 18, cy + 6);
        glVertex2f(cx - 18, cy - 6);
        glEnd();
        glPopMatrix();

        glColor3fv(cursor);
        glPushMatrix();
        glBegin(GL_LINE_LOOP);
        glVertex2f(cx, cy + 6);
        glVertex2f(cx - 6, cy + 18);
        glVertex2f(cx + 6, cy + 18);
        glEnd();
        glPopMatrix();

        glColor3fv(cursor);
        glPushMatrix();
        glBegin(GL_LINE_LOOP);
        glVertex2f(cx + 6, cy);
        glVertex2f(cx + 18, cy + 6);
        glVertex2f(cx + 18, cy - 6);
        glEnd();
        glPopMatrix();

        glColor3fv(cursor);
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
        if ((*g).ship.boost > 0) {
            vel[0] = vel[3] = 1;
            vel[1] = vel[2] = 0;
        } else {
            vel[0] = .3;
            vel[1] = .9;
            vel[2] = .3;
            vel[3] = (*g).ship.vel/MAX_THRUST;
        }
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
    float bp = (*g).ship.boost/(*g).ship.maxBoost;
    if (bp <= 0) {
        bp = 0;
    }
    else {
        for (int i = 0; i < 8; i++) {
            float theta = ((2.0*PI)/8.0)*i + (PI/8);
            float vx1, vy1, vx2, vy2;
            // boost percent of full
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

    blue[3]=1;
    glEnable(GL_BLEND);
    glBlendFunc( GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
    glColor4fv(blue);
    glPushMatrix();
    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < p; i++) {
        float x = radar[0] + .25*radar[2]*cos(i*2*PI/p);
        float y = radar[1] + .25*radar[2]*sin(i*2*PI/p);
        glVertex2f(x,y);
    }
    glEnd();
    glPopMatrix();

    glEnable(GL_BLEND);
    glBlendFunc( GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
    glColor4fv(blue);
    glPushMatrix();
    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < p; i++) {
        float x = radar[0] + .5*radar[2]*cos(i*2*PI/p);
        float y = radar[1] + .5*radar[2]*sin(i*2*PI/p);
        glVertex2f(x,y);
    }
    glEnd();
    glPopMatrix();

    glEnable(GL_BLEND);
    glBlendFunc( GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
    glColor4fv(blue);
    glPushMatrix();
    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < p; i++) {
        float x = radar[0] + .75*radar[2]*cos(i*2*PI/p);
        float y = radar[1] + .75*radar[2]*sin(i*2*PI/p);
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
            shield[3] = ((int)(*g).ship.currentShield % 10) /10;
        glEnable(GL_BLEND);
        glBlendFunc( GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
        glColor4fv(shield);
        glPushMatrix();
        glBegin(GL_POLYGON);
        glVertex2f( cx + rt_edge * cos(bAngle - i * sz) 		,
                100 + rt_edge * sin(bAngle - i * sz)       	);
        glVertex2f( cx + rt_edge * cos(bAngle - (i + 1) * sz)  	,
                100 + rt_edge * sin(bAngle - (i + 1) * sz) 	);
        glVertex2f( cx + lt_edge * cos(bAngle - (i + 1) * sz)  	,
                100 + lt_edge * sin(bAngle - (i + 1) * sz)	);
        glVertex2f( cx + lt_edge * cos(bAngle - i * sz) 	 	,
                100 + lt_edge * sin(bAngle - i * sz)		);

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
    bar = 10;
    for (int i = 0; i < 10; i++) {
        float sz = .02;
        if ((*g).ship.currentHealth > bar) {
            health[3] = 1;
        }
        else
            health[3] = ((int)(*g).ship.currentHealth % 10) /10;
        glEnable(GL_BLEND);
        glBlendFunc( GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
        glColor4fv(health);
        glPushMatrix();
        glBegin(GL_POLYGON);
        glVertex2f( cx + rt_edge * cos(bAngle + i * sz) 		,
                100 + rt_edge*sin(bAngle + i*sz)			);
        glVertex2f( cx + rt_edge * cos(bAngle + (i + 1) * sz) 	,
                100 + rt_edge * sin(bAngle + (i + 1) * sz)	);
        glVertex2f( cx + lt_edge * cos(bAngle + (i + 1) * sz) ,
                100 + lt_edge * sin(bAngle + ( i + 1) * sz)	);
        glVertex2f( cx + lt_edge * cos(bAngle + i * sz) 		,
                100 + lt_edge * sin(bAngle + i * sz)		);

        lt_edge += 1;
        bAngle +=.01;
        glEnd();
        glPopMatrix();
        if ((*g).ship.currentHealth < bar) {
            break;
        }
        bar += 10;
    }
}
