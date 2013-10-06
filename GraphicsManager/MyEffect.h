#pragma once
#include "MyEffect.h"
#include "dxstdafx.h"

class MyEffect 
{

	public:
		ID3DXEffect * effect;

		MyEffect();
		void CompileEffect(LPDIRECT3DDEVICE9 g_pd3dDevice, const char * filename);
};
