#include "dxstdafx.h"
#include "World.h"

World::World(void)
{
}

void World::updateEffect(ID3DXEffect * effect, GraphicsManager * gManager, D3DXVECTOR4	* shipDiffuse, 
						D3DXVECTOR3	* shipPos, int numShips)
{
	D3DXMATRIXA16 mWorldViewProjection;
	D3DXVECTOR3 vLightDir[MAX_LIGHTS];
	D3DXCOLOR   vLightDiffuse[MAX_LIGHTS];	
	D3DXMATRIXA16 mWorld;
	D3DXMATRIXA16 mView;
	D3DXMATRIXA16 mProj;

	mWorld = matTransform;
	mProj = gManager->cam->GetProjMatrix();
	mView = gManager->cam->GetViewMatrix();
	mWorldViewProjection = mWorld * mView * mProj;
	D3DXVECTOR3 newPos = gManager->cam->GetPosVec();
	D3DXVECTOR4	camPos	= D3DXVECTOR4(newPos.x, newPos.y, newPos.z, 1);
	
	//initialize effect globals
	for (int i =0; i<4; i++)
	{
        vLightDir[i]		= gManager->lights[i].Direction;
		vLightDiffuse[i]	= gManager->lights[i].Diffuse;
	}

	D3DXCOLOR colorMtrlDiffuse(1.0f, 1.0f, 1.0f, 1.0f);
	D3DXCOLOR colorMtrlAmbient(0.35f, 0.35f, 0.35f, 0);
	D3DXCOLOR vWhite = D3DXCOLOR(1,1,1,1);

	effect->SetMatrix	( "g_mWorldViewProjection", &mWorldViewProjection );
	effect->SetMatrix	( "g_mWorld", &mWorld );
	effect->SetVector	( "camPos", &camPos );

	effect->SetValue	( "sunLightDir", vLightDir, sizeof(D3DXVECTOR3)*MAX_LIGHTS );
	effect->SetValue	( "sunLightDiffuse", vLightDiffuse, sizeof(D3DXVECTOR4)*MAX_LIGHTS );
	effect->SetInt		( "numSunLights", 4 );    

	//set material and texture
	effect->SetValue	( "planetMatAmbient", &colorMtrlAmbient, sizeof(D3DXCOLOR) );
	effect->SetValue	( "planetMatDiffuse", &colorMtrlDiffuse, sizeof(D3DXCOLOR) );  
	effect->SetTexture	( "colorMap", this->colorMap);
	effect->SetTexture	( "colorMap", this->colorMap);
	effect->SetTexture	( "normalMap", this->normalMap);
	effect->SetTexture	( "cubeTexture",this->cubeTexture);
	effect->SetFloat	( "intensity",0);
	
	  
	//planet specific setup
	effect->SetValue( "lightPositions", shipPos, sizeof(D3DXVECTOR3)*4 );
	effect->SetValue( "lightDiffuse", shipDiffuse, sizeof(D3DXVECTOR4)* 4 );	
	effect->SetInt( "numLights", numShips );    		
}


//Draws Planet Mesh
//
void World::drawMesh(MyEffect * myEffect, GraphicsManager * gManager, D3DXVECTOR4	* shipDiffuse, 
						D3DXVECTOR3	* shipPos, int numShips)
{
	UINT cPasses;
	//ID3DXMesh * mesh = gManager->getMesh(meshFileName); 
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
		printf("Found NULL Mesh object %s", meshFileName);
	}
}