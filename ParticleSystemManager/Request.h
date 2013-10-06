#pragma once
#include "./../Common/dxstdafx.h"
#include "./../Objects/SpaceObject.h"
#include "Emitter.h"

class Request
{
public:

	Request(Emitter * p_emitter, SpaceObject * p_spaceObj);
	~Request();
	void Activate(DWORD f_startStamp);

	void setActive();
	void setDead();
	bool isActive();

	void setDying();
	void setNotDying();
	bool isDying();
	
	bool isConstant();

	void update();
	void render(Camera * p_cam);

	int getTTL();
	int getStartStamp();
	void setStartStamp(int f_startStamp);

	bool hasLivingParts();
	
private:
	//object pointer??
	SpaceObject * p_spaceObj;
	int f_startStamp;
	Emitter * p_Emitter;
};