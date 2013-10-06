#include "Item.h"

Item::Item(void)
{
	itemType = 0;
}

/////////////////////////
//Item Types
//	0#	-	Red (Bombs)
//	00	-	Standard Bomb
//	01	-	Proximiity bomb

//	1#	-	Blue (Shield boost)
//	10	-	10 second shield boost
//	11	-	10 second invicibility

//	2#	-	Green (Weapon upgrade)
//	20	-	weapon level upgrade
//	21	-	10 second increase firing rate

//	3#	-	White with Red Cross (Health regenerate)
//	30	-	30 hp health increase
/////////////////////////




//void Base::updateEffect(ID3DXEffect * effect, GraphicsManager * gManager)
//{	
//	D3DXMATRIXA16 mWorldViewProjection;
//	D3DXVECTOR3 vLightDir[MAX_LIGHTS];
//	D3DXCOLOR   vLightDiffuse[MAX_LIGHTS];
//	D3DXMATRIXA16 mWorld;
//	D3DXMATRIXA16 mView;
//	D3DXMATRIXA16 mProj;
//
//	mWorld = matTransform;
//	mProj = gManager->cam->GetProjMatrix();
//	mView = gManager->cam->GetViewMatrix();
//	mWorldViewProjection = mWorld * mView * mProj;
//
//	//printf("Base pos %f %f %f %f\n", matTransform._41,matTransform._42,matTransform._43,matTransform._44 );
//	//printf("PlayerShip servr %f %f %f %f\n", this->sharedIPS->matTransform._11,this->sharedIPS->matTransform._12,
//	//										this->sharedIPS->matTransform._13,this->sharedIPS->matTransform._14 );
//
//	//get sun light
//	for (int i =0; i<4; i++)
//	{
//        vLightDir[i]		= gManager->lights[i].Direction;
//		vLightDiffuse[i]	= gManager->lights[i].Diffuse;
//	}
//
//	D3DXCOLOR vWhite = D3DXCOLOR(1,1,1,1);
//
//	effect->SetMatrix	( "g_mWorldViewProjection", &mWorldViewProjection );
//	effect->SetMatrix	( "g_mWorld", &mWorld );
//
//	effect->SetValue	( "sunLightDir", vLightDir, sizeof(D3DXVECTOR3)*MAX_LIGHTS );
//	effect->SetValue	( "sunLightDiffuse", vLightDiffuse, sizeof(D3DXVECTOR4)*MAX_LIGHTS );
//	effect->SetInt		( "numSunLights", MAX_LIGHTS );    
//
//	//set material and texture
//	effect->SetValue	( "g_MaterialAmbientColor", &colorDiffuse, sizeof(D3DXCOLOR) );
//	effect->SetValue	( "g_MaterialDiffuseColor", &colorDiffuse, sizeof(D3DXCOLOR) );    
//	effect->SetTexture	( "g_MeshTexture", texture);	  	
//}
//
//
//
//void Base::drawMesh(MyEffect * myEffect, GraphicsManager * gManager)
//{
//
//	UINT cPasses;
//	//ID3DXMesh * mesh = gManager->getMesh(meshFileName); 
//	ID3DXEffect * effect = myEffect->effect;
//
//	//update effect
//	updateEffect(effect, gManager);
//	
//	//begin effect
//	effect->Begin(&cPasses, 0);
//
//	if(mesh != NULL)
//	{		
//		for (UINT iPass = 0; iPass < cPasses; iPass++)
//		{
//			effect->BeginPass(0);
//			mesh->DrawSubset(0);
//			effect->EndPass();
//			//effect->CommitChanges();		
//		}
//	}
//	else
//	{
//		printf("Found NULL Mesh object %s", meshFileName);
//	}
//}
