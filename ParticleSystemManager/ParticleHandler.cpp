#include "ParticleHandler.h"

ParticleHandler::ParticleHandler(DWORD * globalTime)
{
	this->globalTime = globalTime;
}

ParticleHandler::~ParticleHandler()
{
	for(unsigned int i = 0; i < v_Dead.size(); i++ )
	{	if(v_Dead.at(i))
			delete v_Dead.at(i);
	}
	for(unsigned int i = 0; i < v_Alive.size(); i++)
	{	
		if(v_Alive.at(i))
			delete v_Alive.at(i);
	}
}

void ParticleHandler::initHandler(int i_numParticles)
{
	this->i_numParticles = i_numParticles;

	Particle * p_newPart;
	for(int i = 0; i < i_numParticles; i++ )
	{
		p_newPart = new Particle();
		v_Dead.push_back(p_newPart);
	}
}

/*
void ParticleHandler::initParticles(D3DXMATRIX m_World, D3DXVECTOR4 v_color, 
									float f_partTTL)
{
	D3DXVECTOR3 v_position (0.0f, 0.0f, 0.0f);

	for(int i = 0; i<i_numParticles; i++)
	{
		v_Dead.at(i)->setTTL(f_partTTL);
		v_Dead.at(i)->setColor(D3DCOLOR_COLORVALUE(v_color.x, v_color.y, v_color.z, v_color.w));
		v_Dead.at(i)->setPosition(v_position);


		// SET UP INITIAL VELOCITY, DIRECTION and SPREAD
		float f_vecX = ((float)rand()/ RAND_MAX);
		float f_vecY = ((float)rand()/ RAND_MAX);
		float f_vecZ = ((float)rand()/ RAND_MAX);
		
		v_Dead.at(i)->setVelocity(D3DXVECTOR3(f_vecX, f_vecY, f_vecZ));
		//v_Dead.at(i)->setVelocity(D3DXVECTOR3(0, 0, f_vecZ));
		//v_Dead.at(i)->setVelocity(D3DXVECTOR3(0, 0, .7));
		D3DXMATRIX m_Reorient;
		m_Reorient = m_World;
		m_Reorient._41 = 0.0f;
		m_Reorient._42 = 0.0f;
		m_Reorient._43 = 0.0f;
				
		D3DXVec3TransformCoord(v_Dead.at(i)->getVelocityPtr(), v_Dead.at(i)->getVelocityPtr(), &m_Reorient);
		D3DXVec3TransformCoord(v_Dead.at(i)->getPositionPtr(), v_Dead.at(i)->getPositionPtr(), &m_World);
	}
}
*/

void ParticleHandler::emit(int i_emitQty, SpaceObject * p_spaceObj)
{
	if(i_emitQty != 0)
	{
		for(int i = 0; i < i_emitQty; i++)
		{
			if(v_Dead.size() != 0)
			{
				Particle * p_availParticle = v_Dead.front();
		
				v_Dead.erase(v_Dead.begin());

				p_availParticle->setStartTime(*globalTime);
				p_availParticle->setOwnerWorld(p_spaceObj->matTransform);

				v_Alive.push_back(p_availParticle);
			}
		}
	}
}

void ParticleHandler::checkDead()
{
	int f_curTime = *globalTime;
	Particle * p_curParticle;
	
	if(v_Alive.size() == 0)
		return;

	for(unsigned int i = 0; i< v_Alive.size(); i++)
	{
		p_curParticle = v_Alive.at(i);
		
		if(!p_curParticle)
			continue;

		if(p_curParticle->getElapsedTime(f_curTime) > p_curParticle->getTTL())
		{
			v_Alive.erase(v_Alive.begin() + i);
		
			p_curParticle->setStartTime(0);
			p_curParticle->initAlpha();
			p_curParticle->initPartSize();

			v_Dead.push_back(p_curParticle);
		}
	}
}



D3DXVECTOR3 ParticleHandler::getActivePartPosition(int i_idx)
{
	return v_Alive.at(i_idx)->getPosition();	
}

D3DXVECTOR3 ParticleHandler::getActivePartVelocity(int i_idx)
{
	return v_Alive.at(i_idx)->getVelocity();	
}

D3DXVECTOR3 ParticleHandler::getActivePartColor(int i_idx)
{
	return v_Alive.at(i_idx)->getColor();	
}

int ParticleHandler::getActivePartElapsedTime(int i_idx)
{
	return v_Alive.at(i_idx)->getElapsedTime(*globalTime);
}

int ParticleHandler::getActivePartTTL(int i_idx)
{
	return v_Alive.at(i_idx)->getTTL();
}


void ParticleHandler::setActivePartAlpha(int i_idx, float f_alpha)
{
	v_Alive.at(i_idx)->setAlpha(f_alpha);
}

float ParticleHandler::getActivePartAlpha(int i_idx)
{
	return v_Alive.at(i_idx)->getAlpha();
}

void ParticleHandler::setActivePartSize(int i_idx, float f_psize)
{
	v_Alive.at(i_idx)->setPartSize(f_psize);
}

float ParticleHandler::getActivePartSize(int i_idx)
{
	return v_Alive.at(i_idx)->getPartSize();
}

float ParticleHandler::getActiveInitPartSize(int i_idx)
{
	return v_Alive.at(i_idx)->getInitPartSize();
}

size_t ParticleHandler::getAliveSize()
{
	return v_Alive.size();
}

Particle * ParticleHandler::getDeadPart(int i_idx)
{
	return v_Dead.at(i_idx);
}

D3DXMATRIX ParticleHandler::getActivePartMatrix(int i_idx)
{
	return v_Alive.at(i_idx)->getOwnerWorld();
}