#pragma once
#include "SpaceObject.h"

class Asteroid : public SpaceObject
{
	public:
		//Everyone's stuff
		/////////////////////////////////////////
		Asteroid(void);
		////////////////////////////////////////

		//Arjay's system stuff
		////////////////////////////////////////////
		virtual bool isAsteroid(){return true;}
		////////////////////////////////////////////
};
