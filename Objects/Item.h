#pragma once
#include "SpaceObject.h"

class Item : public SpaceObject
{
	public:
		//Everyone's stuff
		///////////////////////////////////////////
		Item(void);
		int itemType;
		////////////////////////////////////////////

		//Arjay's system stuff
		////////////////////////////////////////////
		virtual bool isItem(){return true;}
		////////////////////////////////////////////

		//Nick's Graphics stuff
		////////////////////////////////////////////		
		//int baseFlag;
		//void updateEffect(ID3DXEffect * effect, GraphicsManager * gManager);
		//void drawMesh(MyEffect * myEffect, GraphicsManager * gManager);
		////////////////////////////////////////////
};
