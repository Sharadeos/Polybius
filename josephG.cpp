//
//program: josephG.cpp
//author: Joseph Gaede
//
#include <GL/glx.h>
#include "fonts.h"
#include "classes.h"
#include <math.h>
typedef float Vec[3];
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



void fighterPF(Game *g, Global gl,int e)
{
    (*g).earr[e].type = 1;
    (*g).earr[e].vel = 1;
    //calculates xy angle and z angle
    float a_xy, a_z;
    a_xy = (*g).earr[e].angle[0];
    a_xy *= PI/180;
    a_z = (*g).earr[e].angle[1];
    a_z *= PI/180;
    //updates x,y,and z positions
    (*g).earr[e].pos[0] += (*g).earr[e].vel*cos(a_xy)*sin(a_z);
    (*g).earr[e].pos[1] += (*g).earr[e].vel*sin(a_xy)*sin(a_z);
    (*g).earr[e].pos[2] += (*g).earr[e].vel*cos(a_z);
    //checks to see if enemy ship is out of range
    if ((*g).earr[e].polar[0] > RANGE-200) {	
        if ((*g).earr[e].polar[1] < 180)
            (*g).earr[e].angle[0] = (*g).earr[e].polar[1] + 180;
	else
            (*g).earr[e].angle[0] = (*g).earr[e].polar[1] - 180;
			
    (*g).earr[e].angle[1] = 180-(*g).earr[e].polar[2];
    (*g).earr[e].type = 2;
    return;
    }
    //keeps xy angle from going above 360 and z angle from going above 180
    float xy =(*g).earr[e].polar[1]+180;
    if (xy > 360)
        xy -= 360;
    float z = (*g).earr[e].polar[2]+90;
    if (z > 180)
        z -= 180;
    //basic movement
    if ((*g).earr[e].type == 1) {
       if ((*g).earr[e].angle[0] < xy-1) {
           (*g).earr[e].angle[0] += .5;
       }
       if ((*g).earr[e].angle[0] > xy+1) {
           (*g).earr[e].angle[0] -= .5;	    
       }
       if ((*g).earr[e].angle[1] < (*g).ship.angle[1])
           (*g).earr[e].angle[1] += .1;
	    
       if ((*g).earr[e].angle[1] > (*g).ship.angle[1])
	   (*g).earr[e].angle[1] -= .1;
       return;
    }
    //lockon movement
    if ((*g).earr[e].type == 2) {
      if ((*g).earr[e].angle[0] < (*g).ship.angle[0])
          (*g).earr[e].angle[0] += .5;
      else
         (*g).earr[e].angle[0] -= .5;

      if ((*g).earr[e].angle[1] < (*g).ship.angle[1])
          (*g).earr[e].angle[1] += .5;
      else
         (*g).earr[e].angle[1] -= .5;

      if ((*g).earr[e].polar[0] < 20)
          (*g).earr[e].type = 1;
      return;	    
    }
	
    //evasion movement   
    if ((*g).earr[e].projection[0] < gl.xres && (*g).earr[e].projection[0] > 0 &&
        (*g).earr[e].projection[1] < gl.yres && (*g).earr[e].projection[1] > 0 && (*g).earr[e].type != 2) {
	   (*g).earr[e].type = 3;
           (*g).earr[e].angle[0] += rand()%181 - 90;
	}
	
    if ((*g).earr[e].type == 3) {
       if ((*g).earr[e].angle[0] < xy-1) {
           (*g).earr[e].angle[0] += (((rand()%4001)+1000)*.0001);
       }else{
           (*g).earr[e].angle[0] -= (((rand()%4001)+1000)*.0001);
       }
    (*g).earr[e].type = 4;
    return;
    }
    
    if ((*g).earr[e].type == 4) {
       if ((*g).earr[e].polar[0] == 500)
          (*g).earr[e].type = 1;
    return;
    }
}
void carrierPF(Game *g, Global gl,int e)
{
    //set initial movement type and velocity   
    (*g).earr[e].type = 1;
    (*g).earr[e].vel = .4;
    
    //calculate angles
    float a_xy, a_z;
    a_xy = (*g).earr[e].angle[0];
    a_xy *= PI/180;
    a_z = (*g).earr[e].angle[1];
    a_z *= PI/180;
    
    //update positions
    (*g).earr[e].pos[0] += (*g).earr[e].vel*cos(a_xy)*sin(a_z);
    (*g).earr[e].pos[1] += (*g).earr[e].vel*sin(a_xy)*sin(a_z);
    (*g).earr[e].pos[2] += (*g).earr[e].vel*cos(a_z);
    
    //check if out of range    
    if ((*g).earr[e].polar[0] > RANGE-200) {
       if ((*g).earr[e].polar[1] < 180)
          (*g).earr[e].angle[0] = (*g).earr[e].polar[1] + 180;
       else
          (*g).earr[e].angle[0] = (*g).earr[e].polar[1] - 180;
			
       (*g).earr[e].angle[1] = 180-(*g).earr[e].polar[2];
       (*g).earr[e].type = 2;
    return;
    }
    //makes sure xy angle is within 360 degrees and z angle is within 180
    float xy =(*g).earr[e].polar[1]+180;
    if (xy > 360)
	xy -= 360;
    float z = (*g).earr[e].polar[2]+90;
    if (z > 180)
	z -= 180;
    
    //normal movement
    if ((*g).earr[e].type == 1) {
      if ((*g).earr[e].angle[0] < xy-1) {
          (*g).earr[e].angle[0] += .2; 
      }
      if ((*g).earr[e].angle[0] > xy+1) {
          (*g).earr[e].angle[0] -= .2; 
      }
      if ((*g).earr[e].angle[1] < (*g).ship.angle[1])
          (*g).earr[e].angle[1] += .1;
            
      if ((*g).earr[e].angle[1] > (*g).ship.angle[1])
          (*g).earr[e].angle[1] -= .1;
    return;
    }
    
    //lockon movement
    if ((*g).earr[e].type == 2) {	    	    
      if ((*g).earr[e].angle[0] < (*g).ship.angle[0])
       	 (*g).earr[e].angle[0] += .2;
      else
	 (*g).earr[e].angle[0] -= .2;
	    	    	    
      if ((*g).earr[e].angle[1] < (*g).ship.angle[1])
	 (*g).earr[e].angle[1] += .2;
      else
         (*g).earr[e].angle[1] -= .2;
	    	    
      if ((*g).earr[e].polar[0] < 20)
         (*g).earr[e].type = 1;
    return;	    
    }
	
    //evasion movement
    if ((*g).earr[e].projection[0] < gl.xres && (*g).earr[e].projection[0] > 0 &&
        (*g).earr[e].projection[1] < gl.yres && (*g).earr[e].projection[1] > 0 && (*g).earr[e].type != 2) {
            (*g).earr[e].type = 3;
            (*g).earr[e].angle[0] += rand()%181 - 90;
    }

    if ((*g).earr[e].type == 3) {	    
       if ((*g).earr[e].angle[0] < xy-1) {	
           (*g).earr[e].angle[0] += (((rand()%4001)+1000)*.0001);	    
       }else{	
           (*g).earr[e].angle[0] -= (((rand()%4001)+1000)*.0001);
       }	    
       (*g).earr[e].type = 4;
    return;
    }
	
    if ((*g).earr[e].type == 4) {
       if ((*g).earr[e].polar[0] == 500)
	   (*g).earr[e].type = 1;
    return;
    }
}
void destroyerPF(Game *g, Global gl, int e)
{
    //initial movement type and velocity
    (*g).earr[e].type = 1;
    (*g).earr[e].vel = .2;
    
    //calculate angles	
    float a_xy, a_z;
    a_xy = (*g).earr[e].angle[0];
    a_xy *= PI/180;
    a_z = (*g).earr[e].angle[1];
    a_z *= PI/180;
    (*g).earr[e].pos[0] += (*g).earr[e].vel*cos(a_xy)*sin(a_z);
    (*g).earr[e].pos[1] += (*g).earr[e].vel*sin(a_xy)*sin(a_z);
    (*g).earr[e].pos[2] += (*g).earr[e].vel*cos(a_z);
    
    //check to see if out of range
    if ((*g).earr[e].polar[0] > RANGE-200) {
       if ((*g).earr[e].polar[1] < 180)
           (*g).earr[e].angle[0] = (*g).earr[e].polar[1] + 180;
       else
           (*g).earr[e].angle[0] = (*g).earr[e].polar[1] - 180;

       (*g).earr[e].angle[1] = 180-(*g).earr[e].polar[2];
       (*g).earr[e].type = 2;
    return;
    }
   
    //keep xy angle within 360 and z angle within 180
    float xy =(*g).earr[e].polar[1]+180;
    if (xy > 360)
        xy -= 360;
    float z = (*g).earr[e].polar[2]+90;
    if (z > 180)
        z -= 180;
    
    //initial movement
    if ((*g).earr[e].type == 1) {
      if ((*g).earr[e].angle[0] < xy-1) {
          (*g).earr[e].angle[0] += .2;
      }
      if ((*g).earr[e].angle[0] > xy+1) {
          (*g).earr[e].angle[0] -= .2;
      }
      if ((*g).earr[e].angle[1] < (*g).ship.angle[1])
          (*g).earr[e].angle[1] += .1;

      if ((*g).earr[e].angle[1] > (*g).ship.angle[1])
          (*g).earr[e].angle[1] -= .1;
    return;
    }
 
    //lockon movement
    if ((*g).earr[e].type == 2) {	    	    
      if ((*g).earr[e].angle[0] < (*g).ship.angle[0])
	  (*g).earr[e].angle[0] += .2;
      else
	  (*g).earr[e].angle[0] -= .2;
	    	    	    
      if((*g).earr[e].angle[1] < (*g).ship.angle[1])
         (*g).earr[e].angle[1] += .2;
      else
         (*g).earr[e].angle[1] -= .2;
	    	    
      if((*g).earr[e].polar[0] < 20)
         (*g).earr[e].type = 1;
    return;	    
    }
	
    //evasion movement
    if ((*g).earr[e].projection[0] < gl.xres && (*g).earr[e].projection[0] > 0 &&
        (*g).earr[e].projection[1] < gl.yres && (*g).earr[e].projection[1] > 0 && (*g).earr[e].type != 2) {
	    (*g).earr[e].type = 3;
	    (*g).earr[e].angle[0] += rand()%181 - 90;
    }
    	
    if ((*g).earr[e].type == 3) {
       if ((*g).earr[e].angle[0] < xy-1) {
           (*g).earr[e].angle[0] += (((rand()%4001)+1000)*.0001);
       }else{
           (*g).earr[e].angle[0] -= (((rand()%4001)+1000)*.0001);
       }
       (*g).earr[e].type = 4;
    return;
    }
    //back to normal movement
    if ((*g).earr[e].type == 4) {
       if ((*g).earr[e].polar[0] == 500)
	   (*g).earr[e].type = 1;
    return;
    }
}
void pathFindingTest2(Game *g,Global gl)
{
    //tests every element of the enemy array
    //for which pathfinding type to use
    //0 = fighter, 1 = carrier, 2 = destroyer
    for (int i = 0; i < (*g).nenemies; i++) {
	if ((*g).earr[i].enemyType == 0) {
	    fighterPF(g,gl,i);
	}
	if ((*g).earr[i].enemyType == 1) {
	    carrierPF(g,gl,i);
	}
	if ((*g).earr[i].enemyType == 2) {
	    destroyerPF(g,gl,i);

	}
    }
}
void score(Game *g,int i)
{
    //increments score based on enemy destroyed
    //100 for fighter, 500 for destroyer, 1000 for carrier
    if ((*g).earr[i].enemyType == 0) 
	(*g).score += 100;
    if ((*g).earr[i].enemyType == 1)
        (*g).score += 1000;
    if ((*g).earr[i].enemyType == 2)
	(*g).score += 500;
}
void scoreBoard(Game *g, Global gl)
{
    //displays player score at top of ship interface
    Rect r;
    r.bot = gl.yres-100;
    r.left = (gl.xres/2)-34;
    r.center = 0;
    ggprint8b(&r,16,0x00ffff00,"score:  %i",(*g).score);


}
