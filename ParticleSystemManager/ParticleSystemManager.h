#pragma once
#include "./../Common/dxstdafx.h"
#include <vector>
#include <string>
#include <map>

#include "./../GraphicsManager/GraphicsManager.h"
#include "EmitterHandler.h"
#include "Emitter.h"
#include "./Emitters/EmitCont.h"
#include "./Emitters/EmitBurst.h"
#include "Particle.h"

class Particle;
//class Emitter;

class ParticleSystemManager
{
	public:

		ParticleSystemManager(LPDIRECT3DDEVICE9 p_Device, DWORD *globalTime, GraphicsManager * p_gManager);
		~ParticleSystemManager(void);

		void initPSM(void);			//init particle manager
		
		void makePSMRequest(char * emitName, SpaceObject * p_spaceObj, int ID);
		void removePSMRequest(int ID);

		void update(void);				//update position of the emitters
		void render(Camera * p_cam);	//render particles with the emitter

	private:
		LPDIRECT3DDEVICE9 p_PSMDevice;
		DWORD *globalTime;
		EmitterHandler * p_EmitHandler;
		GraphicsManager * p_gManager;

		std::map<int, Request *> RLT;

		//std::vector<Request *> activeEmitters;
		//std::vector<Emitter *> vep_emitterBank;
};