#ifndef camera_h
#define camera_h

#include "dxstdafx.h"

class Camera
{
	public:
		Camera(float width, float height, float FOV, float _zoom);
		void CalculateViewMatrix(D3DXMATRIX transMatrix); //  called only object
		D3DXMATRIX GetProjMatrix();
		D3DXMATRIX GetViewMatrix();
		D3DXVECTOR3 GetPosVec();
		D3DXVECTOR3 GetUpVec();
		D3DXVECTOR3 GetLookVec();

	private:
		D3DXVECTOR3 up, look, pos;	// camera vectors
		float zoom;						// camera's z position
		D3DXMATRIX projMatrix;
		D3DXMATRIX viewMatrix;
};

#endif