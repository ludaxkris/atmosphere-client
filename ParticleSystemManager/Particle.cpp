#include "Particle.h"

Particle::Particle()
{
	this->f_startTime = 0;
	D3DXMatrixIdentity(&m_OwnerWorld);
}

Particle::~Particle(void){}

void Particle::setPosition(D3DXVECTOR3 v_newPos)
{
	this->v_CurPos = v_newPos;
}

D3DXVECTOR3 Particle::getPosition()
{
	return this->v_CurPos;
}

D3DXVECTOR3 * Particle::getPositionPtr()
{
	return &(this->v_CurPos);
}

void Particle::setVelocity(D3DXVECTOR3 v_newVel)
{
	this->v_CurVel = v_newVel;
}

D3DXVECTOR3 Particle::getVelocity()
{
	return this->v_CurVel;
}

D3DXVECTOR3 * Particle::getVelocityPtr()
{
	return &(this->v_CurVel);
}

void Particle::setColor(D3DXVECTOR3 v_newColor)
{
	this->v_Color = v_newColor;
}

D3DXVECTOR3 Particle::getColor()
{
	return this->v_Color;
}

void Particle::setTTL(int f_partTTL)
{
	this->f_partTTL = f_partTTL;
}
	
int Particle::getTTL()
{
	return f_partTTL;
}

void Particle::setStartTime(int f_startTime)
{
	this->f_startTime = f_startTime;
}

int Particle::getElapsedTime(int f_curTime)
{
	//problem
	return (f_curTime - f_startTime);
}

void Particle::initAlpha()
{
	this->f_alpha = f_alphaInit;
}

void Particle::setAlpha(float f_alpha)
{
	this->f_alpha = f_alpha;
}

void Particle::setInitAlpha(float f_alphaInit)
{
	this->f_alphaInit = f_alphaInit;
	this->f_alpha = f_alphaInit;
}

float Particle::getAlpha()
{
	return this->f_alpha;
}

float Particle::getInitAlpha()
{
	return this->f_alphaInit;
}

void Particle::initPartSize()
{
	this->f_psize = f_psizeInit;
}

void Particle::setPartSize(float f_psize)
{
	this->f_psize = f_psize;
}

void Particle::setInitPartSize(float f_psizeInit)
{
	this->f_psizeInit = f_psizeInit;
	this->f_psize = f_psizeInit;
}

float Particle::getPartSize()
{
	return this->f_psize;
}

float Particle::getInitPartSize()
{
	return this->f_psizeInit;
}

D3DXMATRIX Particle::getOwnerWorld()
{
	//problem
	return this->m_OwnerWorld;
}

void Particle::setOwnerWorld(D3DXMATRIX m_OwnerWorld)
{
	D3DXVECTOR3 temp;

	temp =  *((D3DXVECTOR3 *)&(m_OwnerWorld._11));
	D3DXVec3Normalize(&temp, &temp);
	*((D3DXVECTOR3 *)&(m_OwnerWorld._11)) = temp;

	temp =  *((D3DXVECTOR3 *)&(m_OwnerWorld._21));
	D3DXVec3Normalize(&temp, &temp);
	*((D3DXVECTOR3 *)&(m_OwnerWorld._21)) = temp;

	temp =  *((D3DXVECTOR3 *)&(m_OwnerWorld._31));
	D3DXVec3Normalize(&temp, &temp);
	*((D3DXVECTOR3 *)&(m_OwnerWorld._31)) = temp;

	this->m_OwnerWorld = m_OwnerWorld;
}