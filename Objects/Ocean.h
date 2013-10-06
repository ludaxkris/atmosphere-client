#pragma once
#include <d3d9.h>
#include <d3dx9.h>
#include "SpaceObject.h"
#include "../GraphicsManager/GraphicsManager.h"

class Ocean 
{	
public:
	//variables
	D3DXVECTOR3			scale;					//scale    
	LPD3DXMESH			mesh;					// Mesh 
	D3DXMATRIX			matScale;
	LPDIRECT3DTEXTURE9  normalMap;
	D3DXMATRIX			matTransform;
	D3DXCOLOR			colorDiffuse;				
	IDirect3DCubeTexture9 * cubeTexture;

	//functions
	Ocean();
	~Ocean();

	void initObject(	LPD3DXMESH mesh,
						LPDIRECT3DTEXTURE9 cube,						
						D3DXVECTOR3 scaleVec);

	void setScalingMtx(D3DXVECTOR3 newScale);
	
	void updateEffect(ID3DXEffect * effect, GraphicsManager * gManager, D3DXVECTOR4	* shipDiffuse, 
						D3DXVECTOR3	* shipPos, int numShips);
	void drawMesh(MyEffect * myEffect, GraphicsManager * gManager, D3DXVECTOR4	* shipDiffuse, 
						D3DXVECTOR3	* shipPos, int numShips);
};
