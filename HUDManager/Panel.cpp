#include "Panel.h"

Panel::Panel(D3DXVECTOR3 v_panelPos, D3DCOLOR v_panelCol, 
			 LPDIRECT3DTEXTURE9 p_panelTexture, DWORD *p_globalTime)
{
	this->v_Center = D3DXVECTOR3(0.0f, 0.0f, 0.0f);    // center at the upper-left corner
	this->v_Position = v_panelPos;
	this->v_Color = v_panelCol;
	this->p_Texture = p_panelTexture;	
}

Panel::~Panel()
{
}

void Panel::update()
{
	blendIn();

}

void Panel::render(LPD3DXSPRITE p_D3DSprite)
{
	/* Prototype
		Draw(LPDIRECT3DTEXTURE9 p_Texture, CONST RECT* pSrcRect,
			CONST D3DXVECTOR3* pCenter, CONST D3DXVECTOR3* pPosition,
			D3DCOLOR Color);
	*/
	
	p_D3DSprite->Draw(p_Texture, NULL, &v_Center, &v_Position, v_Color);
}

void Panel::blendIn()
{
	
}