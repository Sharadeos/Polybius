//Program: chrisR.cpp
//Author:  Christopher Ramirez
//Date:    2019
//Modified: April 30, 12:23 PM
#include "classes.h"
void showChrisRamirez(int x, int y, GLuint textid)
{
	Rect r;
	unsigned int c = 0x00ffff44;
	r.bot = y;
	r.left = x;
	r.center = 0;
	ggprint8b(&r, 16, c, "Chris Ramirez");
	static int wid = 120;
	glPushMatrix();
	glTranslatef(x+250, y, 0);
	glBindTexture(GL_TEXTURE_2D, textid);
	glEnable(GL_ALPHA_TEST);
	glColor3ub(255, 255, 255);
	glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 1.0f); glVertex2i(-wid,-wid);
		glTexCoord2f(0.0f, 0.0f); glVertex2i(-wid, wid);
		glTexCoord2f(1.0f, 0.0f); glVertex2i( wid, wid);
		glTexCoord2f(1.0f, 1.0f); glVertex2i( wid,-wid);
	glEnd();
	glPopMatrix();
}
//Credit to Gordon's Lab2 rainforrest.cpp
//Enables .png transparency
unsigned char *buildAlphaData(Image *img)
{
	//add 4th component to RGB stream...
	int i;
	int a,b,c;
	unsigned char *newdata, *ptr;
	unsigned char *data = (unsigned char *)img->data;
	newdata = (unsigned char *)malloc(img->width * img->height * 4);
	ptr = newdata;
	for (i=0; i<img->width * img->height * 3; i+=3) {
		a = *(data+0);
		b = *(data+1);
		c = *(data+2);
		*(ptr+0) = a;
		*(ptr+1) = b;
		*(ptr+2) = c;
		*(ptr+3) = (a|b|c);
		ptr += 4;
		data += 3;
	}
	return newdata;
}

//Menu Textures
void mainMenuTitle(int x, int y, GLuint textid) 
{
	static int wid = 120;
	glPushMatrix();
	glTranslatef(x-360, y, 0);
	glBindTexture(GL_TEXTURE_2D, textid);
	glEnable(GL_ALPHA_TEST);
	glColor3ub(255, 255, 255);
	glBegin(GL_QUADS);
		//UPLeft, DOWNL, DRight, UR | (x, y)ish
		glTexCoord2f(0.0f, 1.0f); glVertex2i(-wid,-wid); //x = wid-480?
				glTexCoord2f(0.0f, 0.0f); glVertex2i(-wid, wid);
				glTexCoord2f(1.0f, 0.0f); glVertex2i( wid+720, wid);
				glTexCoord2f(1.0f, 1.0f); glVertex2i( wid+720,-wid);
	glEnd();
	glPopMatrix();
}
void mainMenuPlay(int x, int y, GLuint textid) 
{
		static int wid = 40;
		glPushMatrix();
		glTranslatef(x-120, y, 0);
		glBindTexture(GL_TEXTURE_2D, textid);
		glEnable(GL_ALPHA_TEST);
		glColor3ub(255, 255, 255);
		glBegin(GL_QUADS);
				//UPLeft, DOWNL, DRight, UR | (x, y)ish
				glTexCoord2f(0.0f, 1.0f); glVertex2i(-wid,-wid); //x = wid-480?
				glTexCoord2f(0.0f, 0.0f); glVertex2i(-wid, wid);
				glTexCoord2f(1.0f, 0.0f); glVertex2i( wid+240, wid);
				glTexCoord2f(1.0f, 1.0f); glVertex2i( wid+240,-wid);
		glEnd();
		glPopMatrix();
}
void mainMenuControls(int x, int y, GLuint textid) 
{
		static int wid = 40;
		glPushMatrix();
		glTranslatef(x-240, y, 0);
		glBindTexture(GL_TEXTURE_2D, textid);
		glEnable(GL_ALPHA_TEST);
		glColor3ub(255, 255, 255);
		glBegin(GL_QUADS);
				//UPLeft, DOWNL, DRight, UR | (x, y)ish
				glTexCoord2f(0.0f, 1.0f); glVertex2i(-wid,-wid); //x = wid-480?
				glTexCoord2f(0.0f, 0.0f); glVertex2i(-wid, wid);
				glTexCoord2f(1.0f, 0.0f); glVertex2i( wid+480, wid);
				glTexCoord2f(1.0f, 1.0f); glVertex2i( wid+480,-wid);
		glEnd();
		glPopMatrix();
}
void mainMenuCredits(int x, int y, GLuint textid) 
{
		static int wid = 40;
		glPushMatrix();
		glTranslatef(x-180, y, 0);
		glBindTexture(GL_TEXTURE_2D, textid);
		glEnable(GL_ALPHA_TEST);
		glColor3ub(255, 255, 255);
		glBegin(GL_QUADS);
				//UPLeft, DOWNL, DRight, UR | (x, y)ish
				glTexCoord2f(0.0f, 1.0f); glVertex2i(-wid,-wid); //x = wid-480?
				glTexCoord2f(0.0f, 0.0f); glVertex2i(-wid, wid);
				glTexCoord2f(1.0f, 0.0f); glVertex2i( wid+360, wid);
				glTexCoord2f(1.0f, 1.0f); glVertex2i( wid+360,-wid);
		glEnd();
		glPopMatrix();
}
void mainMenuExit(int x, int y, GLuint textid) 
{
		static int wid = 40;
		glPushMatrix();
		glTranslatef(x-120, y, 0);
		glBindTexture(GL_TEXTURE_2D, textid);
		glEnable(GL_ALPHA_TEST);
		glColor3ub(255, 255, 255);
		glBegin(GL_QUADS);
				//UPLeft, DOWNL, DRight, UR | (x, y)ish
				glTexCoord2f(0.0f, 1.0f); glVertex2i(-wid,-wid);
				glTexCoord2f(0.0f, 0.0f); glVertex2i(-wid, wid);
				glTexCoord2f(1.0f, 0.0f); glVertex2i( wid+240, wid);
				glTexCoord2f(1.0f, 1.0f); glVertex2i( wid+240,-wid);
		glEnd();
		glPopMatrix();
}
void onMenuButton();
void stateKeys(Game *g, Global gl) 
{
	//Up
	if (gl.keyhits[62]) {
		if ((*g).menuitem > 0) {
			(*g).menuitem--;
		}
		onMenuButton();
		usleep(100000);
	}
	//Down
	if (gl.keyhits[64]) {
		if ((*g).menuitem < 3) {
			(*g).menuitem++;
		}
		onMenuButton();
		usleep(100000);
	}
	//Enter
	if (gl.keyhits[93]) {
		if ((*g).menuitem == 0) {
			(*g).gameState = GameState::GS_Play;
			(*g).playmusic = true;
		}
		if ((*g).menuitem == 1) {
			(*g).gameState = GameState::GS_Controls;
		}
		if ((*g).menuitem == 2) {
			(*g).gameState = GameState::GS_Credits;
		}
		if ((*g).menuitem == 3) {
			(*g).gameState = GameState::GS_Exit;
		}
	}
	//ESC
	if (gl.keyhits[7]) {
		(*g).gameState = GameState::GS_Menu;
		(*g).menuitem = 0;
	}
}
void checkMenuItem(Game *g) 
{
	if ((*g).menuitem == 0) {
		(*g).playw = 1;
		(*g).controlsw = 0;
		(*g).creditsw = 0;
		(*g).exitw = 0;
	}
	if((*g).menuitem == 1) {
		(*g).playw = 0;
		(*g).controlsw = 1;
		(*g).creditsw = 0;
		(*g).exitw = 0;
	}
	if ((*g).menuitem == 2) {
		(*g).playw = 0;
		(*g).controlsw = 0;
		(*g).creditsw = 1;
		(*g).exitw = 0;
	}
	if ((*g).menuitem == 3) {
		(*g).playw = 0;
		(*g).controlsw = 0;
		(*g).creditsw = 0;
		(*g).exitw = 1;
	}
}
//Organized OPENAL_SOUND_ENGINE class Lab-8
#ifdef USE_OPENAL_SOUND
#define FILENUM 13
class OPENAL_SOUND_ENGINE {
public:
	float vec[6] = {0.0f,0.0f,1.0f, 0.0f,1.0f,0.0f};
	ALuint alBuffer[FILENUM+1];
	ALuint alSource[FILENUM+1];
	string sfxFiles[FILENUM] = {"./audio/weapon1.wav", "./audio/weapon2.wav",
			"./audio/pop.wav", "./audio/death.wav", "./audio/missilelaunch.wav",
			"./audio/explosion.wav", "./audio/menu.wav",
			"./audio/weaponswitch.wav", "./audio/idlealien.wav",
			"./audio/alarm.wav", "./audio/CivilExploreBattle.wav",
			"./audio/MantisExplore.wav", "./audio/TitleScreen.wav"
	};
public:
	OPENAL_SOUND_ENGINE() {
		alutInit(0, NULL);
		if(alGetError() != AL_NO_ERROR) {
			printf("ERROR: alutInit()\n");
			exit(EXIT_FAILURE);
		}
		//Clear error state
		alGetError();
		//Setup Listener
		//Forward and Upward vectors are used
		float vec[6] = {0.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f};
		alListener3f(AL_POSITION, 0.0f, 0.0f, 0.0f);
		alListener3f(AL_VELOCITY, 0.0f, 0.0f, 0.0f);
		alListenerfv(AL_ORIENTATION, vec);
		//To be modified by changing MasterGain
		alListenerf(AL_GAIN, 1.0f);
		//Setup Buffer: holds the sound information
		//Each Source will be attatched to an individual Buffer
		//Due to each sound-card having an unknown amount of allowable
		//Buffers, only 16 Buffers will be used in total at any given time.
		for(int i=0; i<FILENUM; i++) {
			alBuffer[i] = alutCreateBufferFromFile(sfxFiles[i].c_str());
		}
		//Generate a source, store it in a buffer
		alGenSources(FILENUM+1, alSource);
		for(int i=0; i<FILENUM; i++) {
			//alGenSources(i+1, alSource);
			alSourcei(alSource[i], AL_BUFFER, alBuffer[i]);
		}
		//NO LOOPING: Set Volume and Pitch to normal
		//When using alSource3f, offset AL_GAIN's Z for stereo effect
		for(int i=0; i < (FILENUM - 4); i++) {
			alSourcef(alSource[i], AL_GAIN, 0.1f);
			alSourcef(alSource[i], AL_PITCH, 1.0f);
			alSource3f(alSource[i], AL_POSITION, 0.0f, 0.0f, 0.0f);
			alSource3f(alSource[i], AL_VELOCITY, 0.0f, 0.0f, 0.0f);
			alSourcei(alSource[i], AL_LOOPING, AL_FALSE);
			if (alGetError() != AL_NO_ERROR) {
				printf("Error: setting source\n");
				exit(EXIT_FAILURE); //0;
			}
		}
		//LOOPING: Set Volume and Pitch to normal
		for(int i=8; i < FILENUM; i++) {
			alSourcef(alSource[i], AL_GAIN, 1.0f);
			alSourcef(alSource[i], AL_PITCH, 1.0f);
			alSourcei(alSource[i], AL_LOOPING, AL_TRUE);
			if (alGetError() != AL_NO_ERROR) {
				printf("Error: setting source\n");
				exit(EXIT_FAILURE); //0;
			}
		}
	}
	~OPENAL_SOUND_ENGINE() {
		//Cleanup: delete Sources then Buffers
		for(int i=0; i < FILENUM; i++) {
			alDeleteSources(1, &alSource[i]);
			alDeleteBuffers(1, &alBuffer[i]);
		}
		//Close out OpenAL itself
		//Get active Context
		ALCcontext *Context = alcGetCurrentContext();
		//Get Device for active Context
		ALCdevice *Device = alcGetContextsDevice(Context);
		//Disable Context
		alcMakeContextCurrent(NULL);
		//Release Context(s)
		alcDestroyContext(Context);
		//Close Device
		alcCloseDevice(Device);
	}
} audiothing;
#endif
void weapon1() 
{
	float output = rand() % 12 + 4;//min + (rand() % static_cast<float>(max - min + 1));
	alSourcei(audiothing.alSource[0], AL_PITCH, output);
	alSourcePlay(audiothing.alSource[0]);
}
void weapon2() 
{
		alSourcePlay(audiothing.alSource[1]);
}
void onHit() 
{
		float output = rand() % 11 + 4;
		alSourcei(audiothing.alSource[2], AL_PITCH, output);
		alSourcePlay(audiothing.alSource[2]);
}
void onDeath() 
{
		alSourcePlay(audiothing.alSource[3]);
}
void missile() 
{
		alSourcePlay(audiothing.alSource[4]);
}
void onMenuButton() 
{
		alSourcePlay(audiothing.alSource[6]);
}
void playTitleMusic() 
{
		alSourcePlay(audiothing.alSource[12]);
		alSourcei(audiothing.alSource[12], AL_SOURCE_RELATIVE, AL_TRUE);
}
void pauseTitleMusic() 
{
	alSourcePause(audiothing.alSource[12]);
	alSourceRewind(audiothing.alSource[12]);
}
void playMusic() 
{
	alSourcePlay(audiothing.alSource[11]);
	alSourcei(audiothing.alSource[11], AL_SOURCE_RELATIVE, AL_TRUE);
	alSourcePlay(audiothing.alSource[10]);
	alSourcei(audiothing.alSource[10], AL_SOURCE_RELATIVE, AL_TRUE);
	alSourcef(audiothing.alSource[10], AL_GAIN, 0.0f);
}
void playExplosion() 
{
	alSourcei(audiothing.alSource[5], AL_SOURCE_RELATIVE, AL_FALSE);
	alSource3f(audiothing.alSource[5], AL_DIRECTION, 0.0f, 0.0f, 0.0f);
	alSourcePlay(audiothing.alSource[5]);
}
void alShipLocation(ALenum param, ALfloat v1, ALfloat v2, ALfloat v3) 
{
	alSource3f(audiothing.alSource[9], param, v1, v2, v3);
}

float tincrement = 0.01;
bool actionFlag = false, actionFlag2 = false;
double tdif = 0.0;
struct timespec at;
bool setActionFlag() 
{
	actionFlag2 = true;
	if(actionFlag == false) {
		actionFlag = true;
	}else {
		actionFlag = false;
	}
	clock_gettime(CLOCK_REALTIME, &at);
	return 0;
}
void checkAction(struct timespec *t) 
{
	if(actionFlag == true) {
		tdif = timeDiff(&at, t);
		if(tdif <= 3.0) {
			if(tdif == 3.0) {
				timeCopy(t, &at);
			}
			tincrement += 0.01;
			alSourcef(audiothing.alSource[10], AL_GAIN, tincrement);
			alSourcef(audiothing.alSource[11], AL_GAIN, fabs(tincrement - 1.0));
		}
	}
	if((actionFlag == false) && (actionFlag2 == true)) {
			tdif = timeDiff(&at, t);
		if(tdif <= 3.0) {
			tincrement -= 0.01;
			alSourcef(audiothing.alSource[10], AL_GAIN, tincrement);
			alSourcef(audiothing.alSource[11], AL_GAIN, fabs(tincrement - 1.0));
		}
	}
}
void ALPlayerUpdate(ALenum param, ALfloat v1, ALfloat v2, ALfloat v3) 
{
	alListener3f(param, v1, v2, v3);
}
void ALPlayerVel(ALenum param, ALfloat v1) 
{
	alListeneri(param, v1);
}
void orientationVec(ALfloat v1, ALfloat v2, ALfloat v3, ALfloat v4, ALfloat v5,
	ALfloat v6) 
{
	audiothing.vec[0] = v1;
	audiothing.vec[1] = v2;
	audiothing.vec[2] = v3;
	audiothing.vec[3] = v4; //0.0f;
	audiothing.vec[4] = v5; //1.0f;
	audiothing.vec[5] = v6; //0.0f;
	alListenerfv(AL_ORIENTATION, audiothing.vec);
}
