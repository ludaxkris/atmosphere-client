#pragma once
#include "./../Common/dxstdafx.h"
#include "./../Objects/SpaceObject.h"
#include <vector>
#include "Particle.h"

class ParticleHandler
{
public:
	ParticleHandler(DWORD * globalTime);
	~ParticleHandler();

	void initHandler(int i_numParticles);
	//void initParticles(D3DXMATRIX m_World, D3DXVECTOR4 v_color, 
	//	               float f_partTTL);

	void emit(int i_emitQty, SpaceObject * p_spaceObj);
	void checkDead();

	D3DXVECTOR3 getActivePartPosition(int i_idx);
	D3DXVECTOR3 getActivePartVelocity(int i_idx);
	D3DXVECTOR3 getActivePartColor(int i_idx);

	int getActivePartElapsedTime(int i_idx);
	int getActivePartTTL(int i_idx);

	void setActivePartAlpha(int i_idx, float f_alpha);
	float getActivePartAlpha(int i_idx);

	void setActivePartSize(int i_idx, float f_psize);
	float getActivePartSize(int i_idx);
	float getActiveInitPartSize(int i_idx);

	size_t getAliveSize();
	Particle * getDeadPart(int i_idx);

	D3DXMATRIX getActivePartMatrix(int i_idx);


private:
	DWORD * globalTime;
	int i_numParticles;

	std::vector<Particle *> v_Dead;
	std::vector<Particle *> v_Alive;
};