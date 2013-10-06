#include "E_Explosion_Cluster.h"

E_Explosion_Cluster::E_Explosion_Cluster(DWORD * globalTime)
{
	this->setCTime(globalTime);
	this->b_constant = false;
}

void E_Explosion_Cluster::initParticles(D3DXVECTOR3 v_color, int f_partTTL, 
							 float f_initPSize, float f_initAlpha)
{
	D3DXVECTOR3 v_position (0.0f, 0.0f, 0.0f);
	for(int i = 0; i<i_numParticles; i++)
	{
		p_partHandler->getDeadPart(i)->setTTL(f_partTTL);
		p_partHandler->getDeadPart(i)->setColor(D3DXVECTOR3(v_color.x, v_color.y, v_color.z));
		p_partHandler->getDeadPart(i)->setInitPartSize(f_initPSize);
		p_partHandler->getDeadPart(i)->setInitAlpha(f_initAlpha);

		v_position.x = getFullRand() * f_emitDisp;
		v_position.y = getFullRand() * f_emitDisp;
		v_position.z = 0.0;

		p_partHandler->getDeadPart(i)->setPosition(v_position);

		// SET UP INITIAL VELOCITY, DIRECTION and SPREAD
		float f_velX = getFullRand() * sinf(v_emitSpread.x);
		float f_velY = getFullRand() * sinf(v_emitSpread.y);
		float f_velZ = getPosRand();
		
		D3DXVECTOR3 v_velocity(f_velX, f_velY, f_velZ);
		D3DXVec3Normalize(&v_velocity, &v_velocity);


		p_partHandler->getDeadPart(i)->setVelocity(f_emitSpeed * v_velocity);

		D3DXMATRIX m_Reorient;
		m_Reorient = m_OrientAndPos; m_Reorient._41 = 0.0f; m_Reorient._42 = 0.0f; m_Reorient._43 = 0.0f;		
		D3DXVec3TransformCoord(p_partHandler->getDeadPart(i)->getVelocityPtr(), p_partHandler->getDeadPart(i)->getVelocityPtr(), &m_Reorient);
		D3DXVec3TransformCoord(p_partHandler->getDeadPart(i)->getPositionPtr(), p_partHandler->getDeadPart(i)->getPositionPtr(), &m_OrientAndPos);
	}
}

void E_Explosion_Cluster::update(SpaceObject * p_spaceObj)
{
	//check on who should die
	p_partHandler->checkDead();

	int f_curTime = *globalTime;
	int f_elapsedTime = f_curTime - f_startTime;

	if( f_elapsedTime >= f_emitPeriod )
	{
		//giveBirth
		if(!isDying())
		{
			p_partHandler->emit(i_emitQty, p_spaceObj);
		}
		f_startTime = f_curTime;
	}

	decayAlpha();

	//unsigned int i = p_partHandler->getAliveSize();
	if(p_partHandler->getAliveSize() != 0)
		FillVertexBuffer();
}

void E_Explosion_Cluster::decayAlpha()
{
	for(unsigned int i = 0; i < p_partHandler->getAliveSize(); i++)
	{
		int f_elapsedTime = p_partHandler->getActivePartElapsedTime(i);
		
		int f_ttl = p_partHandler->getActivePartTTL(i);
		float f_alpha = 1.0f - (((float)f_elapsedTime)/f_ttl);
		p_partHandler->setActivePartAlpha(i, f_alpha);
		if(f_alpha < 0.01f)
			p_partHandler->setActivePartAlpha(i, 0.01f);
		
		
		/*int i_elapsedTime = p_partHandler->getActivePartElapsedTime(i);
		int i_ttl = p_partHandler->getActivePartTTL(i);
		float f_threshold = ((float)(i_ttl * 4))/5;
		if(i_elapsedTime > f_threshold)	
		{
			float f_decayAmt = ((float)(i_elapsedTime-f_threshold))/(i_ttl-f_threshold);
			float f_alpha = 1.0f - f_decayAmt;
			p_partHandler->setActivePartAlpha(i, f_alpha);
			if(f_alpha < 0.01f)
				p_partHandler->setActivePartAlpha(i, 0.01f);
		}*/
		//float f_psize = p_partHandler->getActiveInitPartSize(i);
		//p_partHandler->setActivePartSize(i, f_psize*f_alpha);
	}
} 