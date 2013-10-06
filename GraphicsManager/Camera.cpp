#include "Camera.h"

Camera::Camera(float width, float height, float FOV, float _zoom)
{	
	up=D3DXVECTOR3(0,1,0);
	look=D3DXVECTOR3(0,0,1);

	zoom = _zoom;
	D3DXMatrixPerspectiveFovLH(&projMatrix, FOV, width/height, 1, 2000);

	D3DXMATRIX baseMatrix;
	D3DXMatrixTranslation(&baseMatrix, 0, 0, zoom);
	pos = D3DXVECTOR3(baseMatrix._41, baseMatrix._42, baseMatrix._43);

	D3DXMatrixLookAtLH(&viewMatrix, &pos, &look, &up);
}

void Camera::CalculateViewMatrix(D3DXMATRIX transMatrix)
{
	D3DXVECTOR3 yaxis = D3DXVECTOR3(0,1,0);
	D3DXVec3TransformCoord(&up, &yaxis, &transMatrix);

	D3DXVECTOR3 zaxis = D3DXVECTOR3(0,0,1);
	D3DXVec3TransformCoord(&look, &zaxis, &transMatrix);

	D3DXMATRIX baseMatrix;										// position matrix
	D3DXMatrixTranslation(&baseMatrix, 0, 0, zoom);				// inital position
	baseMatrix = baseMatrix * transMatrix;						// translated
	pos = D3DXVECTOR3(baseMatrix._41, baseMatrix._42, baseMatrix._43);
	
	D3DXMatrixLookAtLH(&viewMatrix, &pos, &look, &up);
}

D3DXMATRIX Camera::GetProjMatrix()
{
	return projMatrix;
}

D3DXMATRIX Camera::GetViewMatrix()
{
	return viewMatrix;
}

D3DXVECTOR3 Camera::GetPosVec()
{
	return pos;
}

D3DXVECTOR3 Camera::GetUpVec()
{
	D3DXVECTOR3 normedUp;
	D3DXVec3Normalize(&normedUp, &up);
	return normedUp;
}

D3DXVECTOR3 Camera::GetLookVec()
{
	D3DXVECTOR3 normedLook;
	D3DXVec3Normalize(&normedLook, &look);
	return normedLook;
}