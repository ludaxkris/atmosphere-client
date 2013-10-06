#include "MyLight.h"


MyLight::MyLight()
{
	Ambient		= D3DXCOLOR(1,1,1,1);
	Diffuse		= D3DXCOLOR(1,1,1,1);
	Specular	= D3DXCOLOR(0.5f, 0.5f, 0.5f, 0.5f);
	Direction	= D3DXVECTOR3(0.0f, 0.0f, 1.0f);  
}


void MyLight::SetAmbient(D3DXCOLOR amb)
{
	Ambient		= amb;
}

void MyLight::SetDiffuse(D3DXCOLOR dif)
{
	Diffuse		= dif;
}

void MyLight::SetSpecular(D3DXCOLOR spec)
{
	Specular		= spec;
}

void MyLight::SetPosition(D3DXVECTOR3 pos)
{
	Position		= pos;
}

void MyLight::Update()
{
	//Direction = D3DXVECTOR3( 0, 0 ,-1 );	
	//Direction = D3DXVECTOR3(-Position.x, -Position.y ,-Position.z );	
	Direction = Position;
	D3DXVec3Normalize(&Direction, &Direction);
}