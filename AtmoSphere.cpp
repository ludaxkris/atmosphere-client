//--------------------------------------------------------------------------------------
// File: BasicHLSL.cpp
//
// This sample shows a simple example of the Microsoft Direct3D's High-Level 
// Shader Language (HLSL) using the Effect interface. 
//
// Copyright (c) Microsoft Corporation. All rights reserved.
//--------------------------------------------------------------------------------------
#include "dxstdafx.h"

#include "./Network/client.h"
#include "./GraphicsManager/GraphicsManager.h"
#include "./GraphicsManager/Controller.h"
#include "./GraphicsManager/MyEffect.h"
#include "./Objects/ObjectManager.h"
#include "./Objects/SkyBox.h"
#include "./ConfigManager/ConfigManager.h"
#include "./ParticleSystemManager/ParticleSystemManager.h"
#include "./HUDManager/HUDManager.h"
#include "./Sound/Sound.h"


//#Define
#define OFFSET_THRUST				0	
#define OFFSET_PROJECTILE_TYPE_1	100
#define OFFSET_EXPLOSION_CLUSTER	200
#define OFFSET_WORLD				300

//--------------------------------------------------------------------------------------
// Global variables
//--------------------------------------------------------------------------------------
//my variables
DWORD *					globalTime;
DWORD					localTime = timeGetTime();
int						currentPlayer			= -1;

GraphicsManager *		graphicsManager			= NULL;
ObjectManager *			objectManager			= NULL;
ParticleSystemManager * psm			= NULL;
HUDManager *			hudManager	= NULL;

ConfigManager *		cManager		= NULL;
AtmosphereConfig *	config			= NULL;

Controller *		control			= NULL;
IDirect3DDevice9*	pd3dDevice		= NULL;
LPDIRECT3D9			g_pD3D			= NULL;		//directx version
HWND				g_hWnd			= NULL;		

//reconnect variable
static bool reconnect = false;

//***TEST ONLY*******//
static bool reqFlag = false;
//***TEST ONLY*******//

//network globals0
client * com;

//Sound
Sound* sound;



//--------------------------------------------------------------------------------------
// control ID
//--------------------------------------------------------------------------------------
#define IDI_DIRECTX_ICON		107


//--------------------------------------------------------------------------------------
// Forward declarations 
//--------------------------------------------------------------------------------------
void ReleaseResource();
LRESULT CALLBACK WindowProc( HWND   hWnd, UINT   msg, WPARAM wParam, LPARAM lParam );
void Update();
void Render();
void init();
void initAtmosphere();
void ConnectToServer();
void recreatePSM();


//--------------------------------------------------------------------------------------
// Entry point to the program. Initializes everything and goes into a message processing 
// loop. Idle time is used to render the scene.
//--------------------------------------------------------------------------------------
int WINAPI WinMain( HINSTANCE hInstance,
                    HINSTANCE hPrevInstance,
                    LPSTR     lpCmdLine,
                    int       nCmdShow )
{
	if(hInstance || hPrevInstance || lpCmdLine){};
	WNDCLASSEX winClass;
	MSG        uMsg;


	//config manager
	cManager = new ConfigManager();
	config = cManager->buildAtmosphere();

    memset(&uMsg,0,sizeof(uMsg));

	winClass.lpszClassName = L"MY_WINDOWS_CLASS";
	winClass.cbSize        = sizeof(WNDCLASSEX);
	winClass.style         = CS_HREDRAW | CS_VREDRAW ;
	winClass.lpfnWndProc   = WindowProc;
	winClass.hInstance     = hInstance;
	winClass.hIcon	       = LoadIcon(hInstance, (LPCTSTR)IDI_DIRECTX_ICON);
    winClass.hIconSm	   = LoadIcon(hInstance, (LPCTSTR)IDI_DIRECTX_ICON);
	winClass.hCursor       = LoadCursor(NULL, IDC_ARROW);
	winClass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	winClass.lpszMenuName  = NULL;
	winClass.cbClsExtra    = 0;
	winClass.cbWndExtra    = 0;
	
	if( !RegisterClassEx(&winClass) )
		return E_FAIL;

	g_hWnd = CreateWindowEx( NULL,L"MY_WINDOWS_CLASS", 
                             L"ATMOSPHERE",
						     WS_OVERLAPPEDWINDOW | WS_VISIBLE,
							 0, 0, config->screenSizeWidth, config->screenSizeHeight, 
							 NULL, NULL, hInstance, NULL );

	if( g_hWnd == NULL )
		return E_FAIL;

    ShowWindow( g_hWnd, nCmdShow );
    UpdateWindow( g_hWnd );

	init();

	while( uMsg.message != WM_QUIT )
	{
		if( PeekMessage( &uMsg, NULL, 0, 0, PM_REMOVE ) )
		{ 
			TranslateMessage( &uMsg );
			DispatchMessage( &uMsg );
		}
        else
		{
			Update();
			Render();
		}
		//if reconnect
		if(reconnect)
		{	
 			ConnectToServer();
			recreatePSM();
			Sleep(1000);
			reconnect = false;
			printf("Reconnecting to server\n\n");
		}
	}

	//clean up resources
	ReleaseResource();

    UnregisterClass(L"MY_WINDOWS_CLASS", winClass.hInstance );

	return (int)uMsg.wParam;
}

LRESULT CALLBACK WindowProc( HWND   hWnd, UINT   msg, WPARAM wParam, LPARAM lParam )
{

	//debug with keyboard
	D3DXVECTOR2 rightStick	= D3DXVECTOR2(0,0);
	D3DXVECTOR2 leftStick	= D3DXVECTOR2(0,0);
    int size = sizeof(D3DXVECTOR2);
	static char* data = (char*)malloc(2*size);	
	char command  = 11;	
	//debug

	switch( msg )
	{
        case WM_KEYDOWN:
		{
			switch( wParam )
			{
				case VK_ESCAPE:
					PostQuitMessage(0);
					break;
				case 'C': 
					printf("Recompiling Shaders\n");
					graphicsManager->compileShaders(pd3dDevice);
					break;
				case 'W':					
					leftStick = D3DXVECTOR2(0,1);
					break;
				case 'A':
					leftStick = D3DXVECTOR2(-1,0);
					break;
				case 'D':
					leftStick = D3DXVECTOR2(1,0);
					break;
				case 'S':
					leftStick = D3DXVECTOR2(0,-1);
					break;
				case 'R':
					reconnect = true;
					break;
	
			}    
			break;
		}
		case WM_CLOSE:
		{
			PostQuitMessage(0);
		}
		
        case WM_DESTROY:
		{
            PostQuitMessage(0);
		}
        break;

		default:
		{
			return DefWindowProc( hWnd, msg, wParam, lParam );
		}
		break;
	}
		

	if(leftStick.x != leftStick.y)
	{
		printf("Sending vector %f %f\n", leftStick.x, leftStick.y);
		memcpy(data, &leftStick, size);
		memcpy((data+size), &rightStick, size);
		com->sendDataToServer( data, size*2, command );
	}

	return 0;
}

void init( void )
{
	//sound = new Sound();
	//sound->initMusic();
    g_pD3D = Direct3DCreate9( D3D_SDK_VERSION );

	if( g_pD3D == NULL )
	{
		// TO DO: Respond to failure of Direct3DCreate8
		return;
	}

    D3DDISPLAYMODE d3ddm;

    if( FAILED( g_pD3D->GetAdapterDisplayMode( D3DADAPTER_DEFAULT, &d3ddm ) ) )
	{
		// TO DO: Respond to failure of GetAdapterDisplayMode
		return;
	}

	HRESULT hr;

	if( FAILED( hr = g_pD3D->CheckDeviceFormat( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, 
												d3ddm.Format, D3DUSAGE_DEPTHSTENCIL,
												D3DRTYPE_SURFACE, D3DFMT_D16 ) ) )
	{
		if( hr == D3DERR_NOTAVAILABLE )
			// POTENTIAL PROBLEM: We need at least a 16-bit z-buffer!
			return;
	}

	//
	// Do we support hardware vertex processing? if so, use it. 
	// If not, downgrade to software.
	//

	D3DCAPS9 d3dCaps;

	if( FAILED( g_pD3D->GetDeviceCaps( D3DADAPTER_DEFAULT, 
		                               D3DDEVTYPE_HAL, &d3dCaps ) ) )
	{
		// TO DO: Respond to failure of GetDeviceCaps
		return;
	}

	DWORD dwBehaviorFlags = 0;

	if( d3dCaps.VertexProcessingCaps != 0 )
		dwBehaviorFlags |= D3DCREATE_HARDWARE_VERTEXPROCESSING;
	else
		dwBehaviorFlags |= D3DCREATE_SOFTWARE_VERTEXPROCESSING;

	//
	// Everything checks out - create a simple, windowed device.
	D3DPRESENT_PARAMETERS d3dpp;
	memset(&d3dpp, 0, sizeof(d3dpp));

    d3dpp.BackBufferFormat       = d3ddm.Format;
	d3dpp.SwapEffect             = D3DSWAPEFFECT_DISCARD;
	d3dpp.Windowed               = TRUE;
    d3dpp.EnableAutoDepthStencil = TRUE;
    d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
    d3dpp.PresentationInterval   = D3DPRESENT_INTERVAL_IMMEDIATE;

    if( FAILED( g_pD3D->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, g_hWnd,
                                      dwBehaviorFlags, &d3dpp, &pd3dDevice ) ) )
	{
		// TO DO: Respond to failure of CreateDevice
		return;
	}	
	
	//init graphics
	initAtmosphere();
}

void initAtmosphere()
{

	//graphics manager
	graphicsManager = new GraphicsManager();
	graphicsManager->initialize(pd3dDevice);

	//init object manager
	objectManager = new ObjectManager();
	objectManager->initGraphics(graphicsManager);

	//setup time
	//globalTime = &objectManager->getMary()->currentTime;
	globalTime = &localTime;

	//HUD
	hudManager = new HUDManager(pd3dDevice);
	hudManager->initHUD(graphicsManager, globalTime);

	//particle system
	psm = new ParticleSystemManager(pd3dDevice, globalTime, graphicsManager);
	psm->initPSM();


	//controller
	/////////////////////////////////////////
	control = new Controller();

	//init network
	/////////////////////////////////////////	
	ConnectToServer();	
	
	//setup global camera
	graphicsManager->cam = new Camera((float)config->screenSizeWidth, (float)config->screenSizeHeight,
										D3DX_PI/4, -80);;
}


void ConnectToServer()
{
	//delete com object
	if(com != NULL)
	{
		com->closeConnection();						
		delete (com);
	}

	//create new client
	com = new client(objectManager->m_PublicMem, objectManager->totalSharedMemSize);

	//connect objManager
	com->objMan = objectManager;	
	com->connectToServer(config->serverIP, config->serverPort); 

	//command to join a game
	char command = 0;
	int joinType = config->joinType;

	//connect to server
	com->sendDataToServer( (&joinType), sizeof(int),command);
	Sleep(1000);
	currentPlayer = ((int)com->playerID);
	graphicsManager->currentPlayer = currentPlayer;
	cerr<<"PlayerID::"<<currentPlayer <<endl;

	//psm->makePSMRequest("E_CLOUDS", &objectManager->privStartWorld[0], OFFSET_WORLD);
}


//--------------------------------------------------------------------------------------
// Update will be called before Render
//--------------------------------------------------------------------------------------
void Update()
{
	localTime = timeGetTime();
	static bool sent = false;

	static D3DXVECTOR2 rightStick;
	static D3DXVECTOR2 leftStick;
	static char command;
	static int size = sizeof(D3DXVECTOR2);
	static char* data = (char*)malloc(2*size);

	control->CheckControllerStatus();

	if(control->moveLeftStick())
		leftStick  = control->GetLeftThumbPos();
	else
		leftStick =D3DXVECTOR2(0,0);

	if(control->moveRightStick())
		rightStick = control->GetRightThumbPos();
	else
		rightStick =D3DXVECTOR2(0,0);
	
/******TEST ONLY*************/
	//if(com->temp == 0)
	//	fprintf(stderr,"%d # of update this frame\n", com->temp);

	//com->temp = 0;

	if(control->pressX())
	{
		if(!reqFlag)
		{
			psm->makePSMRequest("E_EXPLOSION_CLUSTER", objectManager->privStartPlayerShip, OFFSET_EXPLOSION_CLUSTER + 99);
			reqFlag = true;
		}
	}
	else
		reqFlag = false;
	
	
/******TEST ONLY************/
	if(control->moveLeftStick() || control->moveRightStick())
	{
		command  = 11;	
		memcpy(data, &leftStick, size);
		memcpy((data+size), &rightStick, size);

		com->sendDataToServer( data, size*2, command );
	}

	//update particle system
	//////////////////////////////////////////////////////////////////////////
	psm->update();


	//////////////////////////////////////////////////////////////////////////

	//Sleep(10);
}

void recreatePSM ()
{
	//clean up emitters 
	///////////////////////////////////////////////////////////////////////////
	//delete PSM and create new
	delete(psm);
	globalTime = &localTime;

	psm = new ParticleSystemManager(pd3dDevice, globalTime, graphicsManager);
	psm->initPSM();
}


//--------------------------------------------------------------------------------------
// Renders the scene
//--------------------------------------------------------------------------------------
void Render()
{ 
	//check to see if we should render
	if(!(objectManager->draw & 1))
	{
		return;
	}
	
	objectManager->draw = 0;

	static int numProjFired = 0;
	com->updateMemory();
	//clear buffer
	pd3dDevice->Clear( 0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
                         D3DCOLOR_COLORVALUE(0.5f,0.1f,0.1f,0.1f), 1.0f, 0 );

	pd3dDevice->BeginScene();
	//////////////////////////////////////////////////////////////////////////	

	//vars
	int numLights	= 0;
	static D3DXVECTOR4		dLightColor [4];
	static D3DXVECTOR3		dLightPos	[4];


	//draw player ship
	//////////////////////////////////////////////////////////////////////////
	PlayerShip * currentPShip;//	= objectManager->getPlayerShipList();
	
	graphicsManager->ship->effect->SetTechnique( "RenderMed" );

	for(int i = 0; i <objectManager->numPlayerShip; i++)
	{
		currentPShip = &objectManager->privStartPlayerShip[i];

		//get color and position
		dLightColor[numLights]	= D3DXVECTOR4(0,0,0,0);
		dLightPos[numLights]		= D3DXVECTOR3(0,0,0);	

		//check flag
		///////////////////////////////////////////////////////////////////////
		//check to see if we should delete
		if(currentPShip->shipFlag & 2)
		{
			//printf("Delete Thrusters::%d\n", currentPShip->index);
			psm->makePSMRequest("E_EXPLOSION_CLUSTER", currentPShip, OFFSET_EXPLOSION_CLUSTER + currentPShip->index);
			psm->removePSMRequest(currentPShip->index);


			//play sound at the correctSpot
			PlayerShip* curPlayer = &(objectManager->privStartPlayerShip[currentPlayer]);
			D3DXVECTOR3 pos = curPlayer->position;

			//sound->playSound(6, pos.x, pos.y, pos.z, 0, 0, 0);
			//sound->playSound(8, pos.x, pos.y, pos.z, 0, 0, 0);	

		}
		
		//check to see if we should emit
		if(currentPShip->shipFlag & 1)
		{					
			//printf("Create Thrusters::%d\n", currentPShip->index);
			psm->makePSMRequest("E_THRUST", currentPShip, OFFSET_THRUST + currentPShip->index);


			//play sound at the correctSpot
			PlayerShip* curPlayer = &(objectManager->privStartPlayerShip[currentPlayer]);
			D3DXVECTOR3 pos = curPlayer->position;

			//sound->playSound(5, pos.x, pos.y, pos.z, 0, 0, 0);	
		}
		
		//reset flag			
		currentPShip->shipFlag = 0;


		//Graphics
		//////////////////////////////////////////////////////////////////////
		//update and draw only when the ship is alive
		if(((SharedInfoPlayerShip *)currentPShip->sharedInfo)->hp >0)
		{
			//if(currentPShip)
			currentPShip->updatePosition();
			
			if(currentPShip->index == currentPlayer)
				graphicsManager->cam->CalculateViewMatrix(currentPShip->matCamera);


			currentPShip->drawMesh(graphicsManager->ship, graphicsManager);

			//get color and position
			dLightColor[numLights]	= currentPShip->colorDiffuse;
			dLightPos[numLights]		= currentPShip->position;	
		}

		numLights++;		
	}

	//Draw base
	//////////////////////////////////////////////////////////////////////////
	Base* currentBase;
	
	for(int i = 0; i< objectManager->numBase; i++)
	{

		currentBase = &objectManager->privStartBase[i];

		//check flag
		////////////////////////////////////////////////////
		if(currentBase->baseFlag & 1)
			fprintf(stderr, "Base %d is getting attacked.\n", currentBase->index);

		if(currentBase->baseFlag & 2)
		{
			fprintf(stderr, "Base %d has been taken over by %d.\n", currentBase->index,currentBase->ownerID);
		}

		//reset flag
		currentBase->baseFlag = 0;

		//Graphics		
		currentBase ->updatePosition();
		currentBase->drawMesh(graphicsManager->ship, graphicsManager);

		
	}
	

	graphicsManager->ship->effect->End();

	//update player position before we play sound
	PlayerShip* curPlayer = &(objectManager->privStartPlayerShip[currentPlayer]);
	D3DXVECTOR3 pPos = curPlayer->position;


	//sound->updatePlayerAudio(graphicsManager, pPos);

	//Request/Delete projectile
	//////////////////////////////////////////////////////////////////////////
	Projectile* currentProj	= objectManager->getProjectileList();
	
	for(int i = 0; i< objectManager->numProjectile; i++)
	{
		currentProj = &objectManager->privStartProjectile[i];

		currentProj->updatePosition();	
		//currentProj->drawMesh(ship, graphicsManager);		

		//request emitter delete
		if((int)currentProj->projFlag & 2)
		{
			//fprintf(stderr,"Flag value %d\n",currentProj->projFlag);						
//			fprintf(stderr,"Delete projectile %d\n",currentProj->index + OFFSET_PROJECTILE_TYPE_1);	
			psm->removePSMRequest(currentProj->index + OFFSET_PROJECTILE_TYPE_1);

			//play sound at the correctSpot
			D3DXVECTOR3 pos = currentProj->position;

//			cerr<<"PLAYER:::: X:"<<pPos.x<<" :: Y ::"<<pPos.y<<" :: Z ::"<<pPos.z<<endl;
//			cerr<<"delete PROJ:::: X:"<<pos.x<<" :: Y ::"<<pos.y<<" :: Z ::"<<pos.z<<endl;

			//don't know how to get the rectang position from projectile
			//sound->playSound(3, pos.x, pos.y, pos.z, 0, 0, 0);

		}

		//request make
		if((int)currentProj->projFlag & 1)
		{
//			fprintf(stderr,"Create projectile %d\n", currentProj->index + OFFSET_PROJECTILE_TYPE_1);					
			psm->makePSMRequest("E_PROJECTILE_TYPE_1", currentProj, OFFSET_PROJECTILE_TYPE_1 + currentProj->index);
			
			//play sound at the correctSpot
			D3DXVECTOR3 pos = currentProj->position;

//			cerr<<"PLAYER:::: X:"<<pPos.x<<" :: Y ::"<<pPos.y<<" :: Z ::"<<pPos.z<<endl;
//			cerr<<"CREATE PROJ:::: X:"<<pos.x<<" :: Y ::"<<pos.y<<" :: Z ::"<<pos.z<<endl;

			//don't know how to get the rectang position from projectile
			//play creation from the player... HACKHACKHACK HACK
			//sound->playSound(2, pPos.x, pPos.y, pPos.z, 0, 0, 0);


		}

		//reset flag
		currentProj->projFlag = 0;

		
	}/*while(currentProj->sharedInfo->next != -1 && 
		(currentProj = &objectManager->privStartProjectile[currentProj->sharedInfo->next]));*/


	//draw planet	
	//////////////////////////////////////////////////////////////////////////	
	World * currentWorld	= objectManager->getWorldpList();
	
	if(graphicsManager->world == NULL)
		return;

	graphicsManager->world->effect->SetTechnique("RenderMed");		
	
	currentWorld->updatePosition();
	currentWorld->drawMesh(graphicsManager->world,graphicsManager, dLightColor, dLightPos, numLights);		
	
	graphicsManager->world->effect->End();
	

	//draw sky box
	//////////////////////////////////////////////////////////////////////////	
	if(graphicsManager->sky == NULL)
		return;

	graphicsManager->sky->effect->SetTechnique( "Skybox" );		
	objectManager->skyBox->Render(pd3dDevice, graphicsManager->sky->effect, graphicsManager);	
	graphicsManager->sky->effect->End();



	//draw Ocean
	//////////////////////////////////////////////////////////////////////////
	if(graphicsManager->ocean == NULL)
		return;
	graphicsManager->ocean->effect->SetTechnique( "RenderMed" );
	//objectManager->ocean->drawMesh(graphicsManager->ocean,graphicsManager, dLightColor, dLightPos, numLights);
	graphicsManager->ocean->effect->End();	


	//draw particle system
	//////////////////////////////////////////////////////////////////////////
	psm->render(graphicsManager->cam);

	//draw HUD
	hudManager->render();


	//////////////////////////////////////////////////////////////////////////
	pd3dDevice->EndScene();
    pd3dDevice->Present( NULL, NULL, NULL, NULL );
}



//--------------------------------------------------------------------------------------
// This callback function will be called immediately after the Direct3D device has 
// been destroyed, which generally happens as a result of application termination or 
// windowed/full screen toggles. Resources created in the OnCreateDevice callback 
// should be released here, which generally includes all D3DPOOL_MANAGED resources. 
//--------------------------------------------------------------------------------------
void ReleaseResource()
{
    int num = 0;
   
	//release effects 
	if(graphicsManager != NULL )
		delete graphicsManager;
	
	//release config manager
	delete cManager;
	delete config;

	
	if(psm != NULL)
		delete psm;

	if(hudManager != NULL)
		delete hudManager;

	if( (num =pd3dDevice->Release()) > 0 )  
    {
		cerr<<"DX9 Resources not released: "<<num<<"\nPress Enter"<<endl;
		getchar();
    }
	
}
