#ifndef GRAPHICSMANAGER_H
#define GRAPHICSMANAGER_H
#include "dxstdafx.h"
#include "MyLight.h"
#include "MyEffect.h"
#include "Camera.h"
#include <map>


class GraphicsManager
{

	struct strCmp 
	{
		bool operator()( const char* s1, const char* s2 ) const 
		{
			return strcmp( s1, s2 ) < 0;
		}
	};

	public:	
		//Variables
		///////////////////////////////////////////////////////////////////////////////////
		std::map<char *, MyEffect *, strCmp> Effects;				// effects
		std::map<char *, ID3DXMesh *, strCmp> Meshes;				// meshs
		std::map<char *, IDirect3DTexture9 *, strCmp> Textures;		// 2Dtextures 
		std::map<char *, IDirect3DCubeTexture9 *, strCmp> cubeTextures;	// 3Dtextures 
		
		int currentPlayer;

		char ** renderTechnique;
		IDirect3DTexture9 ** caustics;								//caustics

		MyLight lights[MAX_LIGHTS];									// global lighting
		Camera *cam;												//camera obj
		LPDIRECT3DDEVICE9 pd3dDevice;								//3d device		
	
		//effects
		MyEffect * ship;	
		MyEffect * world;
		MyEffect * sky;	
		MyEffect * ocean;

		//Functions
		///////////////////////////////////////////////////////////////////////////////////
		GraphicsManager		(void);
		~GraphicsManager	();


		void initialize		(LPDIRECT3DDEVICE9 g_pd3dDevice);
		void initCaustics	();
		void initTechnique	();
		void initLight		(void);
		void compileShaders (LPDIRECT3DDEVICE9 pd3dDevice);

		void insertEffect	(char * filename, LPDIRECT3DDEVICE9 g_pd3dDevice);
		void insertTexture	(char * filename, LPDIRECT3DDEVICE9 g_pd3dDevice);
		void insertMesh		(char * filename, LPDIRECT3DDEVICE9 g_pd3dDevice);		
		void insertCubeTexture(char * filename, LPDIRECT3DDEVICE9 g_pd3dDevice);

		//accessor methods
		MyEffect *				getEffect		(char * filename);	
		IDirect3DTexture9 *		getTexture		(char * filename);		
		ID3DXMesh *				getMesh			(char * filename);		
		IDirect3DCubeTexture9 * getCubeTexture	(char * filename);		


		//release resources
		void ReleaseResource ();

};


#endif 
