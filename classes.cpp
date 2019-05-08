#include "classes.h"

Global::Global() {
  xres = 1920;
  yres = 1080;
  mousecode = 0;
  //memset(keys, 0, 65536);
}


Base::Base() {
  VecZero(dir);
  VecZero(vec);
  VecZero(projection);
  angle[0] = 90;	// xy plane (360) x is right left, y is forward backwards
  angle[1] = 90;	// z angle (180) 0 = up, 180 = down
  pos[0] = 0;
  pos[1] = 0;
  pos[2] = 0;

  xyz[0] = 0.0;
  xyz[1] = 0.0;
  xyz[2] = 0.0;
  xyz2[0] = 0.0;
  xyz2[1] = 0.0;
  xyz2[2] = 0.0;
  polar[0] = 0.0;
  polar[1] = 0.0;
  polar[2] = 0.0;

  xScale = 0.0;
  yScale = 0.0;

  radius = 100;

  currentHealth = 3;
  maxHealth = 3;

  boost = 100.0;

  clock_gettime(CLOCK_REALTIME, &bulletTimer);
  clock_gettime(CLOCK_REALTIME, &thrustTimer);
  clock_gettime(CLOCK_REALTIME, &shieldTimer);
}

void Base::updatePolar(Vec ship) {

  xyz[0] = pos[0] - ship[0];
  xyz2[0] = xyz[0]*xyz[0];

  xyz[1] = pos[1] - ship[1];
  xyz2[1] = xyz[1] * xyz[1];

  xyz[2] = pos[2] - ship[2];
  xyz2[2] = xyz[2]*xyz[2];

  // r, distance
  polar[0] = sqrt(xyz2[0] + xyz2[1] + xyz2[2]);
  // theta, z angle
  polar[2] = acos(xyz[2]/polar[0]);
  polar[2] *= 180/PI;

  // xy angle
  if (xyz[0]) {
    polar[1] = atan2(xyz[1],xyz[0]);
    polar[1] *= 180/PI;
  }
  else
    polar[1] = 90.0;
  if (polar[1] > 180) {
     polar[1] -= 180;
     polar[1] = 180 - polar[2];
  }
}

void Base::drawBase(Game * g, Global gl) {

  float e[3];

  e[0] = 0;
  e[1] = polar[1];
  e[2] = polar[2];

  if (e[1] < 0) {
    e[1] = 360 + e[1];
  }

  float s[2];
  s[0] = (*g).ship.angle[0];
  s[1] = (*g).ship.angle[1];
  float low, high;
  low = s[0] - 60;
  high = s[0] + 60;
  high -= low;
  e[1] -= low;

  if (e[1] > 360) {
    e[1] -= 360;
   }
   if (e[1] < 0) {
     e[1] += 360;
   }

  projection[0] = ((high - e[1])/120)*gl.xres;
  projection[1] = ((s[1] + 45 - e[2])/90)*gl.yres;
  float x = projection[0];
  float y = projection[1];
  float tempValue = 0;


  //Scale max at the right edge of the setup_screen

  xScale = ((high - e[1])/60);
  yScale = ((s[1] + 45 - e[2])/45);

  if (xScale  > 1.0) {
    tempValue = xScale - 1.0;
    xScale = 1.0;
    xScale = xScale - tempValue;
  }

  if (yScale  > 1.0) {
    tempValue = yScale - 1.0;
    yScale = 1.0;
    yScale = yScale - tempValue;
  }
  float distanceScale;
  if (polar[0])
     distanceScale = 12/polar[0];
  else
     distanceScale = 12;


  float Yellow[3] = {1,1,0};
  glColor3fv(Yellow);
  glPushMatrix();
  glBegin(GL_TRIANGLE_STRIP);

  //Override to different Vertices for different classes?
  glVertex2i(x-(radius*xScale*distanceScale),y-(radius*yScale*distanceScale));
  glVertex2i(x-(radius*xScale*distanceScale),y+(radius*yScale*distanceScale));
  glVertex2i(x,y);
  glVertex2i(x+(radius*xScale*distanceScale),y-(radius*yScale*distanceScale));
  glVertex2i(x+(radius*xScale*distanceScale),y+(radius*yScale*distanceScale));



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
  ggprint8b(&r, 16, 0x00ffff00, "%.1i",currentHealth);


}
void Base::drawBullet(Game * g, Global gl) {

	if (type == 1) {
   		float e[3];
 	   	e[0] = 0;
 	   	e[1] = polar[1];
 	   	e[2] = polar[2];

   	 	if (e[1] > 360) {
   	 	    e[1] -= 360;
   	 	}
   	 	if (e[1] < 0) {
   	 	    e[1] += 360;
   	 	}

 	   	float s[2];
 	   	s[0] = (*g).ship.angle[0];
 	   	s[1] = (*g).ship.angle[1];
 	   	float low, high;
 	  	low = s[0] - 60;
 	   	high = s[0] + 60;
 	   	high -= low;
 	   	e[1] -= low;
 	   	if (e[1] > 360) {
 	       	e[1] = e[1] - 360;
 	   	}
 	   	float x, y;

 	   	x = ((high - e[1])/120)*gl.xres;
 	   	y = ((s[1] + 45 - e[2])/90)*gl.yres;

   	 	float distanceScale = 48/polar[0];
   	 	//glColor3fv(color);
   	 	//float Green[3] = {0,1,0};
 	   	glColor3fv(color);
 	   	glPushMatrix();
 	   	glBegin(GL_POLYGON);

 	   	glVertex2i(x-radius*distanceScale,y);
 	   	glVertex2i(x,y+radius*distanceScale);
 	   	glVertex2i(x+radius*distanceScale,y);
 	   	glVertex2i(x,y-radius*distanceScale);

 	   	glEnd();
 	   	glPopMatrix();
	}
	if (type == 3) {
   		float e[3];
 	   	e[0] = 0;
 	   	e[1] = polar[1];
 	   	e[2] = polar[2];

   	 	if (e[1] > 360) {
   	 	    e[1] -= 360;
   	 	}
   	 	if (e[1] < 0) {
   	 	    e[1] += 360;
   	 	}

 	   	float s[2];
 	   	s[0] = (*g).ship.angle[0];
 	   	s[1] = (*g).ship.angle[1];
 	   	float low, high;
 	  	low = s[0] - 60;
 	   	high = s[0] + 60;
 	   	high -= low;
 	   	e[1] -= low;
 	   	if (e[1] > 360) {
 	       	e[1] = e[1] - 360;
 	   	}
 	   	float x, y;

 	   	x = ((high - e[1])/120)*gl.xres;
 	   	y = ((s[1] + 45 - e[2])/90)*gl.yres;

   	 	float distanceScale = 48/polar[0];
   	 	//glColor3fv(color);
   	 	//float Green[3] = {0,1,0};

		float capsule[3] = {0,1,0};
		glColor3fv(capsule);
 	   	glPushMatrix();
 	    glBegin(GL_LINE_LOOP);
 		for (int i = 0; i < 6; i++) {
	 	   	glVertex2i(x + cos((i*2*PI)/6)*radius*2*distanceScale,y + sin((i*2*PI)/6)*radius*2*distanceScale);
		}
 	   	glEnd();
 	   	glPopMatrix();
 	   	glPushMatrix();
 	    glBegin(GL_LINE_LOOP);
	 	glVertex2i(x + cos((1*2*PI)/6)*radius*2*distanceScale,y + sin((1*2*PI)/6)*radius*2*distanceScale);
	 	glVertex2i(x + cos((2*2*PI)/6)*radius*2*distanceScale,y + sin((2*2*PI)/6)*radius*2*distanceScale);
	 	glVertex2i(x, y + radius*3*distanceScale);
 	   	glEnd();
 	   	glPopMatrix();

 	   	glPushMatrix();
 	    glBegin(GL_LINE_LOOP);
	 	glVertex2i(x + cos((3*2*PI)/6)*radius*2*distanceScale,y + sin((3*2*PI)/6)*radius*2*distanceScale);
	 	glVertex2i(x + cos((4*2*PI)/6)*radius*2*distanceScale,y + sin((4*2*PI)/6)*radius*2*distanceScale);
	 	glVertex2i(x + cos((7*PI)/6)*radius*3*distanceScale,y + sin((7*PI)/6)*radius*3*distanceScale);
 	   	glEnd();
 	   	glPopMatrix();

 	   	glPushMatrix();
 	    glBegin(GL_LINE_LOOP);
	 	glVertex2i(x + cos((5*2*PI)/6)*radius*2*distanceScale,y + sin((5*2*PI)/6)*radius*2*distanceScale);
	 	glVertex2i(x + cos((6*2*PI)/6)*radius*2*distanceScale,y + sin((6*2*PI)/6)*radius*2*distanceScale);
	 	glVertex2i(x + cos((11*PI)/6)*radius*3*distanceScale,y + sin((11*PI)/6)*radius*3*distanceScale);
		glEnd();
 	   	glPopMatrix();

		float flame[3] = {1,.6,0};
 	   	glColor3fv(flame);
 	   	glPushMatrix();
 	    glBegin(GL_POLYGON);
 		for (int i = 0; i < 16; i++) {
	 	   	glVertex2i(x + cos(i*PI/8)*radius*distanceScale,y + sin(i*PI/8)*radius*distanceScale);
		}
 	   	glEnd();
 	   	glPopMatrix();

	}
}

void Enemy::targeting(Game * g, Global gl) {


	if (angle[0] < 0.0f) {
		angle[0] += 360.0f;
  }
		if (angle[0] > 360.0f) {
			angle[0] -= 360.0f;
	}
		if (angle[1] < 0.1f) {
			angle[1] = 0.1f;
	}
		if (angle[1] > 179.9f) {
			angle[1] = 179.9f;
	}

// change it based on the ships position not its angle
	angle[0] = polar[1] + 180;
	angle[1] = 180 - polar[2];

  struct timespec bt;
   clock_gettime(CLOCK_REALTIME, &bt);
   double ts = timeDiff(&bulletTimer, &bt);
   if (ts > 0.5) {
       timeCopy(&bulletTimer, &bt);
       if ((*g).nbullets < MAX_ARRAY) {
           //shoot a bullet...
           //Bullet *b = new Bullet;
           Bullet *b = &(*g).barr[(*g).nbullets];
           timeCopy(&b->time, &bt);
           b->pos[0] = pos[0];
           b->pos[1] = pos[1];
           b->pos[2] = pos[2];
           //b->vel = (*g).ship.vel + 25;
           b->vel = (*g).ship.vel + 25;
           //convert ship angle to radians
           b->angle[0] = angle[0];
           b->angle[1] = angle[1];
           b->color[0] = 0.0f;
           b->color[1] = 1.0f;
           b->color[2] = 0.0f;
           b->enemyBullet = true;
           (*g).nbullets++;
       }



}

}


Image::Image(const char *fname) {
  if (fname[0] == '\0')
    return;
  //printf("fname **%s**\n", fname);
  int ppmFlag = 0;
  char name[40];
  strcpy(name, fname);
  int slen = strlen(name);
  char ppmname[80];
  if (strncmp(name+(slen-4), ".ppm", 4) == 0)
    ppmFlag = 1;
  if (ppmFlag) {
    strcpy(ppmname, name);
  } else {
    name[slen-4] = '\0';
    //printf("name **%s**\n", name);
    sprintf(ppmname,"%s.ppm", name);
    //printf("ppmname **%s**\n", ppmname);
    char ts[100];
    //system("convert eball.jpg eball.ppm");
    sprintf(ts, "convert %s %s", fname, ppmname);
    system(ts);
  }
  //sprintf(ts, "%s", name);
  FILE *fpi = fopen(ppmname, "r");
  if (fpi) {
    char line[200];
    fgets(line, 200, fpi);
    fgets(line, 200, fpi);
  //skip comments and blank lines
    while (line[0] == '#' || strlen(line) < 2)
      fgets(line, 200, fpi);
      sscanf(line, "%i %i", &width, &height);
      fgets(line, 200, fpi);
      //get pixel data
      int n = width * height * 3;
      data = new unsigned char[n];
      for (int i=0; i<n; i++)
        data[i] = fgetc(fpi);
      fclose(fpi);
  } else {
    printf("ERROR opening image: %s\n",ppmname);
    exit(0);
  }
  if (!ppmFlag)
    unlink(ppmname);
}

Ship::Ship(int x, int y, int z) {
  pos[0] = x;
  pos[1] = y;
  pos[2] = z;

  color[0] = color[1] = color[2] = 1.0;
  color[3] = .1;
  invert = false;

  color[0] = color[1] = color[2] = 1.0;
  maxHealth = maxShield = maxBoost = 100;
  currentHealth = maxHealth;
  currentShield = maxShield;
  boost = maxBoost;
  powerLevel = 1;
  maxBullets = MAX;
  weaponType = 1;
  boost = 100;

  clock_gettime(CLOCK_REALTIME, &bulletTimer);
}

Object::Object(int x, int y, int z) {
  pos[0] = x;
  pos[1] = y;
  pos[2] = z;
} 

Bullet::Bullet()
{
	type = 1;
  radius = 15;
  enemyBullet = false;
}


Asteroid::Asteroid() {
	prev = NULL;
	next = NULL;
  shipClass  = rand()%5;
  maxHealth = 3;
  currentHealth = maxHealth;
}


Game::Game(int xWindowSize, int yWindowSize, const Ship& ship, const Object& object) : ship(ship), object(object)
{
	show_credits = false;
	ahead = NULL;
	barr = new Bullet[MAX_ARRAY];
  earr = new Enemy[MAX_ARRAY];
  sarr = new Squadron[MAX_ARRAY];
	nasteroids = 0;
  nenemies = 0;
	nbullets = 0;
  nsquadrons = 0;
	mouseThrustOn = false;
	mtext = 0;
  difficulty = 1.0;
  level = 1;
  score = 0;
  num_stars = 32000;


  clock_gettime(CLOCK_REALTIME, &difficultyTimer);

}
