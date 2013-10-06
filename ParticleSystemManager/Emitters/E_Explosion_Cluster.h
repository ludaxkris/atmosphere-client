#pragma once
#include "EmitBurst.h"

class E_Explosion_Cluster : public EmitBurst
{
	public:
		E_Explosion_Cluster(DWORD * globalTime);
		void initParticles(D3DXVECTOR3 v_color, int f_partTTL,
						   float f_initPSize, float f_initAlpha);
		void update(SpaceObject * p_spaceObj);
		void decayAlpha();
};