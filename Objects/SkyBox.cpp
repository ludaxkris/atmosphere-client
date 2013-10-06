#include "SkyBox.h"

SkyBox::SkyBox()
{
	D3DXMatrixIdentity(&matScale);
}

SkyBox::~SkyBox()
{
}


void SkyBox::initObject(LPD3DXMESH mesh,
						IDirect3DCubeTexture9 * cube,						
						D3DXVECTOR3 scaleVec)

{
	this->mesh		= mesh;
	this->cubeMap	= cube;
	setScalingMtx(scaleVec);
}


void SkyBox::setScalingMtx(D3DXVECTOR3 newScale)
{
	this->scale = newScale;
	D3DXMatrixScaling(&matScale, scale.x, scale.y, scale.z);

}


void SkyBox::updateEffect(ID3DXEffect * effect, GraphicsManager * gManager)
{
	D3DXMATRIXA16 mWorldViewProjection;
	D3DXVECTOR3 vLightDir[MAX_LIGHTS];
	D3DXCOLOR   vLightDiffuse[MAX_LIGHTS];
	D3DXMATRIXA16 mWorld;
	D3DXMATRIXA16 mView;
	D3DXMATRIXA16 mProj;

	// Get the projection & view matrix from the camera class
	mWorld = this->matScale;
	mProj = gManager->cam->GetProjMatrix();
	mView = gManager->cam->GetViewMatrix();
	mWorldViewProjection = mWorld * mView * mProj;
	
	effect->SetMatrix	( "g_WorldViewProjection", &mWorldViewProjection );
	effect->SetMatrix	( "mView", &mView );
	effect->SetMatrix	( "g_mWorld", &mWorld );

	//set material and texture	
	effect->SetTexture	( "cubeTexture", this->cubeMap);	  		
}


void SkyBox::Render( IDirect3DDevice9* pd3dDevice, ID3DXEffect * effect, GraphicsManager * gManager )
{
	

	UINT cPasses;
//	ID3DXMesh * mesh = gManager->getMesh(meshFileName); 
	
	//update Effect
	updateEffect(effect, gManager);
	
	//begin effect
	effect->Begin(&cPasses, 0);
	
	if(mesh != NULL)
	{		
		for (UINT iPass = 0; iPass < cPasses; iPass++)
		{
			effect->BeginPass(iPass);			
			mesh->DrawSubset(0);
			effect->EndPass();
			//effect->CommitChanges();		
		}
	}
	else
	{
		fprintf(stderr,"Mesh not loaded for skybox.\n");
	}
}