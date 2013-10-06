#include "EmitterHandler.h"

EmitterHandler::EmitterHandler()
{
	//Load configuration file
	cManager = new ConfigManager();
	config = cManager->buildParticleSystem();
}
EmitterHandler::~EmitterHandler()
{
	for(unsigned int i = 0; i < v_EmitterBank.size(); i++)
	{
		for (unsigned int j = 0; j < v_EmitterBank.at(i)->size(); j++)
		{
			delete v_EmitterBank.at(i)->at(j);
		}
		delete v_EmitterBank.at(i);
	}


	for(unsigned int i = 0; i < v_RequestBank.size(); i++)
		delete v_RequestBank.at(i);

	delete cManager;
	delete config;
}

void EmitterHandler::initHandler(LPDIRECT3DDEVICE9 p_Device, DWORD *globalTime, GraphicsManager * p_gManager)
{
	
	this->globalTime = globalTime;
	this->p_gManager = p_gManager;

	//1) Go into config and add your emitter setup into each
	//		line. 

	//2) Extend you emitter subclass from Emitter parent 
	//		(define 3 virtual functions)

	//3) Include the new class into emitterHandler.h

	//4) Add a case into the switch statement below
	//		based on the order you emitter is in the config.

	//5) Make a request to draw the emitter

	size_t emitTypeNum = config->TypeName.size();
	initTypeLookupTable(config->TypeName);

	Emitter * p_Emitter;
	int j = 0;
	float xSpread, ySpread;
	for(unsigned int i=0; i<emitTypeNum; i++)
	{
		std::vector <Emitter* > * temp = new std::vector <Emitter* >();
		v_EmitterBank.push_back(temp);
		int max = j + config->TypeQuantity.at(i);
		for(; j < max;j++)
		{
			switch(i)
			{
				case 0:
					p_Emitter = new E_Thrust(globalTime);
					break;
				case 1:
					p_Emitter = new E_Explosion_Cluster(globalTime);
					break;
				case 2:
					p_Emitter = new E_Projectile_Type_1_lvl_1(globalTime);
					break;
				case 3:
					p_Emitter = new E_Clouds(globalTime);
					break;
				default:
					p_Emitter = new E_Thrust(globalTime);
					break;
			}

			xSpread = config->emitSpread.at(i).x; 
			ySpread = config->emitSpread.at(i).y; 

			if(xSpread == 0.0f)
				config->emitSpread.at(i).x = 0.0f;
			else
				config->emitSpread.at(i).x = D3DX_PI/xSpread;

			if(ySpread == 0.0f)
				config->emitSpread.at(i).y = 0.0f;
			else
				config->emitSpread.at(i).y = D3DX_PI/ySpread;

			createEmitter(p_Device, p_Emitter, 
						  config->EmitPos.at(i), config->EmitDir.at(i), 
						  config->EmitColor.at(i), config->TexFileName.at(i), 
						  config->EffectFileName.at(i), config->PartNum.at(i), 
						  config->emitTTL.at(i), config->partTTL.at(i), 
						  config->emitPeriod.at(i), config->emitQty.at(i), 
						  config->emitSpeed.at(i), config->emitSpread.at(i), 
						  config->emitDisp.at(i), config->initPSize.at(i), 
						  config->initAlpha.at(i), config->ColorMapFileName.at(i));

			v_EmitterBank.at(i)->push_back(p_Emitter);	//add this emitter to the vector
		}
	}
	
}

void EmitterHandler::createEmitter( 
					LPDIRECT3DDEVICE9 p_Device, Emitter * p_Emitter, 
					D3DXVECTOR3 v_position, D3DXVECTOR3 v_dir,
					D3DXVECTOR3 v_color, char * c_textureName,
					char * c_effectName, int i_numParticles, 
					int f_emitTTL, int f_partTTL,
					int f_emitPeriod, int i_emitQty,
					float f_emitSpeed, D3DXVECTOR2 v_emitSpread,
					float f_emitDisp, float f_initPSize,
					float f_initAlpha, char * c_colorMap)
{
	
	p_Emitter->initEmitter(p_Device, i_numParticles, 
						   f_emitTTL,
						   f_emitPeriod, i_emitQty,
						   f_emitSpeed, v_emitSpread,
						   f_emitDisp, p_gManager->getTexture(c_textureName),
						   p_gManager->getEffect(c_effectName),
						   p_gManager->getTexture(c_colorMap));

	p_Emitter->initWorld(v_position, v_dir);

	p_Emitter->initParticles(v_color, f_partTTL, f_initPSize, f_initAlpha);
}

void EmitterHandler::initTypeLookupTable(std::vector<char*> v_TypeNames)
{
	for(unsigned int i = 0; i < v_TypeNames.size(); i++)
	{
		typeLT[v_TypeNames.at(i)] = i;
	}
}

int EmitterHandler::findTypeIndex(char * emitterTag)
{
	if(typeLT.find(emitterTag) != typeLT.end())
		return typeLT[emitterTag];
	return -1;
}

Request * EmitterHandler::activateEmitter(char * emitterTag, SpaceObject * p_spaceObj)
{
	int i_typeIndex = findTypeIndex(emitterTag);
	if(i_typeIndex == -1)
	{
		printf("Wrong Emitter Tag\n");
		return NULL;
	}
	
	//printf("Size of emitter %d:\n",emitterTag );

	Emitter * p_available = GetAvailableEmitter(i_typeIndex);
	if(p_available == NULL)
	{
		printf("No available emitters were found (%s)\n", emitterTag);
		return NULL;
	}

	Request * p_newRequest = new Request(p_available, p_spaceObj);

	p_newRequest->Activate(*globalTime);

	v_RequestBank.push_back(p_newRequest);
	
	return p_newRequest;
}

Emitter * EmitterHandler::GetAvailableEmitter(int i_typeIndex)
{
	Emitter * p_availableEmitter = NULL;
	
	for(unsigned int i = 0; i < v_EmitterBank.at(i_typeIndex)->size(); i++)
		if(!v_EmitterBank.at(i_typeIndex)->at(i)->isActive())
			p_availableEmitter = v_EmitterBank.at(i_typeIndex)->at(i);
	return p_availableEmitter;
}

void EmitterHandler::update(void)
{
	for(unsigned int i = 0; i < v_RequestBank.size(); i++)
	{
		if(v_RequestBank[i]->isActive())
		{
			if(keepAlive(i))
				v_RequestBank[i]-> update();
		}
	}
}

void EmitterHandler::render(Camera * p_cam)
{
	for(unsigned int i = 0; i< v_RequestBank.size(); i++)
	{
		if(v_RequestBank[i]->isActive())
		{
			v_RequestBank[i]->render(p_cam);
			//v_RequestBank[i]->p_Emitter->renderFFP();
		}
	}
}

bool EmitterHandler::keepAlive(int i_idx)
{
	//get current time stamp
	int f_curStamp = *globalTime;

	Request * p_curRequest = v_RequestBank[i_idx];
	int f_startStamp = p_curRequest->getStartStamp();

	//check if request is constant
	if(p_curRequest->isConstant())
	{
		if(p_curRequest->isDying() && !p_curRequest->hasLivingParts())
		{
			//kick from request vector
			v_RequestBank.erase(v_RequestBank.begin()+i_idx);
			
			//set emitter to dead
			p_curRequest->setDead();

			//set as not dying
			p_curRequest->setNotDying();
		
			//delete request object
			delete p_curRequest;

			return false;
		}

		p_curRequest->setStartStamp(f_curStamp);

	}
	else
	{
		if((f_curStamp-f_startStamp) > p_curRequest->getTTL())
		{
			//set emitter as dying
			p_curRequest->setDying();

			//if there are no living particles
			if(!p_curRequest->hasLivingParts())
			{
				//kick from request vector
				v_RequestBank.erase(v_RequestBank.begin()+i_idx);
			
				//set emitter to dead
				p_curRequest->setDead();

				//set as not dying
				p_curRequest->setNotDying();
		
				//delete request object
				delete p_curRequest;

				return false;
			}
		}
	}
	return true;
}