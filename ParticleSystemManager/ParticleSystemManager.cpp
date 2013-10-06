#include "ParticleSystemManager.h"


ParticleSystemManager::ParticleSystemManager(LPDIRECT3DDEVICE9 p_Device, DWORD *globalTime, GraphicsManager * p_gManager)
{
	this->p_PSMDevice = p_Device;
	this->globalTime = globalTime;
	this->p_EmitHandler = new EmitterHandler();
	this->p_gManager = p_gManager;
}

ParticleSystemManager::~ParticleSystemManager(void)
{
	this->p_PSMDevice = NULL;
	this->globalTime = NULL;
	delete this->p_EmitHandler;
}

void ParticleSystemManager::initPSM(void)
{
	p_EmitHandler->initHandler(p_PSMDevice, globalTime, p_gManager);
}

void ParticleSystemManager::makePSMRequest(char * emitName, SpaceObject * p_spaceObj, int ID)
{
	//check if the RLT contains the Request with the given ID
	//printf("RLTm: %d\n",RLT.size());
	std::map<int,Request *>::iterator iter;
	
	Request * p_newReq;

	//if yes, deactivate the request insert new one
	p_newReq = p_EmitHandler->activateEmitter(emitName, p_spaceObj);

	//Problem: *Change to ID!!*//
	if(p_newReq && p_newReq->isConstant())
	{

		removePSMRequest(ID);



		/*
		for( iter = RLT.begin(); iter != RLT.end(); iter++ ) 
		{
			//Problem:change index to request ID
			if(iter->first == ID)
			{
				//if it does remove it
				iter->second->setDying();
				RLT.erase(iter);
				break;
			}
		}
		*/
		RLT[ID] = p_newReq;
	}

	//printf("RLTm: %d\n",RLT.size());
}

void ParticleSystemManager::removePSMRequest(int ID)
{
	//check if the RLT contains the Request with the given ID
	std::map<int,Request *>::iterator iter; 

	Request * toDelete = RLT[ID];

	if(toDelete != NULL)
	{
		toDelete->setDying();
		RLT[ID] =NULL;
	}
/*
	for( iter = RLT.begin(); iter != RLT.end(); iter++ ) {
		//Problem: *Change to ID!!
		if(iter->first == ID)
		{
			//if it does remove it
			iter->second->setDying();
			RLT.erase(iter);
			break;
		}
	}*/

	//printf("RLTr: %d\n",RLT.size());
}


void ParticleSystemManager::update(void)
{
	p_EmitHandler->update();
}

void ParticleSystemManager::render(Camera * p_cam)
{
	p_EmitHandler->render(p_cam);
}
