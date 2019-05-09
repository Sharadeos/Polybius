//
//program: asteroids.cpp
//author:  Gordon Griesel
//date:    2014 - 2018
//mod spring 2015: added constructors
//This program is a game starting point for a 3350 project.
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

//joseph extern functions
void josephG(int x, int y, GLuint textid);
void pathFindingTest2(Game *g,Global gl);
void fighterPF(Game *g, int x);
void frigatePF(Game *g, int x);
void carrierPF(Game*g, int x);
void score(Game *g,int i);
void scoreBoard(Game *g, Global gl);
//luis extern functions
//void createAsteroid(Game *g, Global gl);
//void createBullet(Game *g, Global gl, Object object);
void luisRender(Game *g, Global gl);
void difficultyScaling(Game *g, Global gl);
bool collisionDetection(Base object1, Base object2);
void spawnEnemy(Game *g, Global gl, Vec pos, Bool squad, int enemyType);
void enemyTargeting(Game *g, Global gl);
//chris extern functions
void playTitleMusic();
void pauseTitleMusic();
void playMusic();
void weapon1();
void weapon2();
unsigned char *buildAlphaData(Image *img);
void mainMenuTitle(int x, int y, GLuint textid);
void mainMenuPlay(int x, int y, GLuint textid);
void mainMenuControls(int x, int y, GLuint textid);
void mainMenuCredits(int x, int y, GLuint textid);
void mainMenuExit(int x, int y, GLuint textid);
void stateKeys(Game *g, Global gl);
void checkMenuItem(Game *g, Global gl);
//joey extern functions
void joeyPhysics(Game *g, Global gl);
void joeyStars(Game *g, Global gl);
void joeyRender(Game *g, Global gl);
void credit(Game *g, Global gl);

//Adolfo External Functions
void AdolfoRender(Game *g, Global gl);


// add png files name and create array based on # of pngs
//Image img("./images/bigfoot.png");
Image img[15] = {
"./images/bigfoot.png",
"./images/luis_3350.png",
"./images/IMG_Adolfo_Valencia.png",
"./images/chris_ramirez.png",
"./images/josephG.png",
"./images/blackhole.jpg",
"./images/TITLE.png",
"./images/PLAY.png",
"./images/CONTROLS.png",
"./images/CREDITS.png",
"./images/EXIT2.png",
"./images/PLAYw.png",
"./images/CONTROLSw.png",
"./images/CREDITSw.png",
"./images/EXIT2w.png"
//macros
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
        instance = new Game(gl.xres, gl.yres, Ship(0, 0, 0), Object(0, 100, 0));
    }
    return instance;
}
Game* g = Game::getInstance();




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
	x11.set_mouse_position(0,0);
	int done=0;

// MOVE
#ifdef USE_OPENAL_SOUND
	playTitleMusic();
#endif

	for (int i = 0; i < (*g).num_stars; i++) {
			(*g).stars[i][0] = (rand() % 359999)*.001; // maps to degrees
			(*g).stars[i][1] = (rand() % 179999)*.001;
			(*g).stars[i][2] = ((rand() % 10) + 1)*.1;
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

	render();
		//functions before render will not render on the setup_screen_res
		credit(g,gl);
		stateKeys(g, gl);
		checkMenuItem(g, gl);
		if (!(*g).show_credits) {
			physicsCountdown += timeSpan;
			while (physicsCountdown >= physicsRate) {
				physics();
				physicsCountdown -= physicsRate;
			}

		}


		x11.swapBuffers();
	}
	cleanup_fonts();
	logClose();
	system("xset r on");
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


  //Menu Textures
  glGenTextures(1, &gl.titleTexture);
  w = img[6].width;
  h = img[6].height;
  glBindTexture(GL_TEXTURE_2D, gl.titleTexture);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  unsigned char *titleData = buildAlphaData(&img[6]);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
          GL_RGBA, GL_UNSIGNED_BYTE, titleData);
  free(titleData);

  glGenTextures(1, &gl.playTexture);
  w = img[7].width;
  h = img[7].height;
  glBindTexture(GL_TEXTURE_2D, gl.playTexture);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  unsigned char *playData = buildAlphaData(&img[7]);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
          GL_RGBA, GL_UNSIGNED_BYTE, playData);
  free(playData);

  glGenTextures(1, &gl.controlsTexture);
  w = img[8].width;
  h = img[8].height;
  glBindTexture(GL_TEXTURE_2D, gl.controlsTexture);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  unsigned char *controlsData = buildAlphaData(&img[8]);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
          GL_RGBA, GL_UNSIGNED_BYTE, playData);
  free(controlsData);

  glGenTextures(1, &gl.creditsTexture);
  w = img[9].width;
  h = img[9].height;
  glBindTexture(GL_TEXTURE_2D, gl.creditsTexture);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  unsigned char *creditsData = buildAlphaData(&img[9]);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
          GL_RGBA, GL_UNSIGNED_BYTE, creditsData);
  free(creditsData);

  glGenTextures(1, &gl.exitTexture);
  w = img[10].width;
  h = img[10].height;
  glBindTexture(GL_TEXTURE_2D, gl.exitTexture);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  unsigned char *exitData = buildAlphaData(&img[10]);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
          GL_RGBA, GL_UNSIGNED_BYTE, exitData);
  free(exitData);

  //Menu Items Highlighted
  glGenTextures(1, &gl.playwTexture);
  w = img[11].width;
  h = img[11].height;
  glBindTexture(GL_TEXTURE_2D, gl.playwTexture);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  unsigned char *playwData = buildAlphaData(&img[11]);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
          GL_RGBA, GL_UNSIGNED_BYTE, playwData);
  free(playwData);

  glGenTextures(1, &gl.controlswTexture);
  w = img[12].width;
  h = img[12].height;
  glBindTexture(GL_TEXTURE_2D, gl.controlswTexture);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  unsigned char *controlswData = buildAlphaData(&img[12]);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
          GL_RGBA, GL_UNSIGNED_BYTE, controlswData);
  free(controlswData);

  glGenTextures(1, &gl.creditswTexture);
  w = img[13].width;
  h = img[13].height;
  glBindTexture(GL_TEXTURE_2D, gl.creditswTexture);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  unsigned char *creditswData = buildAlphaData(&img[13]);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
          GL_RGBA, GL_UNSIGNED_BYTE, creditswData);
  free(creditswData);

  glGenTextures(1, &gl.exitwTexture);
  w = img[14].width;
  h = img[14].height;
  glBindTexture(GL_TEXTURE_2D, gl.exitwTexture);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  unsigned char *exitwData = buildAlphaData(&img[14]);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
          GL_RGBA, GL_UNSIGNED_BYTE, exitwData);
  free(exitwData);
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

	static int savex = 0;
	static int savey = 0;
	static int ct=0;
	//std::cout << "m" << std::endl << std::flush;
	if (e->type == ButtonRelease) {
		return;
	}
	if (e->type == ButtonPress) {
		if (e->xbutton.button==1) {
			if ((*g).ship.weaponType == 1) {
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
    		            //b->vel = (*g).ship.vel + 25;
						#ifdef USE_OPENAL_SOUND
							weapon1();
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
			if ((*g).ship.weaponType == 2) {
	    		//a little time between each bullet
	        	struct timespec bt;
    	    	clock_gettime(CLOCK_REALTIME, &bt);
        		double ts = timeDiff(&(*g).bulletTimer, &bt);
        		if (ts > 0.1) {
            		timeCopy(&(*g).bulletTimer, &bt);
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
	    	            //Bullet *b = new Bullet;
    	    	        Bullet *b = &(*g).barr[(*g).nbullets];
        	    	    timeCopy(&b->time, &bt);
            	    	b->pos[0] = (*g).ship.pos[0] + xo;
	                	b->pos[1] = (*g).ship.pos[1] + yo;
		                b->pos[2] = (*g).ship.pos[2];
    		            //b->vel = (*g).ship.vel + 25;
#ifdef USE_OPENAL_SOUND
						weapon2();
#endif
						b->vel = (*g).ship.vel + 25;
            		    //convert ship angle to radians
                		b->angle[0] = (*g).ship.angle[0];
		                b->angle[1] = (*g).ship.angle[1];
    		            b->color[0] = 0.0f;
        		        b->color[1] = 1.0f;
            	    	b->color[2] = 0.0f;
                		(*g).nbullets++;
    	            	//shoot a bullet...
	    	            //Bullet *b = new Bullet;
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
    		            c->color[0] = 0.0f;
        		        c->color[1] = 0.0f;
            	    	c->color[2] = 1.0f;
                		(*g).nbullets++;
            		}
				}
        }
	}
	if (e->xbutton.button==3) {
		//Right button is down
	}

}
	if (savex != e->xbutton.x || savey != e->xbutton.y) {
		//Mouse moved

		float xdiff = savex - e->xbutton.x;
		float ydiff = savey - e->xbutton.y;
		if (++ct < 2)
			return;
		//std::cout << "savex: " << savex << std::endl << std::flush;
		//std::cout << "e->xbutton.x: " << e->xbutton.x << std::endl <<
		//std::flush;
		if (xdiff <= 0) {
			//std::cout << "xdiff: " << xdiff << std::endl << std::flush;
			(*g).ship.angle[0] += .1*TURN*xdiff;
			if ((*g).ship.angle[0] < 0.0f)
				(*g).ship.angle[0] += 360.0f;
		}
		else if (xdiff >= 0) {
			//std::cout << "xdiff: " << xdiff << std::endl << std::flush;
			(*g).ship.angle[0] += .1*TURN*xdiff;
			if ((*g).ship.angle[0] > 360.0f)
				(*g).ship.angle[0] -= 360.0f;
		}
		if (ydiff >= 0) {
			(*g).ship.angle[1] -= .1*PITCH*ydiff;
			if ((*g).ship.angle[1] < 0.1f)
				(*g).ship.angle[1] = 0.1f;
		}
		else if (ydiff <= 0) {
			//std::cout << "xdiff: " << xdiff << std::endl << std::flush;
			(*g).ship.angle[1] -= .1*PITCH*ydiff;
			if ((*g).ship.angle[1] > 179.9f)
				(*g).ship.angle[1] = 179.9f;
		}
		x11.set_mouse_position(100,100);
		savex=100;
		savey=100;
	}


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
		//if (key == XK_Escape)
		//	return 1; //replace with menu state
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

	joeyPhysics(g, gl);
	difficultyScaling(g, gl);
	//pathFindingTest2(g,gl);
	enemyTargeting(g, gl);


}



void render()
{
	glClear(GL_COLOR_BUFFER_BIT);
	(*g).object.drawBase(g, gl);
	joeyStars(g, gl);
	/*if (!(*g).show_credits) {
		(*g).object.drawBase(g, gl);
		for (int i=0; i< (*g).nbullets; i++) {
			//Bullet *b = & (*g).barr[i];
		   (*g).barr[i].drawBullet(g, gl);
	    }
	AdolfoRender(g, gl);
	joeyRender(g, gl);
	luisRender(g, gl);
	scoreBoard(g,gl);
		//ggprint8b(&r, 16, 0x00ffff00, "(object render x=%.1f,object render y=%.1f)", (((*g).ship.angle[0] + 60 - object.polar[1])/120)*gl.xres,  (((*g).ship.angle[1] + 45 - object.polar[2])/90)*gl.yres);
	}*/
	switch ((*g).gameState) {
		case GameState::GS_Menu:
			mainMenuTitle(.5*gl.xres, .8*gl.yres, gl.titleTexture);
			if((*g).playw) {
				mainMenuPlay(.5*gl.xres, .5*gl.yres, gl.playwTexture);
			}
			mainMenuPlay(.5*gl.xres, .5*gl.yres, gl.playTexture);
			if((*g).controlsw) {
				mainMenuControls(.5*gl.xres, .4*gl.yres, gl.controlswTexture);
			}
			mainMenuControls(.5*gl.xres, .4*gl.yres, gl.controlsTexture);
			if((*g).creditsw) {
				mainMenuCredits(.5*gl.xres, .3*gl.yres, gl.creditswTexture);
			}
			mainMenuCredits(.5*gl.xres, .3*gl.yres, gl.creditsTexture);
			if((*g).exitw) {
				mainMenuExit(.5*gl.xres, .2*gl.yres, gl.exitwTexture);
			}
			mainMenuExit(.5*gl.xres, .2*gl.yres, gl.exitTexture);
			break;
		case GameState::GS_Play:
			(*g).object.drawBase(g, gl);
			for (int i=0; i < (*g).nbullets; i++) {
				//Bullet *b = &(*g).barr[i];
				(*g).barr[i].drawBullet(g, gl);
			}
			AdolfoRender(g, gl);
			joeyRender(g, gl);
			luisRender(g, gl);
			scoreBoard(g, gl);
			break;
		case GameState::GS_Controls:
			//
			break;
		case GameState::GS_Credits:
			(*g).mtext -= .02;
	    	andrewH(.5*gl.xres, .9*gl.yres, gl.bigfootTexture,(*g).mtext);
  	  		creditsLuis(.5*gl.xres, .7*gl.yres, gl.luisTexture);
	    	AdolfoValenciaPicture(.5*gl.xres, .5*gl.yres, gl.AdolfoTexture);
    		showChrisRamirez(.5*gl.xres, .3*gl.yres, gl.chrisTexture);
			josephG(.5*gl.xres, .1*gl.yres, gl.josephTexture);
			break;
		case GameState::GS_Exit:
			exit(EXIT_SUCCESS);
			break;
	}
	/*if((*g).show_credits) {
        // function calls for everyone with parameters
		mainMenuTitle(.5*gl.xres, .8*gl.yres, gl.titleTexture);
		mainMenuPlay(.5*gl.xres, .6*gl.yres, gl.playTexture);
		mainMenuControls(.5*gl.xres, .5*gl.yres, gl.controlsTexture);
		mainMenuCredits(.5*gl.xres, .4*gl.yres, gl.creditsTexture);
		mainMenuExit(.5*gl.xres, .3*gl.yres, gl.exitTexture);
	}*/
}
