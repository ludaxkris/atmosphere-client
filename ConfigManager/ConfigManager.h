#pragma once
#ifndef _CONFIGMANAGER_
#define _CONFIGMANAGER_

#include "./../Common/dxstdafx.h"
#include "tinyxml.h"
#include <string.h>
#include <vector>
#include <exception>


/** 
 * Object for the controller class. 
 */
class ControllerConfig
{
public:	
	ControllerConfig();
	~ControllerConfig();
	int loadOkay;
	std::vector <char *> stateNames;
	std::vector <int> stateMin;
	std::vector <int> stateMax;
};

/** 
 * Object for the Atmosphere game. 
 */
class AtmosphereConfig
{
public:	
	AtmosphereConfig();
	~AtmosphereConfig();
	int loadOkay;
	char * serverIP;
	char * serverPort;
	int screenSizeWidth;
	int screenSizeHeight;
	int joinType;
};


/**
 * Object used by ObjManagerConfig class, 
 * specifically for player, base, and AI.
 */
class ObjManagerObjects 
{
public:
	ObjManagerObjects();
	~ObjManagerObjects();
	D3DXVECTOR4 color;
	D3DXVECTOR3 position;
	float scaleX, scaleY, scaleZ;
	char * texture;
	char * mesh;
};


/** 
 * Object for the Object Manager Class. 
 */
class ObjManagerConfig
{
public:	
	ObjManagerConfig();
	~ObjManagerConfig();
	int loadOkay;

	std::vector <ObjManagerObjects *> players;
	std::vector <ObjManagerObjects *> bases;
	std::vector <ObjManagerObjects *> AI;

	//Used by world objects
	char * worldColorMap;
	char * worldNormalMap;
	char * worldIrraTexture;
	char * worldMesh;
	D3DXVECTOR3 worldPosition;
	float worldScaleX, worldScaleY, worldScaleZ;

	//Used by sky box
	float skyboxScaleX, skyboxScaleY, skyboxScaleZ;
	char * skyboxCubeTexture;
	char * skyboxMesh;

	ObjManagerObjects * loadPlayer(TiXmlElement * player);
	ObjManagerObjects * loadBase(TiXmlElement * base);
	ObjManagerObjects * loadAI(TiXmlElement * AI);
};

/** 
 * Object for the Particle System Manager Class. 
 */
class ParticleSystemConfig
{
public:	
	ParticleSystemConfig();
	~ParticleSystemConfig();
	int loadOkay;

	std::vector<char*> TypeName;
	std::vector<int> TypeQuantity;
	std::vector<D3DXVECTOR3> EmitPos;
	std::vector<D3DXVECTOR3> EmitDir;
	std::vector<D3DXVECTOR3> EmitColor;
	std::vector<char*> TexFileName;
	std::vector<char*> ColorMapFileName;
	std::vector<char*> EffectFileName;
	std::vector<int> PartNum;
	std::vector<int> emitTTL;
	std::vector<int> partTTL;
	std::vector<int> emitPeriod;
	std::vector<int> emitQty;
	std::vector<float> emitSpeed;
	std::vector<D3DXVECTOR2> emitSpread;
	std::vector<float> emitDisp;
	std::vector<float> initPSize;
	std::vector<float> initAlpha;

	void loadCharVector(std::vector <char*> * vec, TiXmlElement * rootNode, char * nodeName);
	void loadIntVector(std::vector <int> * vec, TiXmlElement * rootNode, char * nodeName);
	void loadD3Vector(std::vector <D3DXVECTOR3> * vec, TiXmlElement * rootNode, char * nodeName);
	void loadFloatVector(std::vector <float> * vec, TiXmlElement * rootNode, char * nodeName);
	
};

class ConfigManager
{
public:
	ConfigManager();
	
	//Functions called by other components.
	ControllerConfig * buildController();
	AtmosphereConfig * buildAtmosphere();
	ObjManagerConfig * buildObjManager();	
	ParticleSystemConfig * buildParticleSystem();

	//File names
	char * controllerXML;
	char * atmosphereXML;
	char * objManagerXML;
	char * particleSystemXML;
};


#endif; /* _CONFIGMANAGER_ */