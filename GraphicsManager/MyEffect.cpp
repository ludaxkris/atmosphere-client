#include "MyEffect.h"
#include <fstream>

MyEffect::MyEffect()
{
	effect = NULL;
}

void MyEffect:: CompileEffect(LPDIRECT3DDEVICE9 g_pd3dDevice, const char * filename)
{
	WCHAR temp[MAX_PATH];
	WCHAR str[MAX_PATH];
	DWORD dwShaderFlags = D3DXFX_NOT_CLONEABLE;    
	LPD3DXBUFFER errorBuffer = NULL;

	// Read the D3DX effect file
	MultiByteToWideChar(CP_ACP, 0, filename, (int)strlen (filename),  temp, MAX_PATH);
	
	//null string
	temp[strlen (filename)] = '\0';

	// Create the mesh texture from a file
    FindDXSDKMediaFileCch( str, MAX_PATH, temp);

	//release before adding
	if(effect != NULL)
		SAFE_RELEASE(effect);

	//create effect file	
    if(D3DXCreateEffectFromFile( g_pd3dDevice, str, NULL, NULL, dwShaderFlags, NULL, &effect, &errorBuffer ) < 0)
	{
		//output error info to file
		char *errorString = (char*)(errorBuffer->GetBufferPointer());
		
		std::ofstream errorFile("ErrorCompile.txt");
		if (errorFile.is_open()) {
			errorFile << errorString << std::endl;
			errorFile.close();
		}

		printf("Error compiling texture: %s\n", filename);
		effect = NULL;

		exit(0);
	}
}