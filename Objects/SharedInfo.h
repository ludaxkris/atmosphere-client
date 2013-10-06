#pragma once
#include <d3d9.h>
#include <d3dx9.h>

class SharedInfoSpaceObject
{
	public:	
	int next;
	int pre;
	D3DXMATRIX	matTransform;
};

class SharedInfoShip : public SharedInfoSpaceObject
{
public:
	int hp;
};

class SharedInfoPlayerShip : public SharedInfoShip
{
public:
	int playerFlag;
	int weaponType;
	int weaponLevel;
};

class SharedInfoAIShip : public SharedInfoShip
{
public:
	int AILevel;
	int baseID;
	char attkOrDef;
};

class SharedInfoWorld : public SharedInfoSpaceObject
{
};

class SharedInfoBase : public SharedInfoSpaceObject
{
public:

	//1 bit, base is first being attacked
	//2 bit, base is taken over

	char baseFlag;
	int AILevel;
	int playerID;
};

class SharedInfoProjectile : public SharedInfoSpaceObject
{
public:
	char projFlag;
	char shipID;
	char type;
};

class SharedInfoAsteroid : public SharedInfoSpaceObject
{
public:
	int hp;
};

class SharedInfoItem : public SharedInfoSpaceObject
{
	int itemType;
};