#pragma once
#include "EmitCont.h"

class E_Projectile_Type_1_lvl_1 : public EmitCont
{
	public:
		E_Projectile_Type_1_lvl_1(DWORD * globalTime);
		void initParticles(D3DXVECTOR3 v_color, int f_partTTL,
						   float f_initPSize, float f_initAlpha);
		void update(SpaceObject * p_spaceObj);
		void decayAlpha();
};