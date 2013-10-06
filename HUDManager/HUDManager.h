#pragma once
#include <vector>

#include "./../Common/dxstdafx.h"
#include "./../GraphicsManager/GraphicsManager.h"
#include "./Panel.h"

class HUDManager
{
	public:
		HUDManager(LPDIRECT3DDEVICE9 p_Device);
		~HUDManager(void);

		void initHUD(GraphicsManager * p_gManager, DWORD * p_globalTime);

		void update(void);
		void render(void);

		LPDIRECT3DDEVICE9 p_Device;
		LPD3DXSPRITE p_D3DSprite;
		LPDIRECT3DTEXTURE9 p_Texture;

		std::vector<Panel *> v_Panels;
};