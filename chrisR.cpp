//Program: chrisR.cpp
//Author:  Christopher Ramirez
//Date:    2019
//Modified: March 9, 12:01 PM
#include <GL/glx.h>
#include <cstdlib>
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

#ifdef USE_OPENAL_SOUND
class OPENAL_SOUND_ENGINE {
public:
    float vec[6] = {0.0f,0.0f,1.0f, 0.0f,1.0f,0.0f};
    ALuint alBuffer[1];
    ALuint alSource[1];
public:
    OPENAL_SOUND_ENGINE() {
        alutInit(0, NULL);
        if(alGetError() != AL_NO_ERROR) {
            printf("ERROR: alutInit()\n");
            exit(EXIT_FAILURE);
        }
        //Clear error state
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
        alBuffer[0] = alutCreateBufferFromFile("./bullet_fire.wav");
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
            exit(EXIT_FAILURE);//0;
        }
    }
    ~OPENAL_SOUND_ENGINE() {
        //Cleanup
        //1st delete Sources
        alDeleteSources(1, &alSource[0]);
        //2nd delete Buffers
        alDeleteBuffers(1, &alBuffer[0]);
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
    alSourcePlay(audiothing.alSource[0]);
}
