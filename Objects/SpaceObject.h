#pragma once
#include <d3d9.h>
#include <d3dx9.h>
#include "../GraphicsManager/GraphicsManager.h"
#include "../GraphicsManager/MyEffect.h"
#include "SharedInfo.h"

#define SPEED_RATIO 0.05f

class SpaceObject
{
	int m_nMemIndex;

	public:

		//Everyone's stuff
		/////////////////////////////////////
		SpaceObject(void);
		/////////////////////////////////////

		//Arjay's system stuff
		////////////////////////////////////////////
		//bool to say if it's taken or not... do later
		int index;
		int getIndex(){return index;}
		void setIndex(int ind){index = ind;}

		virtual bool isAIShip(){return false;}
		virtual bool isPlayerShip(){return false;}
		virtual bool isAsteroid(){return false;}
		virtual bool isBase(){return false;}
		virtual bool isProjectile(){return false;}
		virtual bool isWorld(){return false;}
		virtual bool isItem(){return false;}
		////////////////////////////////////////////

		//Graphics stuff
		////////////////////////////////////////////
		LPD3DXMESH				mesh;					// Mesh 
		LPDIRECT3DTEXTURE9		texture;				// Mesh texture
		D3DXVECTOR4				colorDiffuse;			// diffuse color

		char meshFileName[256];
		char textureFileName[256];

		D3DXVECTOR3				position;
		D3DXMATRIX				matTransform;
		D3DXMATRIX				matCamera;
		SharedInfoSpaceObject *	sharedInfo;

		//Functions
		void initObject(LPD3DXMESH mesh, LPDIRECT3DTEXTURE9 texture, D3DXVECTOR4 color);
		void updatePosition();
		//////////////////////////////////////////////
		
};
