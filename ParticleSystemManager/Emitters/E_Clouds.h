#pragma once
#include "EmitCont.h"

class E_Clouds : public EmitCont
{
	public:
		E_Clouds(DWORD * globalTime);

		//functions
		inline float getRandSize(){return ( (float)(rand()%500+500)/ 1000);}


		void initParticles(D3DXVECTOR3 v_color, int f_partTTL,
						   float f_initPSize, float f_initAlpha);
		void update(SpaceObject * p_spaceObj);
		void decayAlpha();
};