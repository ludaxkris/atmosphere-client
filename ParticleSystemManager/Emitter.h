#pragma once
#include <string>
#include <vector>

#include <d3d9.h>
#include <d3dx9tex.h>
#include <d3d9types.h>

#include "./../GraphicsManager/MyEffect.h"
#include "./../Objects/SpaceObject.h"

#include "ParticleHandler.h"
#include "Particle.h"
class Particle;


class Emitter
{
	struct CUSTOMVERTEX
	{
		D3DXVECTOR3 v_psPosition;
		D3DCOLOR v_color; 
		//alpha value is placed into the 
		//last slot of color
		float f_psize;
	};

	#define D3DFVF_CUSTOMVERTEX (D3DFVF_XYZ|D3DFVF_DIFFUSE)
	
public:
	Emitter();
	Emitter(DWORD * globalTime);
	~Emitter(void);
	void initEmitter(LPDIRECT3DDEVICE9 p_Device, int i_nParticles, 
					 int f_emitTTL, int f_emitPeriod, 
					 int i_emitQty, float f_emitSpeed, 
					 D3DXVECTOR2 v_emitSpread, float f_emitDisp, 
					 LPDIRECT3DTEXTURE9 p_texture, MyEffect * p_effect,
					 LPDIRECT3DTEXTURE9 p_colorMap);

	void initWorld(D3DXVECTOR3 v_pos, D3DXVECTOR3 v_dir);
	/*
	void initParticles(D3DXVECTOR4 v_color, float f_partTTL);
	*/
	virtual void initParticles(D3DXVECTOR3 v_color, int f_partTTL,
							   float f_initPSize, float f_initAlpha) = 0;

	void setWorldTransform();

	void setNumParticles(int i_nParticles);	//set the number of particles

	/*void update(void);
	void decayAlpha(void);*/
	virtual void update(SpaceObject * p_spaceObj) = 0;
	virtual void decayAlpha(void) = 0;

	void FillVertexBuffer();

	void pre_render(void);
	void renderFFP(void);	//render the particles in this emitter
	void post_render(void);

	void renderEffect(Camera * p_cam, SpaceObject * p_spaceObj);
	void updateEffect(ID3DXEffect * p_DXeffect, Camera * p_cam, SpaceObject * p_spaceObj);

	bool b_constant;

	inline void setActive (void) {b_Active = true;}
	inline void setDead (void){b_Active = false;}

	inline void setDying (void) {b_Dying = true;}
	inline void setNotDying (void) {b_Dying = false;}
	
	inline bool isActive(void) {return b_Active;}
	inline bool isDying(void) {return b_Dying;}
	inline DWORD FLOAT_TO_DWORD (FLOAT f) {return *((DWORD*)&f);}

	inline int getTTL(void){return f_emitTTL;};
	inline float getFullRand(void){return (((float)rand()/ RAND_MAX)+((float)rand()/ RAND_MAX)-1.0f);}
	inline float getPosRand(void){return ((float)rand()/ RAND_MAX);}
	inline float getNegRand(void){return ((float)rand()/ RAND_MAX)-1.0f;}
	void setCTime(DWORD * globalTime);

	bool Emitter::hasLivingParts();

protected:

	//num of particles in this emitter
	int i_numParticles;

	D3DXMATRIX m_World;
	D3DXMATRIX m_OrientAndPos;

	ParticleHandler * p_partHandler;

	DWORD * globalTime;	//global timer

	int f_startTime;
	int f_emitPeriod;	//number of particles per milisecond

	int i_emitQty;

	float f_emitSpeed;
	D3DXVECTOR2 v_emitSpread;
	float f_emitDisp;

private:

	
	
	
	LPDIRECT3DDEVICE9 p_emitterDevice;	//store a copy of the device

	D3DXVECTOR3 v_curPos;	//current particle position
	D3DXVECTOR3 v_curVel;	//currentdirection and velocity

	//vertex buffer to hold sprites
	LPDIRECT3DVERTEXBUFFER9 p_vertexBuffer;

	//set the texture that will be applied to each particle
	LPDIRECT3DTEXTURE9 p_Texture;
	LPDIRECT3DTEXTURE9 p_ColorMap;
	MyEffect * p_effect;

	bool b_Active;		//living id
	bool b_Dying;

	int f_emitTTL;	//emitter lifespan
	
	LPDIRECT3DVERTEXBUFFER9 createVertexBuffer();

	//pointer to the particle type array used for creation
	//Particle * p_particles;

};