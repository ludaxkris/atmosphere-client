#pragma once
#include "SpaceObject.h"

class World : public SpaceObject
{
	public:	
		//Everyone's
		//////////////////////////////////////////
		World(void);
		//////////////////////////////////////////

		//Arjay's system stuff
		////////////////////////////////////////////
		virtual bool isWorld(){return true;}
		////////////////////////////////////////////

		//Nick's Graphics stuff
		////////////////////////////////////////////
		IDirect3DCubeTexture9 * cubeTexture;
		LPDIRECT3DTEXTURE9		colorMap;			// Mesh texture
		LPDIRECT3DTEXTURE9		normalMap;			// Mesh texture

		void updateEffect(ID3DXEffect * effect, GraphicsManager * gManager, D3DXVECTOR4	* shipDiffuse, 
							D3DXVECTOR3	* shipPos, int numShips);
		void drawMesh(MyEffect * effect, GraphicsManager * gManager, D3DXVECTOR4	* shipDiffuse, 
							D3DXVECTOR3	* shipPos, int numShips);
		
		////////////////////////////////////////////
};
