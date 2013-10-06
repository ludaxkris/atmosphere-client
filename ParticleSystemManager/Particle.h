#pragma once
#include <d3d9.h>
#include <d3dx9tex.h>
#include <d3d9types.h>

class Particle
{
public:
	Particle();
	~Particle(void);


	void setPosition(D3DXVECTOR3 v_newPos);
	D3DXVECTOR3 getPosition();
	D3DXVECTOR3 * getPositionPtr();
	
	void setVelocity(D3DXVECTOR3 v_newVel);
	D3DXVECTOR3 getVelocity();
	D3DXVECTOR3 * getVelocityPtr();
	
	void setColor(D3DXVECTOR3 v_newColor);
	D3DXVECTOR3 getColor();

	void setTTL(int f_partTTL);
	int getTTL();

	void setStartTime(int f_startTime);
	int getElapsedTime(int f_curTime);

	void initAlpha();
	void setAlpha(float f_alpha);
	void setInitAlpha(float f_alphaInit);
	float getAlpha();
	float getInitAlpha();

	void initPartSize();
	void setPartSize(float f_Size);
	void setInitPartSize(float f_psizeInit);
	float getPartSize();
	float getInitPartSize();

	D3DXMATRIX getOwnerWorld();
	void setOwnerWorld(D3DXMATRIX m_OwnerWorld);

private:
	D3DXVECTOR3 v_CurPos;
	D3DXVECTOR3 v_Color;
	float f_psize;
	float f_alpha;

	D3DXVECTOR3 v_CurVel;
	
	int f_partTTL;
	int f_startTime;

	float f_alphaInit;
	float f_psizeInit;

	D3DXMATRIX m_OwnerWorld;
};