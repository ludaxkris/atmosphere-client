#include "E_Clouds.h"

E_Clouds::E_Clouds(DWORD * globalTime)
{
	this->setCTime(globalTime);
	this->b_constant = true;
}


void E_Clouds::initParticles(D3DXVECTOR3 v_color, int f_partTTL, 
							 float f_initPSize, float f_initAlpha)
{
	int cloudsPerGroup = 10;
	D3DXVECTOR3 v_position (0.0f, 0.0f, 0.0f);
	for(int j = 0; j < 30; j++)
	{
		D3DXVECTOR3 dir = D3DXVECTOR3(getFullRand()*5,getFullRand()*5,getFullRand()*5);
		initWorld(D3DXVECTOR3(0,0,0), dir);	
		for(int i = 0; i<cloudsPerGroup; i++)
		{
			p_partHandler->getDeadPart(i+(j*cloudsPerGroup))->setTTL(f_partTTL);
			p_partHandler->getDeadPart(i+(j*cloudsPerGroup))->setColor(D3DXVECTOR3(v_color.x, v_color.y, v_color.z));
			p_partHandler->getDeadPart(i+(j*cloudsPerGroup))->setInitPartSize(getRandSize()*150);
			p_partHandler->getDeadPart(i+(j*cloudsPerGroup))->setInitAlpha(0.12f);

			v_position.x = getFullRand() * 5;
			v_position.y = getFullRand() * 5;
			v_position.z = -23;

			if(i+(j*cloudsPerGroup) == 0)
			{
				p_partHandler->getDeadPart(i+(j*cloudsPerGroup))->setInitPartSize(1000);
				v_position.x = 0;
				v_position.y = 0;
				v_position.z = 0;
				p_partHandler->getDeadPart(i+(j*cloudsPerGroup))->setInitAlpha(0.95f);

			}

			p_partHandler->getDeadPart(i+(j*cloudsPerGroup))->setPosition(v_position);

			// SET UP INITIAL VELOCITY, DIRECTION and SPREAD
			
			float f_velX = 0.0;
			float f_velY = 0.0;
			float f_velZ = 0.0;

			D3DXVECTOR3 v_velocity(f_velX, f_velY, f_velZ);
			D3DXVec3Normalize(&v_velocity, &v_velocity);


			p_partHandler->getDeadPart(i+(j*cloudsPerGroup))->setVelocity(f_emitSpeed * v_velocity);
			/*printf("Before %f, %f, %f\n",p_partHandler->getDeadPart(i)->getVelocity().x,
						p_partHandler->getDeadPart(i)->getVelocity().y,
						p_partHandler->getDeadPart(i)->getVelocity().z);*/

			D3DXMATRIX m_Reorient;
			m_Reorient = m_OrientAndPos; m_Reorient._41 = 0.0f; m_Reorient._42 = 0.0f; m_Reorient._43 = 0.0f;
			D3DXVec3TransformCoord(p_partHandler->getDeadPart(i+(j*cloudsPerGroup))->getVelocityPtr(),
									p_partHandler->getDeadPart(i+(j*cloudsPerGroup))->getVelocityPtr(), &m_Reorient);
			D3DXVec3TransformCoord(p_partHandler->getDeadPart(i+(j*cloudsPerGroup))->getPositionPtr(),
									p_partHandler->getDeadPart(i+(j*cloudsPerGroup))->getPositionPtr(), &m_OrientAndPos);

			/*printf("%f, %f, %f\n",p_partHandler->getDeadPart(i)->getVelocity().x,
						p_partHandler->getDeadPart(i)->getVelocity().y,
						p_partHandler->getDeadPart(i)->getVelocity().z);*/
		}
	}
}

void E_Clouds::update(SpaceObject * p_spaceObj)
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

	//decayAlpha();

	if(p_partHandler->getAliveSize() != 0)
		FillVertexBuffer();
}

void E_Clouds::decayAlpha()
{
	for(unsigned int i = 0; i < p_partHandler->getAliveSize(); i++)
	{
		int f_ttl = p_partHandler->getActivePartTTL(i);
		int f_elapsedTime = p_partHandler->getActivePartElapsedTime(i);
		float f_alpha = 1.0f - (((float)f_elapsedTime)/f_ttl);
		p_partHandler->setActivePartAlpha(i, f_alpha);
		
		float f_psize = p_partHandler->getActiveInitPartSize(i);
		p_partHandler->setActivePartSize(i, f_psize*f_alpha);
	}
} 
