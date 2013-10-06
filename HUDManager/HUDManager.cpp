#include "HUDManager.h"

HUDManager::HUDManager(LPDIRECT3DDEVICE9 p_Device)
{
	this->p_Device = p_Device;

	LPDIRECT3D9 p_interface;    // the pointer to our Direct3D interface
	p_interface = Direct3DCreate9(D3D_SDK_VERSION);    // create the Direct3D object

	D3DXCreateSprite(p_Device, &p_D3DSprite);    // create the Direct3D Sprite object

	
	//p_Texture = p_gManager->getTexture(hudTexFileName);
}

HUDManager::~HUDManager(void)
{
	p_D3DSprite->Release();
	for(unsigned int i = 0; i < v_Panels.size(); i++)
	{
		delete v_Panels.at(i);
	}
}

void HUDManager::initHUD(GraphicsManager * p_gManager, DWORD * p_globalTime)
{
	std::vector<D3DXVECTOR3>v_panelPos;
	std::vector<D3DCOLOR>v_panelCol;
	std::vector<char * >v_hudTexFileName;
	

	v_panelPos.push_back(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	v_panelCol.push_back(D3DCOLOR_ARGB(127, 255, 255, 255));
	v_hudTexFileName.push_back("..\\Media\\Textures\\HUD\\Panel2.png");
	

	Panel * panel;
	for(unsigned int i = 0; i < v_hudTexFileName.size(); i++)
	{
		panel = new Panel(v_panelPos.at(i), v_panelCol.at(i), 
			              p_gManager->getTexture(v_hudTexFileName.at(i)),
						  p_globalTime);
		v_Panels.push_back(panel);
	}
}


void HUDManager::update(void)
{
	for(unsigned int i = 0; i < v_Panels.size(); i++)
	{
		v_Panels.at(i)->update();
	}
}

void HUDManager::render(void)
{
	p_D3DSprite->Begin(D3DXSPRITE_ALPHABLEND);    // begin sprite drawing

	// perform sprite drawing here
	for(unsigned int i = 0; i < v_Panels.size(); i++)
	{
		v_Panels.at(i)->render(p_D3DSprite);
	}

	p_D3DSprite->End();    // end sprite drawing

}