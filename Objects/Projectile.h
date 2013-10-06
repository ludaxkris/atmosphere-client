#pragma once
#include "SpaceObject.h"

class Projectile :	public SpaceObject
{
	public:
		//Everyone's stuff
		//////////////////////////////////////////
		char projFlag;
		Projectile(void);
		/////////////////////////////////////////

		//Arjay's system stuff
		////////////////////////////////////////////
		virtual bool isProjectile(){return true;}
		////////////////////////////////////////////


		//Nick's Grahpics stuff
		///////////////////////////////////////////
		void updateEffect(ID3DXEffect * effect, GraphicsManager * gManager);
		void drawMesh(MyEffect * myEffect, GraphicsManager * gManager);
		//////////////////////////////////////////
};