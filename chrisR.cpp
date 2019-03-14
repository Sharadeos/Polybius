//Program: chrisR.cpp
//Author:  Christopher Ramirez
//Date:    2019
//Modified: March 9, 12:01 PM
#include <GL/glx.h>
#include "fonts.h"
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#ifdef USE_OPENAL_SOUND
#include </usr/include/AL/alut.h>
#endif
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

//void setUpOPENAL () {}?
//void BulletSounds() {}
//void clearOPENAL()
// MAKE OPENAL CLASS
/*
void setUpOPENAL() {
#ifdef USE_OPENAL_SOUND
	alutInit(0, NULL);
	if(alGetError() != AL_NO_ERROR) {
		printf("ERROR: alutInit()\n");
		//return 0;
	}
        //Clear error state.
        alGetError();
#endif
}
void setUpListener() {
#ifdef USE_OPENAL_SOUND
        //Setup Listener
        //Forward and Upward vectors are used
        float vec[6] = {0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f};
        alListener3f(AL_POSITION, 0.0f, 0.0f, 0.0f);
        alListenerfv(AL_ORIENTATION, vec);
        alListenerf(AL_GAIN, 1.0f);
#endif
}
void setUpBuffer() {
#ifdef USE_OPEN_SOUND
        //Setup Buffer: holds the sound information
        ALuint alBuffer[1];
	alBuffer[0] = alutCreateBufferFromFile("./bullet_fire.wav");
#endif
}
void setUpSource() {
#ifdef USE_OPEN_SOUND*/
	//Generate [1] source and store in the matching buffers
	//
	//Source == Sound
	/*
	ALuint alSource[1];
        //Generate a source, store it in a buffer
        alGenSources(1, alSource);
        alSourcei(alSource[0], AL_BUFFER, alBuffer[0]);
        //Set Volume and Pitch to normal
        //No Looping of Sound
        alSourcef(alSource[0], AL_GAIN, 0.1f);
        alSourcef(alSource[0], AL_PITCH, 1.0f);
        alSourcei(alSource[0], AL_LOOPING, AL_FALSE);
        if (alGetError() != AL_NO_ERROR) {
                printf("ERROR: setting source\n");
                return 0;
        }
	alSourcePlay(alSource[0]);
#endif
}
void pewPew() {
#ifdef USE_OPENAL_SOUND
	extern ALuint *alSource;
	alSourcePlay(alSource[0]);
#endif
}
void audioCleanup() {
#ifdef USE_OPENAL_SOUND
	extern ALuint *alSource;
	extern ALuint *alBuffer;
	//Cleanup
	//Delete the sources
	alDeleteSources(1, &alSource[0]);
	//Delete the Buffers
	alDeleteBuffers(1, &alBuffer[0]);
	//
	//Close out OpenAL itself
	//Get active context
	ALCcontext *Context = alcGetCurrentContext();
	//Get device for active context
	ALCdevice *Device = alcGetContextsDevice(Context);
	//Disable context
	alcMakeContextCurrent(NULL);
	//Release context(s)
	alcDestroyContext(Context);
	//Close Device
	alcCloseDevice(Device);
#endif
}

void audioTestFunction()
{
#ifdef USE_OPENAL_SOUND
	alutInit(0, NULL);
	if(alGetError() != AL_NO_ERROR) {
		printf("ERROR: alutInit()\n");
		return 0;
	}
	//Clear error state.
	alGetError();
	//
	//Setup Listener
	//Forward and Upward vectors are used
	float vec[6] = {0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f};
	alListener3f(AL_POSITION, 0.0f, 0.0f, 0.0f);
	alListenerfv(AL_ORIENTATION, vec);
	alListenerf(AL_GAIN, 1.0f);
	//
	//Setup Buffer: holds the sound information
	ALuint alSource;
	//Generate a source, store it in a buffer
	alGenSources(1, &alSource);
	alSourcei(alSource, AL_BUFFER, alBuffer);
	//Set Volume and Pitch to normal
	//No Looping of Sound
	alSourcef(alSource, AL_GAIN, 0.0f);
	alSourcef(alSource, AL_PITCH, 1.0f);
	alSourcei(alSource, AL_LOOPING, AL_FALSE);
	if (alGetError() != AL_NO_ERROR) {
		printf("ERROR: setting source\n");
		return 0;
	}
}*/

