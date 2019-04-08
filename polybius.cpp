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
void modify_singleton(Num n);
void createAsteroid(Game *g);


// add png files name and create array based on # of pngs
//Image img("./images/bigfoot.png");
Image img[5] = {
"./images/bigfoot.png",
"./images/luis_3350.png",
"./images/IMG_Adolfo_Valencia.png",
"./images/chris_ramirez.png",
"./images/josephG.png"
};


Global gl(1250, 900);

Game* Game::instance = 0;
Game* Game::getInstance()
{
    if (instance == 0) {
        instance = new Game();
    }
    return instance;
}


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

Game* g = Game::getInstance();
Num n(35);

//==========================================================================
// M A I N
//==========================================================================
int main()
{
	logOpen();
	init_opengl();
	srand(time(NULL));
	clock_gettime(CLOCK_REALTIME, &timePause);
	clock_gettime(CLOCK_REALTIME, &timeStart);
	x11.set_mouse_position(100,100);
	int done=0;
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
		if (!(*g).show_credits) {
			physicsCountdown += timeSpan;
			while (physicsCountdown >= physicsRate) {
				physics();
				physicsCountdown -= physicsRate;
			}
		}
		render();
		x11.swapBuffers();
	}
	cleanup_fonts();
	logClose();
	return 0;
}

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
			struct timespec bt;
			clock_gettime(CLOCK_REALTIME, &bt);
			double ts = timeDiff(&(*g).bulletTimer, &bt);
			if (ts > 0.1) {
				timeCopy(&(*g).bulletTimer, &bt);
				//shoot a bullet...
				if ((*g).nbullets < MAX_BULLETS) {
					Bullet *b = &(*g).barr[(*g).nbullets];
					timeCopy(&b->time, &bt);
					b->pos[0] = (*g).ship.pos[0];
					b->pos[1] = (*g).ship.pos[1];
					b->vel[0] = (*g).ship.vel[0];
					b->vel[1] = (*g).ship.vel[1];
					//convert ship angle to radians
					Flt rad = (((*g).ship.angle+90.0) / 360.0f) * PI * 2.0;
					//convert angle to a vector
					Flt xdir = cos(rad);
					Flt ydir = sin(rad);
					b->pos[0] += xdir*20.0f;
					b->pos[1] += ydir*20.0f;
					b->vel[0] += xdir*6.0f + rnd()*0.1;
					b->vel[1] += ydir*6.0f + rnd()*0.1;
					b->color[0] = 1.0f;
					b->color[1] = 1.0f;
					b->color[2] = 1.0f;
					++(*g).nbullets;
				}
			}
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
}


int check_keys(XEvent *e)
{
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
			modify_singleton(n);
			createAsteroid(g);
			break;
		case XK_Down:
			break;
		case XK_equal:
			break;
		case XK_minus:
			break;
	}
	return 0;
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
	//std::cout << "frag" << std::endl;
}

void physics()
{
	Flt d0,d1,dist;
	//Update ship position
	(*g).ship.pos[0] += (*g).ship.vel[0];
	(*g).ship.pos[1] += (*g).ship.vel[1];
	//Check for collision with window edges
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
	//
	//
	//Update bullet positions
	struct timespec bt;
	clock_gettime(CLOCK_REALTIME, &bt);
	int i=0;
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
		//move the bullet
		b->pos[0] += b->vel[0];
		b->pos[1] += b->vel[1];
		//Check for collision with window edges
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
		i++;
	}
	//
	//Update asteroid positions
	//pathFinding(&a->pos[0], &a->pos[2]
    Asteroid *a = (*g).ahead;
	while (a) {
        a->pos[0] += a->vel[0];
        a->pos[0] += a->vel[1];
        switch(a->shipClass) {
            case 1:
                fighterPF(&a->pos[0],&a->pos[1],(*g).ship.pos[0], (*g).ship.pos[1]);
                break;
            case 2:
                frigatePF(&a->pos[0],&a->pos[1], (*g).ship.pos[0]);
                break;
            case 3:
                squadronPF(&a->pos[0],&a->pos[1],(*g).ship.pos[0], (*g).ship.pos[1]);
                break;
            case 4:
                missilePF(&a->pos[0],&a->pos[1],(*g).ship.pos[0], (*g).ship.pos[1]);
                break;
            case 5:
		        carrierPF(&a->pos[0],&a->pos[1],(*g).ship.pos[0],(*g).ship.pos[1]);
                break;
            default:
                break;
        }
                //Check for collision with window edges
        if (a->pos[0] < -100.0) {
			a->pos[0] += (float)gl.xres+200;
		}
		else if (a->pos[0] > (float)gl.xres+100) {
			a->pos[0] -= (float)gl.xres+200;
		}
		else if (a->pos[1] < -100.0) {
			a->pos[1] += (float)gl.yres+200;
		}
		else if (a->pos[1] > (float)gl.yres+100) {
			a->pos[1] -= (float)gl.yres+200;
		}
		//a->angle += a->rotate;
		a = a->next;
	}
	//
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
					deleteAsteroid(g, a);
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
	//---------------------------------------------------
	//check keys pressed now
	if (gl.keys[XK_Left]) {
		(*g).ship.angle += 4.0;
		if ((*g).ship.angle >= 360.0f)
			(*g).ship.angle -= 360.0f;
	}
	if (gl.keys[XK_Right]) {
		(*g).ship.angle -= 4.0;
		if ((*g).ship.angle < 0.0f)
			(*g).ship.angle += 360.0f;
	}
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
	if (gl.keys[XK_space]) {
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
				b->vel[0] = (*g).ship.vel[0];
				b->vel[1] = (*g).ship.vel[1];
				//convert ship angle to radians
				Flt rad = (((*g).ship.angle+90.0) / 360.0f) * PI * 2.0;
				//convert angle to a vector
				Flt xdir = cos(rad);
				Flt ydir = sin(rad);
				b->pos[0] += xdir*20.0f;
				b->pos[1] += ydir*20.0f;
				b->vel[0] += xdir*6.0f + rnd()*0.1;
				b->vel[1] += ydir*6.0f + rnd()*0.1;
				b->color[0] = 1.0f;
				b->color[1] = 1.0f;
				b->color[2] = 1.0f;
				(*g).nbullets++;
			}
		}
	}
	if ((*g).mouseThrustOn) {
		//should thrust be turned off
		struct timespec mtt;
		clock_gettime(CLOCK_REALTIME, &mtt);
		double tdif = timeDiff(&mtt, &(*g).mouseThrustTimer);
		//std::cout << "tdif: " << tdif << std::endl;
		if (tdif < -0.3)
			(*g).mouseThrustOn = false;
	}
}

void render()
{
	glClear(GL_COLOR_BUFFER_BIT);
	if (!(*g).show_credits) {
		Rect r;
		//
		r.bot = gl.yres - 20;
		r.left = 10;
		r.center = 0;
		ggprint8b(&r, 16, 0x00ff0000, "3350 - Asteroids");
		ggprint8b(&r, 16, 0x00ffff00, "n bullets: %i", (*g).nbullets);
		ggprint8b(&r, 16, 0x00ffff00, "n asteroids: %i", (*g).nasteroids);
		//-------------------------------------------------------------------------
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
		}
		//-------------------------------------------------------------------------
		//Draw the asteroids
		{
			Asteroid *a = (*g).ahead;
			while (a) {
				//Log("draw asteroid...\n");
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
		//Draw the bullets
		for (int i=0; i<(*g).nbullets; i++) {
			Bullet *b = &(*g).barr[i];
			//Log("draw bullet...\n");
			glColor3f(1.0, 1.0, 1.0);
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
