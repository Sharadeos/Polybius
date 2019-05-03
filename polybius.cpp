//
//program: asteroids.cpp
//author:  Gordon Griesel
//date:    2014 - 2018
//mod spring 2015: added constructors
//This program is a game starting point for a 3350 project.
//
//
#include "classes.h"

//function prototypes
void init_opengl(void);
void check_mouse(XEvent *e);
int check_keys(XEvent *e);
void physics();
void render();

// add prototypes of all external functions

void AdolfoValenciaPicture(int x, int y, GLuint textid);
void andrewH(int x, int y, GLuint textid, float i);
void creditsLuis(int x, int y, GLuint textid);
void showChrisRamirez(int x, int y, GLuint textid);
void josephG(int x, int y, GLuint textid);
void fighterPF(float* a, float* b, int x, int y);
void frigatePF(float* a, float* b, int x);
void squadronPF(float* a, float* b, int x, int y);
void missilePF(float* a, float* b, int x, int y);
void carrierPF(float* a, float* b, int x, int y);
//luis extern functions
void createAsteroid(Game *g, Global gl);
void createBullet(Game *g, Global gl, Object object);
void difficultyScaling(Game *g, Global gl);

// add png files name and create array based on # of pngs
//Image img("./images/bigfoot.png");
Image img[6] = {
"./images/bigfoot.png",
"./images/luis_3350.png",
"./images/IMG_Adolfo_Valencia.png",
"./images/chris_ramirez.png",
"./images/josephG.png",
"./images/blackhole.jpg"//macros
#define rnd() (((Flt)rand())/(Flt)RAND_MAX)
#define random(a) (rand()%a)
#define VecZero(v) (v)[0]=0.0,(v)[1]=0.0,(v)[2]=0.0
#define MakeVector(x, y, z, v) (v)[0]=(x),(v)[1]=(y),(v)[2]=(z)
#define VecCopy(a,b) (b)[0]=(a)[0];(b)[1]=(a)[1];(b)[2]=(a)[2]
#define VecDot(a,b)	((a)[0]*(b)[0]+(a)[1]*(b)[1]+(a)[2]*(b)[2])
#define VecSub(a,b,c) (c)[0]=(a)[0]-(b)[0]; \
						(c)[1]=(a)[1]-(b)[1]; \
						(c)[2]=(a)[2]-(b)[2]
};

//
Global gl;
//X Windows variables
class X11_wrapper {
private:
	Display *dpy;
	Window win;
	GLXContext glc;
public:
	X11_wrapper() { }
	X11_wrapper(int w, int h) {
		GLint att[] = { GLX_RGBA, GLX_DEPTH_SIZE, 24, GLX_DOUBLEBUFFER, None };
		//GLint att[] = { GLX_RGBA, GLX_DEPTH_SIZE, 24, None };
		XSetWindowAttributes swa;
		setup_screen_res(gl.xres, gl.yres);
		dpy = XOpenDisplay(NULL);
		if (dpy == NULL) {
			std::cout << "\n\tcannot connect to X server" << std::endl;
			exit(EXIT_FAILURE);
		}
		Window root = DefaultRootWindow(dpy);
		XWindowAttributes getWinAttr;
		XGetWindowAttributes(dpy, root, &getWinAttr);
		int fullscreen=0;
		gl.xres = w;
		gl.yres = h;
		if (!w && !h) {
			//Go to fullscreen.
			gl.xres = getWinAttr.width;
			gl.yres = getWinAttr.height;
			//When window is fullscreen, there is no client window
			//so keystrokes are linked to the root window.
			XGrabKeyboard(dpy, root, False,
				GrabModeAsync, GrabModeAsync, CurrentTime);
			fullscreen=1;
		}
		XVisualInfo *vi = glXChooseVisual(dpy, 0, att);
		if (vi == NULL) {
			std::cout << "\n\tno appropriate visual found\n" << std::endl;
			exit(EXIT_FAILURE);
		}
		Colormap cmap = XCreateColormap(dpy, root, vi->visual, AllocNone);
		swa.colormap = cmap;
		swa.event_mask = ExposureMask | KeyPressMask | KeyReleaseMask |
			PointerMotionMask | MotionNotify | ButtonPress | ButtonRelease |
			StructureNotifyMask | SubstructureNotifyMask;
		unsigned int winops = CWBorderPixel|CWColormap|CWEventMask;
		if (fullscreen) {
			winops |= CWOverrideRedirect;
			swa.override_redirect = True;
		}
		win = XCreateWindow(dpy, root, 0, 0, gl.xres, gl.yres, 0,
			vi->depth, InputOutput, vi->visual, winops, &swa);
		//win = XCreateWindow(dpy, root, 0, 0, gl.xres, gl.yres, 0,
		//vi->depth, InputOutput, vi->visual, CWColormap | CWEventMask, &swa);
		set_title();
		glc = glXCreateContext(dpy, vi, NULL, GL_TRUE);
		glXMakeCurrent(dpy, win, glc);
		show_mouse_cursor(0);
	}
	~X11_wrapper() {
		XDestroyWindow(dpy, win);
		XCloseDisplay(dpy);
	}
	void set_title() {
		//Set the window title bar.
		XMapWindow(dpy, win);
		XStoreName(dpy, win, "Asteroids template");
	}
	void check_resize(XEvent *e) {
		//The ConfigureNotify is sent by the
		//server if the window is resized.
		if (e->type != ConfigureNotify)
			return;
		XConfigureEvent xce = e->xconfigure;
		if (xce.width != gl.xres || xce.height != gl.yres) {
			//Window size did change.
			reshape_window(xce.width, xce.height);
		}
	}
	void reshape_window(int width, int height) {
		//window has been resized.
		setup_screen_res(width, height);
		glViewport(0, 0, (GLint)width, (GLint)height);
		glMatrixMode(GL_PROJECTION); glLoadIdentity();
		glMatrixMode(GL_MODELVIEW); glLoadIdentity();
		glOrtho(0, gl.xres, 0, gl.yres, -1, 1);
		set_title();
	}

	void setup_screen_res(const int w, const int h) {
		gl.xres = w;
		gl.yres = h;
	}
	void swapBuffers() {
		glXSwapBuffers(dpy, win);
	}
	bool getXPending() {
		return XPending(dpy);
	}
	XEvent getXNextEvent() {
		XEvent e;
		XNextEvent(dpy, &e);
		return e;
	}
	void set_mouse_position(int x, int y) {
		XWarpPointer(dpy, None, win, 0, 0, 0, 0, x, y);
	}
	void show_mouse_cursor(const int onoff) {
		if (onoff) {
			//this removes our own blank cursor.
			XUndefineCursor(dpy, win);
			return;
		}
		//vars to make blank cursor
		Pixmap blank;
		XColor dummy;
		char data[1] = {0};
		Cursor cursor;
		//make a blank cursor
		blank = XCreateBitmapFromData (dpy, win, data, 1, 1);
		if (blank == None)
			std::cout << "error: out of memory." << std::endl;
		cursor = XCreatePixmapCursor(dpy, blank, blank, &dummy, &dummy, 0, 0);
		XFreePixmap(dpy, blank);
		//this makes you the cursor. then set it using this function
		XDefineCursor(dpy, win, cursor);
		//after you do not need the cursor anymore use this function.
		//it will undo the last change done by XDefineCursor
		//(thus do only use ONCE XDefineCursor and then XUndefineCursor):
	}
} x11(0, 0);
Game* Game::instance = 0;
Game* Game::getInstance()
{
    if (instance == 0) {
        instance = new Game(gl.xres, gl.yres, Ship(0, 0, 0));
    }
    return instance;
}
Game* g = Game::getInstance();


float xScale = 0.0;
float yScale = 0.0;
float testValue = 0.0;


Object object(0, 100, 0);

//==========================================================================
// M A I N
//==========================================================================
int main()
{
	system("xset r off");
	logOpen();
	init_opengl();
	srand(time(NULL));
	clock_gettime(CLOCK_REALTIME, &timePause);
	clock_gettime(CLOCK_REALTIME, &timeStart);
	x11.set_mouse_position(100,100);
	int done=0;

// MOVE
	for (int i = 0; i < (*g).num_stars; i++) {
			(*g).stars[i][0] = (rand() % 359999)*.001; // maps to degrees
			(*g).stars[i][1] = (rand() % 179999)*.001;
			//(*g).stars[i][0] = i; // maps to degrees
			//(*g).stars[i][1] = i;
	}
// MOVE

	while (!done) {
		while (x11.getXPending()) {
			XEvent e = x11.getXNextEvent();
			x11.check_resize(&e);
			check_mouse(&e);
			done = check_keys(&e);
		}
		clock_gettime(CLOCK_REALTIME, &timeCurrent);
		timeSpan = timeDiff(&timeStart, &timeCurrent);
		timeCopy(&timeStart, &timeCurrent);


		//functions before render will not render on the setup_screen_res

		if (!(*g).show_credits) {
			physicsCountdown += timeSpan;
			while (physicsCountdown >= physicsRate) {
				physics();
				physicsCountdown -= physicsRate;
			}
			//difficultyScaling(g, gl);
		}

		render();
		x11.swapBuffers();
	}
	cleanup_fonts();
	logClose();
	return 0;
}
// in charge of initalizing image textures
void init_opengl(void)
{
	//OpenGL initialization
	glViewport(0, 0, gl.xres, gl.yres);
	//Initialize matrices
	glMatrixMode(GL_PROJECTION); glLoadIdentity();
	glMatrixMode(GL_MODELVIEW); glLoadIdentity();
	//This sets 2D mode (no perspective)
	glOrtho(0, gl.xres, 0, gl.yres, -1, 1);
	//
	glDisable(GL_LIGHTING);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_FOG);
	glDisable(GL_CULL_FACE);
	//
	//Clear the screen to black
	glClearColor(0.0, 0.0, 0.0, 1.0);
	//Do this to allow fonts
	glEnable(GL_TEXTURE_2D);
	initialize_fonts();

	glGenTextures(1, &gl.bigfootTexture);
	// Generate texture for each texture id
	int w = img[0].width;
	int h = img[0].height;

	// Bind texture for each texture id
	glBindTexture(GL_TEXTURE_2D, gl.bigfootTexture);
        //
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
  glTexImage2D(GL_TEXTURE_2D, 0, 3, w, h, 0,
		GL_RGB, GL_UNSIGNED_BYTE, img[0].data);

	glGenTextures(1, &gl.luisTexture);
	// Generate texture for each texture id
	w = img[1].width;
	h = img[1].height;

	// Bind texture for each texture id
	glBindTexture(GL_TEXTURE_2D, gl.luisTexture);
        //
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
  glTexImage2D(GL_TEXTURE_2D, 0, 3, w, h, 0,
		GL_RGB, GL_UNSIGNED_BYTE, img[1].data);

	glGenTextures(1, &gl.AdolfoTexture);

	w=img[2].width;
  h=img[2].height;
  glBindTexture (GL_TEXTURE_2D, gl.AdolfoTexture);
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
  glTexImage2D(GL_TEXTURE_2D, 0, 3, w, h, 0,
     GL_RGB, GL_UNSIGNED_BYTE, img[2].data);

  glGenTextures(1, &gl.chrisTexture);
  w = img[3].width;
  h = img[3].width;
  glBindTexture(GL_TEXTURE_2D, gl.chrisTexture);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexImage2D(GL_TEXTURE_2D, 0, 3, w, h, 0,
     GL_RGB, GL_UNSIGNED_BYTE, img[3].data);

  glGenTextures(1, &gl.josephTexture);
  w = img[4].width;
  h = img[4].width;
  glBindTexture(GL_TEXTURE_2D, gl.josephTexture);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexImage2D(GL_TEXTURE_2D, 0, 3, w, h, 0,
     GL_RGB, GL_UNSIGNED_BYTE, img[4].data);

 	glGenTextures(1, &gl.blackholeTexture);
  w = img[5].width;
  h = img[5].width;
  glBindTexture(GL_TEXTURE_2D, gl.blackholeTexture);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexImage2D(GL_TEXTURE_2D, 0, 3, w, h, 0,
    GL_RGB, GL_UNSIGNED_BYTE, img[5].data);



}

void normalize2d(Vec v)
{
	Flt len = v[0]*v[0] + v[1]*v[1];
	if (len == 0.0f) {
		v[0] = 1.0;
		v[1] = 0.0;
		return;
	}
	len = 1.0f / sqrt(len);
	v[0] *= len;
	v[1] *= len;
}

void check_mouse(XEvent *e)
{
	/*
	//Did the mouse move?
	//Was a mouse button clicked?
	static int savex = 0;
	static int savey = 0;
	//
	static int ct=0;
	//std::cout << "m" << std::endl << std::flush;
	if (e->type == ButtonRelease) {
		return;
	}
	if (e->type == ButtonPress) {
		if (e->xbutton.button==1) {
			//Left button is down
			//a little time between each bullet
					//createBullet(g, gl, (*g).ship);


		}
		if (e->xbutton.button==3) {
			//Right button is down
		}
	}
	//keys[XK_Up] = 0;
	if (savex != e->xbutton.x || savey != e->xbutton.y) {
		//Mouse moved
		int xdiff = savex - e->xbutton.x;
		int ydiff = savey - e->xbutton.y;
		if (++ct < 10)
			return;
		//std::cout << "savex: " << savex << std::endl << std::flush;
		//std::cout << "e->xbutton.x: " << e->xbutton.x << std::endl <<
		//std::flush;
		if (xdiff > 0) {
			//std::cout << "xdiff: " << xdiff << std::endl << std::flush;
			(*g).ship.angle += 0.05f * (float)xdiff;
			if ((*g).ship.angle >= 360.0f)
				(*g).ship.angle -= 360.0f;
		}
		else if (xdiff < 0) {
			//std::cout << "xdiff: " << xdiff << std::endl << std::flush;
			(*g).ship.angle += 0.05f * (float)xdiff;
			if ((*g).ship.angle < 0.0f)
				(*g).ship.angle += 360.0f;
		}
		if (ydiff > 0) {
			//apply thrust
			//convert ship angle to radians
			Flt rad = (((*g).ship.angle+90.0) / 360.0f) * PI * 2.0;
			//convert angle to a vector
			Flt xdir = cos(rad);
			Flt ydir = sin(rad);
			(*g).ship.vel[0] += xdir * (float)ydiff * 0.01f;
			(*g).ship.vel[1] += ydir * (float)ydiff * 0.01f;
            Flt speed = sqrt((*g).ship.vel[0]*(*g).ship.vel[0]+
					(*g).ship.vel[1]*(*g).ship.vel[1]);
			if (speed > 10.0f) {
				speed = 10.0f;
				normalize2d((*g).ship.vel);
				(*g).ship.vel[0] *= speed;
				(*g).ship.vel[1] *= speed;
			}
			(*g).mouseThrustOn = true;
			clock_gettime(CLOCK_REALTIME, &(*g).mouseThrustTimer);
		}
		x11.set_mouse_position(100,100);
		savex=100;
		savey=100;
	}
	*/
}


int check_keys(XEvent *e)
{
	int key = XLookupKeysym(&e->xkey, 0);
	if (e->type == KeyRelease) {
		gl.keyhits[key%KEYS] = 0;
	}
	if (e->type == KeyPress) {
		//std::cout << "press" << std::endl;
		gl.keyhits[key%KEYS] = 1;
		if (key == XK_Escape)
			return 1;
	}
	return 0;

	/*
	//keyboard input?
	static int shift=0;
	int key = (XLookupKeysym(&e->xkey, 0) & 0x0000ffff);
	//Log("key: %i\n", key);
	if (e->type == KeyRelease) {
		gl.keys[key]=0;
		if (key == XK_Shift_L || key == XK_Shift_R)
			shift=0;
		return 0;
	}
	if (e->type == KeyPress) {
		//std::cout << "press" << std::endl;
		gl.keys[key]=1;
		if (key == XK_Shift_L || key == XK_Shift_R) {
			shift=1;
			return 0;
		}
	} else {
		return 0;
	}
	if (shift){}
	switch (key) {
		case XK_Escape:
			return 1;
		case XK_c:
			(*g).show_credits = !(*g).show_credits;
			break;
		case XK_q:
			createAsteroid(g, gl);
			break;
		case XK_e:
			(*g).ship.powerLevel++;
			break;
		case XK_Down:
			break;
		case XK_equal:
			break;
		case XK_minus:
			break;
	}
	return 0;
	*/
}

void deleteAsteroid(Game *g, Asteroid *node)
{
	//Remove a node from doubly-linked list
	//Must look at 4 special cases below.
	if (node->prev == NULL) {
		if (node->next == NULL) {
			//only 1 item in list.
			g->ahead = NULL;
		} else {
			//at beginning of list.
			node->next->prev = NULL;
			g->ahead = node->next;
		}
	} else {
		if (node->next == NULL) {
			//at end of list.
			node->prev->next = NULL;
		} else {
			//in middle of list.
			node->prev->next = node->next;
			node->next->prev = node->prev;
		}
	}
	delete node;
	node = NULL;
}
/*
void buildAsteroidFragment(Asteroid *ta, Asteroid *a)
{
	//build ta from a
	ta->nverts = 8;
	ta->radius = a->radius / 2.0;
	Flt r2 = ta->radius / 2.0;
	Flt angle = 0.0f;
	Flt inc = (PI * 2.0) / (Flt)ta->nverts;
	for (int i=0; i<ta->nverts; i++) {
		ta->vert[i][0] = sin(angle) * (r2 + rnd() * ta->radius);
		ta->vert[i][1] = cos(angle) * (r2 + rnd() * ta->radius);
		angle += inc;
	}
	ta->pos[0] = a->pos[0] + rnd()*10.0-5.0;
	ta->pos[1] = a->pos[1] + rnd()*10.0-5.0;
	ta->pos[2] = 0.0f;
	ta->angle = 0.0;
	ta->rotate = a->rotate + (rnd() * 4.0 - 2.0);
	ta->color[0] = 0.8;
	ta->color[1] = 0.8;
	ta->color[2] = 0.7;
	ta->vel[0] = a->vel[0] + (rnd()*2.0-1.0);
	ta->vel[1] = a->vel[1] + (rnd()*2.0-1.0);
	ta->currentHealth = a->maxHealth;
	//std::cout << "frag" << std::endl;
}
*/
void physics()
{
	//Update ship position
	float a_xy, a_z;
	a_xy = (*g).ship.angle[0];
	a_xy *= PI/180;
	a_z = (*g).ship.angle[1];
	a_z *= PI/180;
	(*g).ship.pos[0] += (*g).ship.vel*cos(a_xy)*sin(a_z);
	(*g).ship.pos[1] += (*g).ship.vel*sin(a_xy)*sin(a_z);
	(*g).ship.pos[2] += (*g).ship.vel*cos(a_z);

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
    object.updatePolar((*g).ship.pos);
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
	// q

	if (gl.keyhits[13]) {
		(*g).ship.vel -= .1;
		if ((*g).ship.vel <= 0) {
		    (*g).ship.vel = 0;
		}
		//if ((*g).ship.angle >= 360.0f)
		//    (*g).ship.angle -= 360.0f;
	}
	// e
	if (gl.keyhits[1]) {
		(*g).ship.vel += .1;
		if ((*g).ship.vel >= MAX_THRUST) {
		    (*g).ship.vel = MAX_THRUST;
		}
		//if ((*g).ship.angle < 0.0f)
		//(*g).ship.angle += 360.0f;
	}const int MAX_BULLETS = 11;
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
			if ((*g).nbullets < MAX_BULLETS) {
				//shoot a bullet...
				//Bullet *b = new Bullet;
				Bullet *b = &(*g).barr[(*g).nbullets];
				timeCopy(&b->time, &bt);
				b->pos[0] = (*g).ship.pos[0];
				b->pos[1] = (*g).ship.pos[1];
				b->pos[2] = (*g).ship.pos[2];
				//b->vel = (*g).ship.vel + 25;
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


void drawObject(Object rend_object)
{

	int sizeofObject = 50;
	float e[3];
	e[0] = 0;
	e[1] = rend_object.polar[1];
	e[2] = rend_object.polar[2];

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
        e[1] = e[1] - 360;
    }
    float x, y;

		x = ((high - e[1])/120)*gl.xres;
  	y = ((s[1] + 45 - e[2])/90)*gl.yres;



		//Scale max at the right edge of the setup_screen

		xScale = ((high - e[1])/60);
		yScale = ((s[1] + 45 - e[2])/45);

		if (xScale > 1.0) {
			testValue = xScale - 1.0;
			xScale = 1.0;
			xScale = xScale - testValue;
		}

		if (yScale > 1.0) {
			testValue = yScale - 1.0;
			yScale = 1.0;
			yScale = yScale - testValue;
		}

		float distanceScale = 12/rend_object.polar[0];


    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		glBegin(GL_POLYGON);
			/*
			 glVertex2i(50,100);
			 glVertex2i(100,100);
			 glVertex2i(100,150);
			 glVertex2i(50,150);
			 */
			 glVertex2i(x-(40*xScale*distanceScale),y-(40*yScale*distanceScale));
			 glVertex2i(x-(40*xScale*distanceScale),y+(40*yScale*distanceScale));
			 glVertex2i(x+(40*xScale*distanceScale),y-(40*yScale*distanceScale));
			 glVertex2i(x+(40*xScale*distanceScale),y+(40*yScale*distanceScale));

			 //glVertex2f(50, 100);
			 //glVertex2i(100,100);
			 //glVertex2i(100,150);
			 //glVertex2i(50,150);
		glEnd();

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);






		glColor3f(1.0f, 0.0f, 0.0f);
		glBegin(GL_POINTS);
		//glVertex2f(object.drawSize[0], object.drawSize[1]);
		glEnd();
}
void drawBullet(Bullet rend_object, float xScale, float yScale)
{

	int sizeofObject = 50;
	float e[3];
	e[0] = 0;
	e[1] = rend_object.polar[1];
	e[2] = rend_object.polar[2];

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
      e[1] = e[1] - 360;
  }
  float x, y;

	x = ((high - e[1])/120)*gl.xres;
	y = ((s[1] + 45 - e[2])/90)*gl.yres;

  glColor3fv(rend_object.color);
  glPushMatrix();
  glBegin(GL_POLYGON);
	glVertex2i(x-5,y);
	glVertex2i(x,y+5);
	glVertex2i(x+5,y);
	glVertex2i(x,y-5);
	glEnd();
  glPopMatrix();
/*
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		glBegin(GL_POLYGON);
			 glVertex2i(50,100);
			 glVertex2i(100,100);
			 glVertex2i(100,150);
			 glVertex2i(50,150);
			 glVertex2i(x+rend_object.drawSize[0]-sizeofObject*xScale,y+rend_object.drawSize[1]+sizeofObject*yScale);
			 glVertex2i(x+rend_object.drawSize[0]+sizeofObject*xScale,y+rend_object.drawSize[1]+sizeofObject*yScale);
			 glVertex2i(x+rend_object.drawSize[0]+sizeofObject*xScale,y+rend_object.drawSize[1]-sizeofObject*yScale);
			 glVertex2i(x+rend_object.drawSize[0]-sizeofObject*xScale,y+rend_object.drawSize[1]-sizeofObject*yScale);

			 //glVertex2f(50, 100);
			 //glVertex2i(100,100);
			 //glVertex2i(100,150);
			 //glVertex2i(50,150);
		glEnd();

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glColor3f(1.0f, 0.0f, 0.0f);
		glBegin(GL_POINTS);
		glVertex2f(object.drawSize[0], object.drawSize[1]);
		glEnd();
*/
}



void render()
{

	glClear(GL_COLOR_BUFFER_BIT);
	if (!(*g).show_credits) {

		drawObject(object);


		for (int i=0; i< (*g).nbullets; i++) {
			Bullet *b = & (*g).barr[i];
		    drawBullet( (*g).barr[i], 1, 1);
	    }


				//-------------------------------------------------------------------------
			    /*
			    //Draw the ship
				glColor3fv((*g).ship.color);
				glPushMatrix();
				glTranslatef((*g).ship.pos[0], (*g).ship.pos[1], (*g).ship.pos[2]);
				//float angle = atan2(ship.dir[1], ship.dir[0]);
				glRotatef((*g).ship.angle, 0.0f, 0.0f, 1.0f);
				glBegin(GL_TRIANGLES);
				//glVertex2f(-10.0f, -10.0f);
				//glVertex2f(  0.0f, 20.0f);
				//glVertex2f( 10.0f, -10.0f);
				glVertex2f(-12.0f, -10.0f);
				glVertex2f(  0.0f, 20.0f);
				glVertex2f(  0.0f, -6.0f);
				glVertex2f(  0.0f, -6.0f);
				glVertex2f(  0.0f, 20.0f);
				glVertex2f( 12.0f, -10.0f);
				glEnd();
				glColor3f(1.0f, 0.0f, 0.0f);
				glBegin(GL_POINTS);
				glVertex2f(0.0f, 0.0f);
				glEnd();
				glPopMatrix();
				if (gl.keys[XK_Up] || (*g).mouseThrustOn) {
					int i;
					//draw thrust
					Flt rad = (((*g).ship.angle+90.0) / 360.0f) * PI * 2.0;
					//convert angle to a vector
					Flt xdir = cos(rad);
					Flt ydir = sin(rad);
					Flt xs,ys,xe,ye,r;
					glBegin(GL_LINES);
					for (i=0; i<16; i++) {
						xs = -xdir * 11.0f + rnd() * 4.0 - 2.0;
						ys = -ydir * 11.0f + rnd() * 4.0 - 2.0;
						r = rnd()*40.0+40.0;
						xe = -xdir * r + rnd() * 18.0 - 9.0;
						ye = -ydir * r + rnd() * 18.0 - 9.0;
						glColor3f(rnd()*.3+.7, rnd()*.3+.7, 0);
						glVertex2f((*g).ship.pos[0]+xs,(*g).ship.pos[1]+ys);
						glVertex2f((*g).ship.pos[0]+xe,(*g).ship.pos[1]+ye);
					}
					glEnd();
				}*/
				//-------------------------------------------------------------------------
				//Draw the asteroids
				{
					Asteroid *a =  (*g).ahead;
					while (a) {
						//Loig("draw asteroid...\n");
						glColor3fv(a->color);
						glPushMatrix();
						glTranslatef(a->pos[0], a->pos[1], a->pos[2]);
						glRotatef(a->angle, 0.0f, 0.0f, 1.0f);
						glBegin(GL_LINE_LOOP);
						//Log("%i verts\n",a->nverts);
						for (int j=0; j<a->nverts; j++) {
							glVertex2f(a->vert[j][0], a->vert[j][1]);
						}
						glEnd();
						//glBegin(GL_LINES);
						//	glVertex2f(0,   0);
						//	glVertex2f(a->radius, 0);
						//glEnd();
						glPopMatrix();
						glColor3f(1.0f, 0.0f, 0.0f);
						glBegin(GL_POINTS);
						glVertex2f(a->pos[0], a->pos[1]);
						glEnd();
						a = a->next;
					}
				}
				//-------------------------------------------------------------------------
				/*Draw the bullets
				for (int i=0; i<(*g).nbullets; i++) {
					Bullet *b = &(*g).barr[i];
					//Log("draw bullet...\n");
					glColor3f(0.0, 1.0, 0.0);
					glBegin(GL_POINTS);
					glVertex2f(b->pos[0],      b->pos[1]);
					glVertex2f(b->pos[0]-1.0f, b->pos[1]);
					glVertex2f(b->pos[0]+1.0f, b->pos[1]);
					glVertex2f(b->pos[0],      b->pos[1]-1.0f);
					glVertex2f(b->pos[0],      b->pos[1]+1.0f);
					glColor3f(0.8, 0.8, 0.8);
					glVertex2f(b->pos[0]-1.0f, b->pos[1]-1.0f);
					glVertex2f(b->pos[0]-1.0f, b->pos[1]+1.0f);
					glVertex2f(b->pos[0]+1.0f, b->pos[1]-1.0f);
					glVertex2f(b->pos[0]+1.0f, b->pos[1]+1.0f);
					glEnd();
				}
			    */
			    //stars
			    float cx = gl.xres/2;
			    float cy = gl.yres/2;
			    glColor3fv((*g).ship.color);
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

			    //crosshair 1
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

			    //Outer Hexagon
			    //glColor3fv((*g).ship.color);
			    //glPushMatrix();
			    //glBegin(GL_LINE_LOOP);

			    for (int i = 0; i < 6; i++) {
			        float theta = ((2.0*PI)/6.0)*i;
			        v8_2[i][0] = cx + rad[3]*cos(theta);
			        v8_2[i][1] = cy + rad[3]*sin(theta);
			        //glVertex2f(v8_2[i][0],v8_2[i][1]);
			    }
				//glEnd();
			    //glPopMatrix();
			    //

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
			    float b = a + MAX_THRUST;
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

			    glColor4fv(vel);
			    glPushMatrix();
			    glBegin(GL_LINE_LOOP);
			    for (int i = 0; i < pts; i++) {
			        float theta = ((2*PI)/pts)*i;
			        float vertx = cx + (b*cos(theta));
			        float verty = cy + (b*sin(theta));
			        glVertex2f(vertx,verty);
			    }
			    glEnd();
				glPopMatrix();
			    float thrust = a + (*g).ship.vel;
			    while (a < thrust) {
			        glColor4fv(vel);
			        glPushMatrix();
			        glBegin(GL_LINE_LOOP);
			        for (int i = 0; i < pts; i++) {
			            float theta = ((2*PI)/pts)*i;
			            float vertx = cx + (4*a*cos(theta));
			            float verty = cy + (4*a*sin(theta));
			            glVertex2f(vertx,verty);
			        }
			        a += .1;
			        glEnd();
				    glPopMatrix();
			    }

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
			    float radar[3]; // center of radar + radius
			    radar[0] = gl.xres*.10;
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
			    /*
			    float black[3];
			    black[0] = 0;
			    black[1] = 0;
			    black[2] = 0;

			    for (int i = 0; i < 4; i++) {
			    	glColor3fv(black);
				glPushMatrix();
			    	glBegin(GL_LINE);
			        glVertex2f(vertices[4][0],vertices[4][1]);
			        glVertex2f(vertices[i][0],vertices[i][1]);
			    	glEnd();
			    	glPopMatrix();
			    }
			    */

				Rect r;
				//
				r.bot = 100;
				r.left = cx-34;
				r.center = 0;
				//ggprint8b(&r, 16, 0x00ff0000, "3350 - Asteroids");
				ggprint8b(&r, 16, 0x00ffff00, "xScale = %.1f, yScale = %.1f, Test Value = %.1f",xScale, yScale, testValue);
				ggprint8b(&r, 16, 0x00ffff00, "n bullets: %i", (*g).nbullets);
				ggprint8b(&r, 16, 0x00ffff00, "%.1f : %.1f ", (*g).ship.angle[0], (*g).ship.angle[1]);
				//ggprint8b(&r, 16, 0x00ffff00, "%.1f:%.1f,roll=%.1f)", (*g).ship.angle[0], (*g).ship.angle[1], (*g).ship.angle[2]);
				//ggprint8b(&r, 16, 0x00ffff00, "(x=%.1f,y=%.1f)", (*g).ship.pos[0], (*g).ship.pos[1]);
				ggprint8b(&r, 16, 0x00ffff00, "(x=%.1f,y=%.1f,z=%.1f)", (*g).ship.pos[0], (*g).ship.pos[1], (*g).ship.pos[2]);

				ggprint8b(&r, 16, 0x00ffff00, "(object x=%.1f,object y=%.1f,object z=%.1f)", object.pos[0], object.pos[1], object.pos[2]);

				//ggprint8b(&r, 16, 0x00ffff00, "(distance x=%.1f)", sqrt((pow((*g).ship.pos[0] - object.pos[0],2)) + (pow((*g).ship.pos[1] - object.pos[1],2))) );

				//ggprint8b(&r, 16, 0x00ffff00, "(object render x=%.1f,object render y=%.1f)", object.polar[1],object.polar[2]);

			  ggprint8b(&r, 16, 0x00ffff00, "distance p0 = %.1f",object.polar[0]);
			  ggprint8b(&r, 16, 0x00ffff00, "p1:xy = %.1f, fov calc = %.1f",object.polar[1], (*g).ship.angle[0] + 60 - object.polar[1]);
			  ggprint8b(&r, 16, 0x00ffff00, "p2:z = %.1f",object.polar[2]);

			  	//ggprint8b(&r, 16, 0x00ffff00, "(object render x=%.1f,object render y=%.1f)", (((*g).ship.angle[0] + 60 - object.polar[1])/120)*gl.xres,  (((*g).ship.angle[1] + 45 - object.polar[2])/90)*gl.yres);
}


	if ((*g).show_credits) {
	    (*g).mtext -= .02;
	    andrewH(.5*gl.xres, .9*gl.yres, gl.bigfootTexture,(*g).mtext);
  	  creditsLuis(.5*gl.xres, .7*gl.yres, gl.luisTexture);
	    AdolfoValenciaPicture(.5*gl.xres, .5*gl.yres, gl.AdolfoTexture);
    	showChrisRamirez(.5*gl.xres, .3*gl.yres, gl.chrisTexture);
	    josephG(.5*gl.xres, .1*gl.yres, gl.josephTexture);
        // function calls for everyone with parameters
	}
}
