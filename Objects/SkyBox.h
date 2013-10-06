#pragma once
#include <d3d9.h>
#include <d3dx9.h>
#include "../GraphicsManager/GraphicsManager.h"

class SkyBox
{	
public:
	//variables
	IDirect3DCubeTexture9 *	cubeMap;				//cube map texture
	D3DXVECTOR3				scale;					//scale    
	LPD3DXMESH				mesh;					// Mesh 

	D3DXMATRIX				matScale;
	
	
	//functions
	SkyBox();
	~SkyBox();


	void initObject(	LPD3DXMESH mesh,
						IDirect3DCubeTexture9 * cube,						
						D3DXVECTOR3 scaleVec);

	void setScalingMtx(D3DXVECTOR3 newScale);
	void updateEffect(ID3DXEffect * effect, GraphicsManager * gManager);
	void Render( IDirect3DDevice9* pd3dDevice, ID3DXEffect * effect, GraphicsManager * gManager );
};
