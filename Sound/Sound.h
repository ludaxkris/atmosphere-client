#pragma once


#define WIN32_LEAN_AND_MEAN 
#include <windows.h>
#include <stdio.h>
#include <conio.h>
#include <math.h>
#include <stdlib.h>
#include "../GraphicsManager/GraphicsManager.h"

#include "fmod.hpp"
#include "fmod_errors.h"

const int   INTERFACE_UPDATETIME = 50;      // 50ms update for interface
const float DISTANCEFACTOR = 1.0f;          // Units per meter.  I.e feet would = 3.28.  centimeters would = 100.

//maximum amount of continuall sounds playing at once.  
//hardware can only handle 31 though... 255 is just in case we get a really good sound card
#define maxChannels 255

//the maximum amount of sounds we load into the game for playing.  like fireing, thrusters, ect
#define maxSounds 255

#define SCALE 1

/*
	Sound Class:

	utlizes fmod.  Is just an extra layer ontop of FMOD so we can use it easily

*/
class Sound
{
//sets up fmod stuff
int setup();

//shuts down fmod stuff
int shutDown();

//finds the next avalible channel
int findNext();

//for debugging
void ERRCHECK(FMOD_RESULT result);




	//the system we got set up
    FMOD::System    *fmodSystem;

	//an array of FMOD::Sound pointers
    FMOD::Sound    **sound;

	//an array of FMOD::Channels
    FMOD::Channel  **channel;

	//bool array to tell what is taken and what is not taken
	bool* taken;



	FMOD_RESULT      result;



    unsigned int     version;
    FMOD_SPEAKERMODE speakermode;
    FMOD_CAPS        caps;
    char             name[256];



public:

//inits the music... sample and roar sound?
int initMusic();

void updatePlayerAudio(GraphicsManager* graphicsManager, D3DXVECTOR3 pos);

//give the position of the player
//give the velocity of the player
//give forward position
//give up position
//updates the position/velocity/forward position and up position of player
void updatePlayer(float posX, float posY, float posZ, float velX, float velY, float velZ,
				 float forX, float forY, float forZ, float upX, float upY, float upZ);


//Plays sound, takes in position of sound and velocity of the thing that just made a sound
void playSound(int soundNumber, float posX, float posY, float posZ, float velX, float velY, float velZ);

//plays the sound in a loop, takes in the position of the sound and the velocity
//returns the sound handle so we can change this again.
int playSoundLoop(int soundNumber, float posX, float posY, float posZ, float velX, float velY, float velZ);

//stopAllSounds
void stopAllSound();



//stop specific sound
void stopSound(int soundHandle);

//give the position of the sound
//give the velocity of the sound
//updates the position and velocity of sound
void updateSound(int soundHandle, float posX, float posY, float posZ, float velX, float velY, float velZ);
 




public:
	Sound(void);
	~Sound(void);
};
