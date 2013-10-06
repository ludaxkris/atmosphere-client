#pragma once
#include "SpaceObject.h"

class Base : public SpaceObject
{
	public:
		//Everyone's stuff
		///////////////////////////////////////////
		Base(void);
		////////////////////////////////////////////

		//Arjay's system stuff
		////////////////////////////////////////////
		virtual bool isBase(){return true;}
		////////////////////////////////////////////

		//Nick's Graphics stuff
		////////////////////////////////////////////
		char ownerID;
		int baseFlag;
		void updateEffect(ID3DXEffect * effect, GraphicsManager * gManager);
		void drawMesh(MyEffect * myEffect, GraphicsManager * gManager);
		////////////////////////////////////////////
};
