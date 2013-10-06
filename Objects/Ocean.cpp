#include "Ocean.h"

Ocean::Ocean()
{
	D3DXMatrixIdentity(&matScale);
	D3DXMatrixIdentity(&matTransform);
	colorDiffuse = D3DXCOLOR(1,1,1,1);
}

Ocean::~Ocean()
{
}


void Ocean::initObject(LPD3DXMESH mesh,
						LPDIRECT3DTEXTURE9 tex,					
						D3DXVECTOR3 scaleVec)

{
	this->mesh		= mesh;
	this->normalMap	= tex;
	setScalingMtx(scaleVec);
}


void Ocean::setScalingMtx(D3DXVECTOR3 newScale)
{
	this->scale = newScale;
	D3DXMatrixScaling(&matScale, scale.x, scale.y, scale.z);

}


void Ocean::updateEffect(ID3DXEffect * effect, GraphicsManager * gManager, D3DXVECTOR4	* shipDiffuse, 
						D3DXVECTOR3	* shipPos, int numLights)
{
	static int preTime = 0;
	static int cIndex = 0;

	D3DXMATRIXA16 mWorldViewProjection;
	D3DXVECTOR3 vLightDir[MAX_LIGHTS];
	D3DXCOLOR   vLightDiffuse[MAX_LIGHTS];
	D3DXMATRIXA16 mWorld;
	D3DXMATRIXA16 mView;
	D3DXMATRIXA16 mProj;

	//error flag
	int hr;

	// Get the projection & view matrix from the camera class
	mWorld = matScale;
	mProj = gManager->cam->GetProjMatrix();
	mView = gManager->cam->GetViewMatrix();
	mWorldViewProjection = mWorld * mView * mProj;
	
	D3DXVECTOR3 newPos = gManager->cam->GetPosVec();
	D3DXVECTOR4	camPos	= D3DXVECTOR4(newPos.x, newPos.y, newPos.z, 1);
	//printf("mProj %f %f %f\n",newPos.x,newPos.y,newPos.z);

	//initialize effect globals
	for (int i =0; i<4; i++)
	{
        vLightDir[i]		= gManager->lights[i].Direction;
		vLightDiffuse[i]	= gManager->lights[i].Diffuse;
		
	}

	D3DXCOLOR colorMtrlDiffuse(1.0f, 1.0f, 1.0f, 1.0f);
	D3DXCOLOR vWhite = D3DXCOLOR(1,1,1,1);

	effect->SetMatrix	( "g_mWorldViewProjection", &mWorldViewProjection );
	effect->SetMatrix	( "g_mWorld", &mWorld );
	effect->SetVector	( "camPos", &camPos );


	effect->SetValue	( "sunLightDir", vLightDir, sizeof(D3DXVECTOR3)*MAX_LIGHTS );
	effect->SetValue	( "sunLightDiffuse", vLightDiffuse, sizeof(D3DXVECTOR4)*MAX_LIGHTS );
	effect->SetInt		( "numSunLights", 4 );    

	//set material and texture
	effect->SetValue	( "planetMatDiffuse", &colorDiffuse, sizeof(D3DXCOLOR) ); 

	if(timeGetTime()-preTime > 150)
	{
		cIndex = (++cIndex)%30;
		preTime = timeGetTime();
	}

	effect->SetTexture	( "normalMap", gManager->caustics[cIndex]);
	effect->SetTexture	( "cubeTexture",this->cubeTexture);
	effect->SetFloat	( "intensity",0);
	
	  
	//planet specific setup
	effect->SetValue( "lightPositions", shipPos, sizeof(D3DXVECTOR3)*numLights );
	effect->SetValue( "lightDiffuse", shipDiffuse, sizeof(D3DXVECTOR4)* numLights );	
	effect->SetInt( "numLights", numLights );    	

	//set current player index
	effect->SetInt( "playerIndex", gManager->currentPlayer);
}


//Draws Planet Mesh
////////////////////////////
void Ocean::drawMesh(MyEffect * myEffect, GraphicsManager * gManager, D3DXVECTOR4	* shipDiffuse, 
						D3DXVECTOR3	* shipPos, int numShips)
{
	UINT cPasses;
	ID3DXEffect * effect = myEffect->effect;
	
	//update Effect
	updateEffect(effect, gManager, shipDiffuse, shipPos, numShips);
	
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
		printf("Found NULL Mesh object");
	}
}