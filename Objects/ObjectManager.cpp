#include "ObjectManager.h"
#include "ObjectManager.h"

//#define numWorld 1
//#define numBase 16
//#define numPlayerShip 4
//#define numAIShip 15
//#define numAsteroid 15
//#define numProjectile 15


//================================
//Class::ObjectManager
//================================
ObjectManager::ObjectManager()
{
	draw = 0;

	//config manager
	cManager = new ConfigManager();
    config = cManager->buildObjManager();

	numProjectile	= 80;
	numWorld		= 1;
	numBase			= 7;
	numPlayerShip	= 4;
	numAIShip		= 14;
	numAsteroid		= 15;
	numItem			= 7;

	totalSharedMemSize = 0;

	InitPublicMem();
	InitPrivateMem();
}


ObjectManager::~ObjectManager()
{
	delete privStartPlayerShip;
	delete privStartAIShip;
	delete privStartAsteroid;
	delete privStartBase;
	delete privStartProjectile;
	delete privStartWorld;
	delete privStartItem;
	free(m_PublicMem);

	//graphics
	delete skyBox;
}

MaryMaid* ObjectManager::getMary()
{
	return mary;
}

void ObjectManager::InitPrivateMem()
{
	privStartPlayerShip = new PlayerShip [numPlayerShip];

	//assign all the priv ships to pub ships
	for(int i = 0; i< numPlayerShip; i++)
	{
		privStartPlayerShip[i].sharedInfo =
			(SharedInfoPlayerShip *)((char *)pubStartPlayerShip + ( i* sizeof(SharedInfoPlayerShip)));
		privStartPlayerShip[i].index = i;
		privStartPlayerShip[i].shipFlag = 0;

	}

	privStartAIShip = new AIShip [numAIShip];
	//assign all the priv AI ships to pub ships
	for(int i = 0; i< numAIShip; i++)
	{
		privStartAIShip[i].sharedInfo =
			(SharedInfoAIShip *)((char *)pubStartAIShip + ( i* sizeof(SharedInfoAIShip)));
		privStartAIShip[i].index = i;
	}


	privStartAsteroid = new Asteroid [numAsteroid];
	//assign all the priv ships to pub ships
	for(int i = 0; i<numAsteroid ; i++)
	{
		privStartAsteroid[i].sharedInfo =
			(SharedInfoAsteroid *)((char *)pubStartAsteroid + ( i* sizeof(SharedInfoAsteroid)));
		privStartAsteroid[i].index = i;
	}

	privStartBase = new Base [numBase];
	//assign all the priv ships to pub ships
	for(int i = 0; i< numBase; i++)
	{
		privStartBase[i].sharedInfo = 
			(SharedInfoBase *)((char *)pubStartBase + ( i* sizeof(SharedInfoBase)));
		privStartBase[i].index = i;
	}

	privStartProjectile = new Projectile [numProjectile];
	//assign all the priv ships to pub ships
	for(int i = 0; i< numProjectile; i++)
	{
		privStartProjectile[i].sharedInfo =
			(SharedInfoProjectile *)((char *)pubStartProjectile + ( i* sizeof(SharedInfoProjectile)));
		privStartProjectile[i].index = i;
		privStartProjectile[i].projFlag = 0;
		privStartProjectile[i].colorDiffuse = D3DXVECTOR4(1,1,1,1);
	}

	privStartWorld = new World [numWorld];
	//assign all the priv ships to pub ships
	for(int i = 0; i<numWorld ; i++)
	{
		privStartWorld[i].sharedInfo =
			(SharedInfoWorld *)((char *)pubStartWorld + ( i* sizeof(SharedInfoWorld)));
		privStartWorld[i].index = i;
	}

	privStartItem = new Item [numItem];
	//assign all the priv ships to pub ships
	for(int i = 0; i< numItem; i++)
	{
		privStartItem[i].sharedInfo = 
			(SharedInfoItem *)((char *)pubStartItem + ( i* sizeof(SharedInfoItem)));
		privStartItem[i].index = i;
	}
}		

void ObjectManager::InitPublicMem()
{

		totalSharedMemSize =							  sizeof(MaryMaid) 		
														+ (sizeof(SharedInfoWorld) * numWorld)
														+ (sizeof(SharedInfoBase) * numBase)
														+ (sizeof(SharedInfoPlayerShip)* numPlayerShip)
														+ (sizeof(SharedInfoAIShip)* numAIShip)		
														+ (sizeof(SharedInfoAsteroid)* numAsteroid)		
														+ (sizeof(SharedInfoProjectile)* numProjectile)
														+ (sizeof(SharedInfoItem)* numItem);

	//	printf("mary %d\n", sizeof(Marymaid));

		
		m_PublicMem = (SharedInfoSpaceObject *)malloc (totalSharedMemSize);
		memset(m_PublicMem, 0 ,totalSharedMemSize);

		//mary had a little lamb, it's fleece was white as snow and everywhere that mary went, her sheep was sure to go
		//casting to (char*) because we need to add the address and not by its scale, the size of its content.
		// ie.  int * a; a+1 inceases the address in a by 4 not 1.
		mary				= (MaryMaid*) m_PublicMem;
		pubStartWorld		= (SharedInfoWorld*)((char*)mary+ sizeof(MaryMaid));
		pubStartBase		= (SharedInfoBase*)((char*)pubStartWorld +  sizeof(SharedInfoWorld) * numWorld);	
		pubStartPlayerShip  = (SharedInfoPlayerShip*)((char*)pubStartBase + sizeof(SharedInfoBase) * numBase);
		pubStartAIShip		= (SharedInfoAIShip*)((char*)pubStartPlayerShip  + sizeof(SharedInfoPlayerShip)* numPlayerShip);
		pubStartAsteroid	= (SharedInfoAsteroid*)((char*)pubStartAIShip + sizeof(SharedInfoAIShip)* numAIShip);		
		pubStartProjectile	= (SharedInfoProjectile*)((char*)pubStartAsteroid + sizeof(SharedInfoAsteroid)* numAsteroid);	
		pubStartItem		= (SharedInfoItem*)((char*)pubStartProjectile + sizeof(SharedInfoProjectile)* numProjectile);	
}


void ObjectManager::initGraphics(GraphicsManager * gManager)
{
	D3DXVECTOR4 color;

	//init mary
	////////////////////////////////////////////////////////////////////////
	mary->worldStart		= 0;
	mary->playerShipStart	= 0;
	mary->projectileStart	= -1;		
	mary->	AIShipStart		= -1;
	mary->	asteroidStart	= -1;
	mary->	baseStart		= -1;
	mary->	projectileStart	= -1;
	mary->	itemStart		= -1;
	

	//init world
	////////////////////////////////////////////////////////////////////////	
	color = D3DXVECTOR4(1,1,1,1);
	privStartWorld[0].sharedInfo->next=-1;
	privStartWorld[0].sharedInfo->pre=-1;
	privStartWorld[0].initObject(gManager->getMesh(config->worldMesh),NULL, color);
	privStartWorld[0].cubeTexture = gManager->getCubeTexture(config->worldIrraTexture); 
	privStartWorld[0].colorMap	  = gManager->getTexture(config->worldColorMap);
	privStartWorld[0].normalMap	  = gManager->getTexture(config->worldNormalMap); 
	
	D3DXMatrixIdentity(&privStartWorld[0].sharedInfo->matTransform );

	//Bases
	///////////////////////////////////////////////////////////////////////
	for (int i = 0; i < numBase; i++)
	{				
		privStartBase[i].sharedInfo->next	=-1;
		privStartBase[i].sharedInfo->pre	=-1;
		privStartBase[i].initObject(gManager->getMesh(config->bases.at(i)->mesh),
									gManager->getTexture(config->bases.at(i)->texture),
									config->bases.at(i)->color);
	}

	//init player
	////////////////////////////////////////////////////////////////////////	
	for (int i = 0; i < numPlayerShip; i++)
	{				
		privStartPlayerShip[i].sharedInfo->next=-1;
		privStartPlayerShip[i].sharedInfo->pre=-1;
		privStartPlayerShip[i].initObject(gManager->getMesh(config->players.at(i)->mesh),
											gManager->getTexture(config->players.at(i)->texture),
											config->players.at(i)->color);

		//privStartPlayerShip[i].UpdatePosition(D3DXVECTOR2(0,0));
		//privStartPlayerShip[i].cam = new Camera(800, 600, D3DX_PI/4, -45);
	}
	privStartPlayerShip[0].sharedInfo->next= 1;
	privStartPlayerShip[1].sharedInfo->next=-1;

	//int skybox
	////////////////////////////////////////////////////////////////////////		
	skyBox = new SkyBox();
	skyBox->initObject(gManager->getMesh(config->skyboxMesh), 
		//gManager->getCubeTexture("..\\Media\\Textures\\SkyBox\\Grace_cube.dds"),				
		gManager->getCubeTexture(config->skyboxCubeTexture),
						D3DXVECTOR3 (config->skyboxScaleX,
										config->skyboxScaleY,
										config->skyboxScaleZ)
						);

	//init projectiles
	////////////////////////////////////////////////////////////////////////
	for(int i = 0; i< numProjectile; i++)
	{
		privStartProjectile[i].sharedInfo->next = -1;
		privStartProjectile[i].sharedInfo->pre	= -1;
		privStartProjectile[i].initObject(gManager->getMesh("..\\Media\\Models\\cube.X"),
											NULL, color);

	}


	//Ocean
	///////////////////////////////////////////////////////////////////////
	ocean = new Ocean();
	ocean->initObject(gManager->getMesh("..\\Media\\Models\\sphere.X"), NULL, D3DXVECTOR3(1.25,1.25,1.25) );
	ocean->cubeTexture = gManager->getCubeTexture(config->skyboxCubeTexture);
	//ocean->cubeTexture = gManager->getCubeTexture("..\\Media\\Textures\\SkyBox\\Grace_cube.dds");  	

	//Items
	///////////////////////////////////////////////////////////////////////
	for (int i = 0; i < numItem; i++)
	{				
		privStartItem[i].sharedInfo->next	=-1;
		privStartItem[i].sharedInfo->pre	=-1;
		//privStartItem[i].initObject(gManager->getMesh(config->bases.at(i)->mesh),
		//							gManager->getTexture(config->bases.at(i)->texture),
		//							config->bases.at(i)->color);
	}


	


	//clean up config manager
	delete config;
	delete cManager;
}


//Accessor functions
//////////////////////////////////
PlayerShip* ObjectManager::getPlayerShipList()
{
	if(mary ->playerShipStart == -1)
		return NULL;
	return &privStartPlayerShip[mary ->playerShipStart];
}

AIShip* ObjectManager::getAIShipList()
{
	if(mary ->AIShipStart == -1)
		return NULL;
	return &privStartAIShip[mary->AIShipStart];
}

Asteroid* ObjectManager::getAsteroidList()
{
	if(mary ->asteroidStart == -1)
		return NULL;
	return &privStartAsteroid[mary ->asteroidStart];
}

Base* ObjectManager::getBaseList()
{
	if(mary ->baseStart == -1)
		return NULL;
	return &privStartBase[mary->baseStart];
}

Projectile* ObjectManager::getProjectileList()
{
	if(mary ->projectileStart == -1)
		return NULL;

	return &privStartProjectile[mary ->projectileStart];
}

World* ObjectManager::getWorldpList()
{
	if(mary ->worldStart == -1)
		return NULL;

	return &privStartWorld[mary ->worldStart];
}

Item* ObjectManager::getItemList()
{
	if(mary ->itemStart == -1)
		return NULL;
	return &privStartItem[mary->itemStart];
}
