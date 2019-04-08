#include "classes.h"


Num::Num(int n) : num(n)
{
}

int Num::getNum()
{
  return num;
}

Global::Global(int x, int y) {
  xres = XRES;
  yres = YRES;
  memset(keys, 0, 65536);
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

Ship::Ship() {
  VecZero(dir);
  pos[0] = (Flt)(XRES/2);
  pos[1] = (Flt)(YRES/2);
  pos[2] = 0.0f;
  VecZero(vel);
  angle = 0.0;
  color[0] = color[1] = color[2] = 1.0;
}

Bullet::Bullet()
{

}

Asteroid::Asteroid() {
		prev = NULL;
		next = NULL;
    shipClass  = rand()%5;
}


Game::Game(){
	show_credits = false;
	ahead = NULL;
	barr = new Bullet[MAX_BULLETS];
	nasteroids = 0;
	nbullets = 0;
	mouseThrustOn = false;
	mtext = 0;
	//build 10 asteroids...
	for (int j=0; j<10; j++) {
		Asteroid *a = new Asteroid;
		a->nverts = 8;
		a->radius = rnd()*80.0 + 40.0;
		Flt r2 = a->radius / 2.0;
		Flt angle = 0.0f;
		Flt inc = (PI * 2.0) / (Flt)a->nverts;
		for (int i=0; i<a->nverts; i++) {
			a->vert[i][0] = sin(angle) * (r2 + rnd() * a->radius);
			a->vert[i][1] = cos(angle) * (r2 + rnd() * a->radius);
			angle += inc;
		}
		a->pos[0] = (Flt)(rand() % XRES);
		a->pos[1] = (Flt)(rand() % YRES);
		a->pos[2] = 0.0f;
		a->angle = 0.0;
		a->rotate = rnd() * 4.0 - 2.0;
		a->color[0] = 0.8;
		a->color[1] = 0.8;
		a->color[2] = 0.7;
		a->vel[0] = (Flt)(rnd()*2.0-1.0);
		a->vel[1] = (Flt)(rnd()*2.0-1.0);
		//std::cout << "asteroid" << std::endl;
		//add to front of linked list
		a->next = ahead;
		if (ahead != NULL)
			ahead->prev = a;
		ahead = a;
		++nasteroids;
	}
	clock_gettime(CLOCK_REALTIME, &bulletTimer);
}
