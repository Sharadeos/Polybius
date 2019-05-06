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
	static int wid = 70;
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
//Organized OPENAL_SOUND_ENGINE class Lab-8
#ifdef USE_OPENAL_SOUND
#define FILENUM 5
class OPENAL_SOUND_ENGINE {
public:
	float vec[6] = {0.0f,0.0f,1.0f, 0.0f,1.0f,0.0f};
	ALuint alBuffer[FILENUM+1];
	ALuint alSource[FILENUM+1];
	string sfxFiles[FILENUM] = {"./audio/bullet_fire.wav", "./audio/pop.wav",
			"./audio/737engine.wav", "./audio/MilkyWayBATTLE.wav",
			"./audio/MilkyWayEXPLORE.wav"
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
		for(int i=0; i < (FILENUM - 3); i++) {
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
		for(int i=2; i < FILENUM; i++) {
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
void pewPew() {
    float output = rand() % 12 + 4;//min + (rand() % static_cast<float>(max - min + 1));
    alSourcei(audiothing.alSource[0], AL_PITCH, output);
    alSourcePlay(audiothing.alSource[0]);
}
void playMusic() {
	alSourcePlay(audiothing.alSource[4]);
	alSourcei(audiothing.alSource[4], AL_SOURCE_RELATIVE, AL_TRUE);
	alSourcePlay(audiothing.alSource[3]);
	alSourcei(audiothing.alSource[3], AL_SOURCE_RELATIVE, AL_TRUE);
	alSourcef(audiothing.alSource[3], AL_GAIN, 0.0f);
}
void playEngine() {
	alSourcei(audiothing.alSource[2], AL_SOURCE_RELATIVE, AL_FALSE);
	alSource3f(audiothing.alSource[2], AL_DIRECTION, 0.0f, 0.0f, 0.0f);
	alSourcePlay(audiothing.alSource[2]);
}
void pauseEngine() {
    alSourcePause(audiothing.alSource[2]);
}
void alShipLocation(ALenum param, ALfloat v1, ALfloat v2, ALfloat v3) {
	alSource3f(audiothing.alSource[2], param, v1, v2, v3);
}
/*bool checkActivity(int &init, int &num, bool &flag) {
    if(init != num) {
        flag = true;
        (*g).initialNumAsteroids = (*g).nasteroids;
    }
    if(
}*/
void checkActivity(Game *g, Global gl) {
	
}
float tincrement = 0.01;
bool actionFlag = false, actionFlag2 = false;
double tdif = 0.0;
struct timespec at;
bool setActionFlag() {
	actionFlag2 = true;
	if(actionFlag == false) {
		actionFlag = true;
	}else {
		actionFlag = false;
	}
	clock_gettime(CLOCK_REALTIME, &at);
}
void checkAction(struct timespec *t) {
	if(actionFlag == true) {
		tdif = timeDiff(&at, t);
		if(tdif <= 3.0) {
			tincrement += 0.01;
			alSourcef(audiothing.alSource[3], AL_GAIN, tincrement);
			alSourcef(audiothing.alSource[4], AL_GAIN, fabs(tincrement - 1.0));
		}
	}
	if((actionFlag == false) && (actionFlag2 == true)) {
	    	tdif = timeDiff(&at, t);
		if(tdif <= 3.0) {
			tincrement -= 0.01;
			alSourcef(audiothing.alSource[3], AL_GAIN, tincrement);
			alSourcef(audiothing.alSource[4], AL_GAIN, fabs(tincrement - 1.0));
		}
	}
}
//void ALExplodeUpdate(ALenum param, float x, float y //ALfloat *z//) {
//	alSource3f(audiothing.alSource[1], param, x, y, -2.0);
//}
//EX: ALPlayerUpdate(AL_VELOCITY,
void ALPlayerUpdate(ALenum param, ALfloat v1, ALfloat v2, ALfloat v3) {
	alListener3f(param, v1, v2, v3);
}
void ALPlayerVel(ALenum param, ALfloat v1) {
    alListeneri(param, v1);
}
void orientationVec(ALfloat v1, ALfloat v2, ALfloat v3, ALfloat v4, ALfloat v5,
	ALfloat v6) {
    audiothing.vec[0] = v1;
    audiothing.vec[1] = v2;
    audiothing.vec[2] = v3;
    audiothing.vec[3] = v4; //0.0f;
    audiothing.vec[4] = v5; //1.0f;
    audiothing.vec[5] = v6; //0.0f;
    alListenerfv(AL_ORIENTATION, audiothing.vec);
}
//void ALSource
