#pragma once
#include <vector>
#include <list>
#include <map>

#include "./../GraphicsManager/GraphicsManager.h"
#include "./../ConfigManager/ConfigManager.h"
#include "Request.h"
#include "./Emitters/EmitCont.h"
#include "./Emitters/E_Thrust.h"
#include "./Emitters/E_Projectile_Type_1_lvl_1.h"
#include "./Emitters/E_Clouds.h"

#include "./Emitters/EmitBurst.h"
#include "./Emitters/E_Explosion_Cluster.h"

class EmitterHandler
{
public:
	struct strCmp 
	{
		bool operator()( const char* s1, const char* s2 ) const 
		{
			return strcmp( s1, s2 ) < 0;
		}
	};

	EmitterHandler();
	~EmitterHandler();

	void initHandler(LPDIRECT3DDEVICE9 p_Device, DWORD *globalTime, GraphicsManager * p_gManager);

	//create new emitter
	void createEmitter(LPDIRECT3DDEVICE9 p_Device, 
					Emitter * p_Emitter, D3DXVECTOR3 v_position, 
					D3DXVECTOR3 v_dir, D3DXVECTOR3 v_color, 
					char * c_textureName, char * c_effectName, 
					int i_numParticles, int f_emitTTL,
					int f_partTTL, int f_emitPeriod,
					int i_emitQty, float f_emitSpeed,
					D3DXVECTOR2 v_emitSpread, float f_emitDisp,
					float f_initPSize, float f_initAlpha,
					char * c_colorMap);

	void initTypeLookupTable(std::vector<char*> v_TypeNames);
	int findTypeIndex(char * emitterTag);

	Request * activateEmitter(char * emitterTag, SpaceObject * p_spaceObj);
	Emitter * GetAvailableEmitter(int i_typeIndex);

	void update(void);
	void render(Camera * p_cam);

	bool keepAlive(int i_idx);

	//Config Manager
	ConfigManager *cManager;
	ParticleSystemConfig *config;

private:
	DWORD * globalTime;
	GraphicsManager * p_gManager;

	//std::map<char *, Effect *, strCmp> Effects;

	std::vector < std::vector <Emitter* > * > v_EmitterBank;

	std::vector<Request *> v_RequestBank;
	std::map<char *, int, strCmp>typeLT;
};