#include "SpaceObject.h"
#include "strsafe.h"

SpaceObject::SpaceObject(void)
{
}

void SpaceObject::initObject(LPD3DXMESH mesh, LPDIRECT3DTEXTURE9 texture, D3DXVECTOR4 color)
{
	this->mesh		= mesh;
	this->texture	= texture;
	this->colorDiffuse	= color;

	position = D3DXVECTOR3(0,0,0);
	D3DXMatrixIdentity(&matTransform);
	D3DXMatrixIdentity(&matCamera);
}

void SpaceObject::updatePosition()
{
	matTransform = sharedInfo->matTransform;

	D3DXMATRIX matScale;
	D3DXMATRIX matHeight;
	D3DXMATRIX matRotZ;

	float scale = matTransform._31;
	float height = matTransform._32;
	float zradian = matTransform._33;

	D3DXMatrixScaling(&matScale, scale, scale, scale);
	D3DXMatrixTranslation(&matHeight, 0, 0, height);
	D3DXVECTOR3 zaxis = D3DXVECTOR3(0,0,1);
	D3DXMatrixRotationAxis(&matRotZ, &zaxis, zradian);

	D3DXVECTOR3 v1 = D3DXVECTOR3(matTransform._11, matTransform._12, matTransform._13);
	D3DXVECTOR3 v2 = D3DXVECTOR3(matTransform._21, matTransform._22, matTransform._23);
	D3DXVECTOR3 v3;
	D3DXVec3Cross(&v3, &v1, &v2);

	matCamera = matTransform;
	matCamera._31 = v3.x;
	matCamera._32 = v3.y;
	matCamera._33 = v3.z;

	matTransform = matScale * matRotZ * matHeight * matCamera;

	position = D3DXVECTOR3(matTransform._41, matTransform._42, matTransform._43);
}