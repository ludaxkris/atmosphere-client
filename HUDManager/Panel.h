#pragma once
#include "./../Common/dxstdafx.h"

class Panel
{
public:
	Panel(D3DXVECTOR3 v_panelPos, D3DCOLOR v_panelCol, 
		  LPDIRECT3DTEXTURE9 p_panelTexture, DWORD * p_globalTime);
	~Panel();

	void update(void);
	void render(LPD3DXSPRITE p_D3DSprite);

	void blendIn(void);

private:

	DWORD * p_globalTime;
	D3DXVECTOR3 v_Center;
	D3DXVECTOR3 v_Position;
	D3DCOLOR v_Color;
	LPDIRECT3DTEXTURE9 p_Texture;
};