#pragma once
#include "SpaceObject.h"

class Ship : public SpaceObject
{

public:
	int shipFlag;
	
	Ship(void);
	
	//Nick
	//////////////////////////////////////////////////////////////////////////////////
	void updateEffect(ID3DXEffect * effect, GraphicsManager * gManager);
	void drawMesh(MyEffect * myEffect, GraphicsManager * gManager);

};
