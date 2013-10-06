#include "Request.h"

Request::Request(Emitter * p_Emitter, SpaceObject * p_spaceObj)
{
	this->p_spaceObj = p_spaceObj;
	this->p_Emitter = p_Emitter;
	this->f_startStamp = 0;
}

Request::~Request()
{
	this->p_spaceObj = NULL;
	this->p_Emitter = NULL;
	this->f_startStamp = 0;
}

void Request::Activate(DWORD f_startStamp)
{
	this->f_startStamp = f_startStamp;
	setActive();
	setNotDying();
}

void Request::setActive()
{
	this->p_Emitter->setActive();
}

void Request::setDead()
{
	this->p_Emitter->setDead();
}

bool Request::isActive()
{
	return this->p_Emitter->isActive();
}

void Request::setDying()
{
	this->p_Emitter->setDying();
}

void Request::setNotDying()
{
	this->p_Emitter->setNotDying();
}

bool Request::isDying()
{
	return this->p_Emitter->isDying();
}

bool Request::isConstant()
{
	return p_Emitter->b_constant;
}

void Request::update()
{
	this->p_Emitter->update(p_spaceObj);
}

void Request::render(Camera * p_cam)
{
	this->p_Emitter->renderEffect(p_cam, p_spaceObj);
}

int Request::getTTL()
{
	return this->p_Emitter->getTTL();
}

int Request::getStartStamp()
{
	return this->f_startStamp;
}

void Request::setStartStamp(int f_startStamp)
{
	this->f_startStamp = f_startStamp;
}

bool Request::hasLivingParts()
{
	return p_Emitter->hasLivingParts();
}