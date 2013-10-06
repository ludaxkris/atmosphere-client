#pragma once

#include "SpaceObject.h"
#include "AIShip.h"
#include "Asteroid.h"
#include "PlayerShip.h"
#include "Base.h"
#include "Projectile.h"
#include "World.h"
#include "MaryMaid.h"
#include "SkyBox.h"
#include "Ocean.h"
#include "Item.h"
#include "../GraphicsManager/GraphicsManager.h"
#include "../ConfigManager/ConfigManager.h"

class ObjectManager
{

protected:

	//config
	ConfigManager * cManager;
    ObjManagerConfig * config;

	MaryMaid* mary;

	SharedInfoPlayerShip* pubStartPlayerShip;
	SharedInfoAIShip* pubStartAIShip;
	SharedInfoAsteroid* pubStartAsteroid;
	SharedInfoBase*	pubStartBase;
	SharedInfoProjectile* pubStartProjectile;
	SharedInfoWorld* pubStartWorld;
	SharedInfoItem* pubStartItem;

	void InitPrivateMem();	// new
	void InitPublicMem();	// malloc

public:

	//draw mark
	char draw;

	//Variables
	PlayerShip*		privStartPlayerShip;
	AIShip*			privStartAIShip;
	Asteroid*		privStartAsteroid;
	Base*			privStartBase;
	Projectile*		privStartProjectile;
	World*			privStartWorld;
	Item*			privStartItem;


	int numProjectile;
	int numWorld;
	int numBase;
	int numPlayerShip;
	int numAIShip;
	int numAsteroid;
	int numItem;

	int totalSharedMemSize;
	SharedInfoSpaceObject*	m_PublicMem;

	//Graphics
	SkyBox * skyBox;	
	Ocean * ocean;
	
	//Functions
	ObjectManager();
	~ObjectManager();

	void initGraphics(GraphicsManager * gManager);

	//Accessor Functions
	PlayerShip* getPlayerShipList();
	AIShip*		getAIShipList();
	Asteroid*	getAsteroidList();
	Base*		getBaseList();
	Projectile* getProjectileList();
	World*		getWorldpList();
	Item*		getItemList();

	MaryMaid*	getMary();
};
