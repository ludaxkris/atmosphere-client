
#include ".\sound.h"

#include <iostream>
using namespace std;


Sound::Sound(void)
{
	//init the sound and malloc my stuff
	setup();
}

Sound::~Sound(void)
{
	//shutdown fmod
	shutDown();
	//free my stuff
	free(taken);
	free(sound);
	free(channel);

}
//sanity check just in case we get an error, this notifies us what the error was
//and shuts down game so we can handle it during testing phases
void Sound::ERRCHECK(FMOD_RESULT result)
{
    if (result != FMOD_OK)
    {
        printf("FMOD error! (%d) %s\n", result, FMOD_ErrorString(result));
//		Sleep(9999);
       // exit(-1);
    }
}

//Plays sound, takes in position of sound and velocity of the thing that just made a sound
void Sound::playSound(int soundNumber, float bposX, float bposY, float bposZ, float velX, float velY, float velZ)
{
	//SCALE
float posX = SCALE *bposX;
float posY = SCALE *bposY;
float posZ = SCALE *bposZ;

	int SOUNDNUMBER = 0;
	fmodSystem->getChannelsPlaying((&SOUNDNUMBER));
//	fmodSystem->update();

	cerr<<"SOUND NUMBER:::"<<SOUNDNUMBER<<endl;
	if(SOUNDNUMBER > 15)
		return;

	//create a temp channel 
	//play sound will just play the sound once then you're done
	//we don't need to hold onto it because the channel is temporary
	FMOD::Channel* tempChan;


    FMOD_VECTOR pos = { posX, posY, posZ};
    FMOD_VECTOR vel = {  velX, velY, velZ};
	//use specific channel to play sound
	//we can have overlapping sound, because 2nd val is set to false
	//if no overlapping sound set it to true

    result = sound[soundNumber]->setMode(FMOD_LOOP_OFF);
    ERRCHECK(result);
 
    result = fmodSystem->playSound(FMOD_CHANNEL_FREE, sound[soundNumber], true, &tempChan);
	
    ERRCHECK(result);

	//7,8,12
	if(7 == soundNumber || 8 == soundNumber || 12 == soundNumber)
		tempChan->setVolume(.85f);

	else if ( 2 != soundNumber && 3!= soundNumber)
		tempChan->setVolume(.1f);
	

	tempChan->set3DMinMaxDistance(40.0f, 100000.5f);

    result = tempChan->set3DAttributes(&pos, &vel);
    if (result != FMOD_OK)
    {
        printf("FMOD error! (%d) %s\n", result, FMOD_ErrorString(result));
    }
	
	float audi = 0.0f;
	tempChan->getAudibility( (&audi) );
	//cerr<<"AUDI:::"<<audi<<endl;
	


	tempChan->setVolume(audi);
    result = tempChan->setPaused(false);
	
	//no error check, if we used up all the channels then just don't play it
	//it's up to the game logic to play the most important sounds first
//    ERRCHECK(result);

}

//plays the sound in a loop, takes in the position of the sound and the velocity
//returns the sound handel so we can change this again.
int Sound::playSoundLoop(int soundNumber, float posX, float posY, float posZ, float velX, float velY, float velZ)
{

	//find next spot
	int nextSpot = findNext();

	//mark channel as taken
	taken[nextSpot] = true;


	FMOD_VECTOR pos = { posX, posY, posZ};
    FMOD_VECTOR vel = {  velX, velY, velZ};

	//looping sound cannot have multiple of themselves playing
    result = fmodSystem->playSound(FMOD_CHANNEL_FREE, sound[soundNumber], true, &channel[nextSpot]);
    ERRCHECK(result);
    result = channel[nextSpot]->set3DAttributes(&pos, &vel);
    ERRCHECK(result);
    result = channel[nextSpot]->setPaused(false);
    ERRCHECK(result);

	return nextSpot;
}

//stopAllSounds
void Sound::stopAllSound()
{	
	for(int i = 0; i<maxChannels; i++)
	{
		if(taken[i] == true)
		{
			stopSound(i);
		}
	}

}

//stop specific sound, takes in handel to the sound we want to stop
void Sound::stopSound(int soundHandle)
{
	taken[soundHandle] = false;
	result = channel[soundHandle]->setPaused(true);
	ERRCHECK(result);

}


//give the position of the player
//give the velocity of the player
//give forward position
//give up position
//updates the position/velocity/forward position and up position of player
void Sound::updatePlayer(float bposX, float bposY, float bposZ, float velX, float velY, float velZ,
				 float forX, float forY, float forZ, float upX, float upY, float upZ)
{

		//SCALE
float posX = SCALE *bposX;
float posY = SCALE *bposY;
float posZ = SCALE *bposZ;


    FMOD_VECTOR forward        = { forX, forY, forZ };
    FMOD_VECTOR up             = { upX, upY, upZ };
    FMOD_VECTOR vel  = { velX, velY, velZ};
    FMOD_VECTOR pos  = { posX, posY, posZ};

    result = fmodSystem->set3DListenerAttributes(0, &pos, &vel, &forward, &up);
    ERRCHECK(result);

	updateSound(0,posX,posY,posZ,velX,velY,velZ);
	updateSound(1,posX,posY,posZ,velX,velY,velZ);

	fmodSystem->update();

}

//give the position of the sound
//give the velocity of the sound
//updates the position and velocity of sound
void Sound::updateSound(int soundHandle, float posX, float posY, float posZ, float velX, float velY, float velZ)
{
	FMOD_VECTOR pos = { posX, posY, posZ};
    FMOD_VECTOR vel = {  velX, velY, velZ};

    result = channel[soundHandle]->set3DAttributes(&pos, &vel);
    ERRCHECK(result);

	fmodSystem->update();

}

//shutdowns fmod
int Sound::shutDown()
{
	for(int i = 0; i<maxSounds;i++)
	{
		result = sound[i]->release();
		ERRCHECK(result);
	}

    result = fmodSystem->close();
    ERRCHECK(result);
    result = fmodSystem->release();
    ERRCHECK(result);
	return 0;
}


    /*
        Play sounds at certain positions
    */
//inits the music... sample and roar sound?
int Sound::initMusic()
{
	playSoundLoop(0, 0,0,0,0,0,0);
	channel[0]->setVolume(.0005f);
	playSoundLoop(1, 0,0,0,0,0,0);
	channel[1]->setVolume(.01f);

	return 0;
}

//find next avalible channel
int Sound::findNext()
{
	for(int i = 0; i< maxChannels; i++)
		if(taken[i] == false)
			return i;
	return -1;
}


//update players position
void Sound::updatePlayerAudio(GraphicsManager* graphicsManager, D3DXVECTOR3 pos2)
{
						   
	D3DXVECTOR3 up = graphicsManager->cam->GetUpVec();
	D3DXVECTOR3	face = graphicsManager->cam->GetLookVec();
	D3DXVECTOR3 pos = graphicsManager->cam->GetPosVec();


//	cerr<<"pos :: X :: "<<pos.x<<" :: Y :: "<<pos.y<<" :: Z :: "<<pos.z<<endl;



	updatePlayer(pos.x, pos.y, pos.z, 0, 0, 0, face.x, face.y, face.z, up.x, up.y, up.z);
}



//setup fmod stuff
int Sound::setup()
{

	taken = (bool*)malloc(sizeof(bool) * maxChannels);
	for(int i = 0; i<maxChannels; i++)
		taken[i] = false;

	//init the sound and channel arrays
	sound = (FMOD::Sound**) malloc( maxSounds *sizeof(FMOD::Sound*) );
	memset (sound,0,(maxSounds*sizeof(FMOD::Sound*) ));

	channel = (FMOD::Channel**) malloc(maxChannels *sizeof(FMOD::Channel*));
	memset (channel,0,(maxChannels*sizeof(FMOD::Channel*) ));


    /*
        Create a fmodSystem object and initialize.
    */
    result = FMOD::System_Create(&fmodSystem);
    ERRCHECK(result);
    
    result = fmodSystem->getVersion(&version);
    ERRCHECK(result);

    if (version < FMOD_VERSION)
    {
        printf("Error!  You are using an old version of FMOD %08x.  This program requires %08x\n", version, FMOD_VERSION);
        return 0;
    }
    
    result = fmodSystem->getDriverCaps(0, &caps, 0, 0, &speakermode);
    ERRCHECK(result);

    result = fmodSystem->setSpeakerMode(speakermode);       /* Set the user selected speaker mode. */
    ERRCHECK(result);

    if (caps & FMOD_CAPS_HARDWARE_EMULATED)             /* The user has the 'Acceleration' slider set to off!  This is really bad for latency!. */
    {                                                   /* You might want to warn the user about this. */
        result = fmodSystem->setDSPBufferSize(1024, 10);
        ERRCHECK(result);
    }

    result = fmodSystem->getDriverInfo(0, name, 256, 0);
    ERRCHECK(result);

    if (strstr(name, "SigmaTel"))   /* Sigmatel sound devices crackle for some reason if the format is PCM 16bit.  PCM floating point output seems to solve it. */
    {
        result = fmodSystem->setSoftwareFormat(48000, FMOD_SOUND_FORMAT_PCMFLOAT, 0,0, FMOD_DSP_RESAMPLER_LINEAR);
        ERRCHECK(result);
    }

	result = fmodSystem->init(1000, FMOD_INIT_NORMAL, 0);
    if (result == FMOD_ERR_OUTPUT_CREATEBUFFER)         /* Ok, the speaker mode selected isn't supported by this soundcard.  Switch it back to stereo... */
    {
        result = fmodSystem->setSpeakerMode(FMOD_SPEAKERMODE_STEREO);
        ERRCHECK(result);
                
        result = fmodSystem->init(100, FMOD_INIT_NORMAL, 0);/* ... and re-init. */
        ERRCHECK(result);
    }




    /*
        Load some sounds
    */
	//music
    result = fmodSystem->createSound("./media/Sounds/Demo.mp3", FMOD_3D, 0, &sound[0]);
    ERRCHECK(result);
    result = sound[0]->setMode(FMOD_LOOP_NORMAL);
    ERRCHECK(result);
	


	//thrustors
    result = fmodSystem->createSound("./media/Sounds/thrusters.wav", FMOD_3D_LOGROLLOFF, 0, &sound[1]);
    ERRCHECK(result);
    result = sound[1]->setMode(FMOD_LOOP_NORMAL);
    ERRCHECK(result);


//    result = fmodSystem->createSound("../media/swish.wav", FMOD_3D, 0, &sound[2]);
//    ERRCHECK(result);

    result = fmodSystem->createSound("./media/Sounds/shildHit.wav", FMOD_3D | FMOD_3D_LOGROLLOFF, 0, &sound[2]);
    ERRCHECK(result);

//	sound[2]->set3DMinMaxDistance(1.0f, 10.0f);

    result = fmodSystem->createSound("./media/Sounds/shildHit.wav", FMOD_3D | FMOD_3D_LOGROLLOFF, 0, &sound[3]);
    ERRCHECK(result);
//	sound[3]->set3DMinMaxDistance(1.0f, 10.0f);

	//suppose to be collide but couldn't find a good one.
    result = fmodSystem->createSound("./media/Sounds/shildHit.wav", FMOD_3D | FMOD_3D_LOGROLLOFF, 0, &sound[4]);
    ERRCHECK(result);



    result = fmodSystem->createSound("./media/Sounds/shildRecharge.wav", FMOD_3D, 0, &sound[5]);
    ERRCHECK(result);

    result = fmodSystem->createSound("./media/Sounds/shildLoss.wav", FMOD_3D, 0, &sound[6]);
    ERRCHECK(result);

    result = fmodSystem->createSound("./media/Sounds/countdown.wav", FMOD_3D, 0, &sound[7]);
    ERRCHECK(result);

 
    result = fmodSystem->createSound("./media/Sounds/shipDistruction.wav", FMOD_3D, 0, &sound[8]);
    ERRCHECK(result);

    result = fmodSystem->createSound("./media/Sounds/welcome.wav", FMOD_3D, 0, &sound[9]);
    ERRCHECK(result);

    result = fmodSystem->createSound("./media/Sounds/doubleKill.wav", FMOD_3D, 0, &sound[10]);
    ERRCHECK(result);

    result = fmodSystem->createSound("./media/Sounds/multiKill.wav", FMOD_3D, 0, &sound[11]);
    ERRCHECK(result);

    result = fmodSystem->createSound("./media/Sounds/alert.wav", FMOD_3D, 0, &sound[12]);
    ERRCHECK(result);

    result = fmodSystem->createSound("./media/Sounds/bombExp.wav", FMOD_3D, 0, &sound[13]);
    ERRCHECK(result);


	/*
        Set the distance units. (meters/feet etc).
    */
    result = fmodSystem->set3DSettings(1.0f, 1.0f, 5.0f);
    ERRCHECK(result);


	//result = fmodSystem->setGeometrySettings(2000.0f);




	return 0;
}
