#ifndef mylight_h
#define myLight_h
#include "dxstdafx.h"
//#include "resource.h"

#define MAX_LIGHTS 4

class MyLight
{
	public:
		D3DXCOLOR Ambient;
		D3DXCOLOR Diffuse;
		D3DXCOLOR Specular;
		D3DXVECTOR3 Direction;  
		D3DXVECTOR3 Position;  

		MyLight();	

		void SetAmbient(D3DXCOLOR amb);
		void SetDiffuse(D3DXCOLOR dif);
		void SetSpecular(D3DXCOLOR spec);
		void SetPosition(D3DXVECTOR3 dir);
		void Update();
};


#endif 