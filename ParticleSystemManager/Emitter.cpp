#include "Emitter.h"

IDirect3DVertexDeclaration9* p_VertDecl = NULL;

const D3DVERTEXELEMENT9 Decl[] =
{
    { 0, 0,  D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
    { 0, 12, D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR,   0 },
	{ 0, 16, D3DDECLTYPE_FLOAT1, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_PSIZE,   0 },
	{ 0, 20, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD,   0 },
    D3DDECL_END()
};

Emitter::Emitter(){}

Emitter::Emitter(DWORD * globalTime)
{
	this->globalTime = globalTime;
}

Emitter::~Emitter(void)
{
	this->globalTime = NULL;
	if(p_partHandler)
		delete p_partHandler;
	SAFE_RELEASE(p_vertexBuffer);
	SAFE_RELEASE(p_VertDecl);

	p_emitterDevice = NULL;
	p_Texture = NULL;
	p_effect = NULL;
}

void Emitter::initEmitter(LPDIRECT3DDEVICE9 p_Device, int i_nParticles, 
							int f_emitTTL, int f_emitPeriod, 
							int i_emitQty, float f_emitSpeed, 
							D3DXVECTOR2 v_emitSpread, float f_emitDisp, 
							LPDIRECT3DTEXTURE9 p_texture, MyEffect * p_effect,
							LPDIRECT3DTEXTURE9 p_colorMap)
{
	this->p_emitterDevice = p_Device;
	this->i_numParticles = i_nParticles;
	this->p_effect = p_effect;
	this->f_emitTTL = f_emitTTL;
	this->f_emitPeriod = f_emitPeriod;
	this->i_emitQty = i_emitQty;
	this->f_emitSpeed = f_emitSpeed;
	this->v_emitSpread = v_emitSpread;
	this->f_emitDisp = f_emitDisp;
	this->p_Texture = p_texture;
	this->p_effect = p_effect;
	this->p_ColorMap = p_colorMap;

	p_partHandler = new ParticleHandler(globalTime);
	p_partHandler->initHandler(i_numParticles);

	p_vertexBuffer = createVertexBuffer();
	if(!p_VertDecl)
		p_emitterDevice->CreateVertexDeclaration( Decl, &p_VertDecl );

	D3DXMatrixIdentity(&m_OrientAndPos);
	D3DXMatrixIdentity(&m_World);

	f_startTime = *globalTime;

	setDead();
}

void Emitter::initWorld(D3DXVECTOR3 v_pos, D3DXVECTOR3 v_dir)
{
	D3DXMATRIX m_Temp;
	D3DXMatrixIdentity(&m_Temp);

	D3DXVECTOR3 v_z = D3DXVECTOR3(0,0,1);

	D3DXVECTOR3 v_arbRot;
	D3DXVec3Normalize(&v_dir, &v_dir);
	D3DXVec3Cross(&v_arbRot, &v_z ,&v_dir);

	float f_dotRes = D3DXVec3Dot(&v_dir, &v_z);
	
	float f_rad = 0;
	if(D3DXVec3Length(&v_arbRot)==0)
	{
		if(f_dotRes<0)
		{
			v_arbRot = D3DXVECTOR3(0,1,0);
			f_rad = D3DX_PI;
		}
	}
	else
		f_rad = acosf(f_dotRes);
	
	*((D3DXVECTOR3 *)(&m_OrientAndPos._41)) = v_pos;

	D3DXMatrixRotationAxis(&m_Temp, &v_arbRot, f_rad);

	m_OrientAndPos = m_Temp * m_OrientAndPos;
}

void Emitter::setWorldTransform()
{
	m_World = m_OrientAndPos;
}

LPDIRECT3DVERTEXBUFFER9 Emitter::createVertexBuffer()
{
	LPDIRECT3DVERTEXBUFFER9 p_vb;
	if(p_emitterDevice->CreateVertexBuffer( 
							i_numParticles * sizeof(CUSTOMVERTEX), 
							D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY | D3DUSAGE_POINTS,
							0,
							D3DPOOL_DEFAULT, &p_vb, NULL ) < 0 )
	{
		printf ("Error creataing vertex buffer\n");
	}
	return p_vb;
}

void Emitter::FillVertexBuffer()
{
	CUSTOMVERTEX * p_pointVertices; 
	D3DXVECTOR3 v_actPos, v_actColor;
	D3DXMATRIX m_owner;
	float f_alpha;
	p_vertexBuffer->Lock(0, i_numParticles * sizeof(CUSTOMVERTEX),
						(void**)&p_pointVertices, D3DLOCK_DISCARD);

	for(unsigned int i = 0; i< p_partHandler->getAliveSize(); ++i)
	{
		//position
		v_actPos = p_partHandler->getActivePartPosition(i) + p_partHandler->getActivePartVelocity(i) * ((float)p_partHandler->getActivePartElapsedTime(i));
		m_owner = p_partHandler->getActivePartMatrix(i);
		D3DXVec3TransformCoord(&v_actPos, &v_actPos, &m_owner);
		p_pointVertices->v_psPosition = v_actPos;

		//color & alpha
		v_actColor = p_partHandler->getActivePartColor(i);
		f_alpha = p_partHandler->getActivePartAlpha(i);
		p_pointVertices->v_color = D3DCOLOR_COLORVALUE(v_actColor.x, v_actColor.y, v_actColor.z, f_alpha);

		//PSize
		p_pointVertices->f_psize = p_partHandler->getActivePartSize(i);

		p_pointVertices++;
	}
	p_vertexBuffer->Unlock();
}

void Emitter::renderFFP(void)
{
	pre_render();

	p_emitterDevice->DrawPrimitive(D3DPT_POINTLIST, 0, i_numParticles);

	post_render();
}

void Emitter::pre_render(void)
{
	p_emitterDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	p_emitterDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	p_emitterDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	p_emitterDevice->SetRenderState(D3DRS_POINTSPRITEENABLE, TRUE);
	p_emitterDevice->SetRenderState(D3DRS_POINTSCALEENABLE, TRUE);

	p_emitterDevice->SetRenderState(D3DRS_POINTSIZE, FLOAT_TO_DWORD(1.0f));
	p_emitterDevice->SetRenderState(D3DRS_POINTSIZE_MIN, FLOAT_TO_DWORD(1.0f));

	p_emitterDevice->SetRenderState(D3DRS_POINTSCALE_A, FLOAT_TO_DWORD(0.0f));
	p_emitterDevice->SetRenderState(D3DRS_POINTSCALE_B, FLOAT_TO_DWORD(0.0f));
	p_emitterDevice->SetRenderState(D3DRS_POINTSCALE_C, FLOAT_TO_DWORD(1.0f));

	p_emitterDevice->SetTexture(0, p_Texture);

	p_emitterDevice->SetStreamSource(0, p_vertexBuffer, 0, sizeof(CUSTOMVERTEX));
	p_emitterDevice->SetFVF(D3DFVF_CUSTOMVERTEX);
}


void Emitter::post_render(void)
{
	p_emitterDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	p_emitterDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	p_emitterDevice->SetRenderState(D3DRS_POINTSPRITEENABLE, FALSE);
	p_emitterDevice->SetRenderState(D3DRS_POINTSCALEENABLE, FALSE);
}

void Emitter::renderEffect(Camera * p_cam, SpaceObject * p_spaceObj)
{
	ID3DXEffect * p_DXeffect = p_effect->effect;
	if(p_DXeffect == NULL)
		return;

	p_DXeffect->SetTechnique( "RenderTest" );

	updateEffect(p_DXeffect, p_cam, p_spaceObj);

	UINT cPasses;

	p_emitterDevice->SetStreamSource(0, p_vertexBuffer, 0, sizeof(CUSTOMVERTEX));
	p_emitterDevice->SetVertexDeclaration(p_VertDecl);
	//p_emitterDevice->SetFVF(D3DFVF_CUSTOMVERTEX);

	p_DXeffect->Begin(&cPasses, 0);

	
	for (UINT iPass = 0; iPass < cPasses; iPass++)
	{
		
		//p_DXeffect->CommitChanges();	
		
		p_DXeffect->BeginPass(0);
		p_emitterDevice->DrawPrimitive(D3DPT_POINTLIST, 0, (UINT)p_partHandler->getAliveSize());
		p_DXeffect->EndPass();
		
		//float f_alpha;// f_psize;
		/*D3DXMATRIX m_OwnerWorld;
		p_DXeffect->BeginPass(0);

		for(unsigned int i = 0; i<p_partHandler->getAliveSize(); i++)
		{
			//m_OwnerWorld = p_partHandler->getActivePartMatrix(i);

			//p_DXeffect->SetMatrix( "m_World", &m_OwnerWorld );
			//p_DXeffect->SetValue( "g_alpha", &f_alpha, sizeof(float));
			//p_DXeffect->SetValue( "g_psize", &f_psize, sizeof(float));

			p_DXeffect->CommitChanges();
			p_emitterDevice->DrawPrimitive(D3DPT_POINTLIST, i, 1);
		}
		p_DXeffect->EndPass();*/
		
	}
	p_DXeffect->End();

}

void Emitter::updateEffect(ID3DXEffect * p_DXeffect, Camera * p_cam,
						   SpaceObject * p_spaceObj)
{
	D3DXMATRIXA16 m_WorldViewProjection;
	//D3DXVECTOR3 v_LightDir[MAX_LIGHTS];
	//D3DXCOLOR   v_LightDiffuse[MAX_LIGHTS];
	D3DXMATRIXA16 m_OwnerWorld;
	D3DXMATRIXA16 m_View;
	D3DXMATRIXA16 m_Proj;
	D3DXMatrixIdentity(&m_View);

	//setWorldTransform();
	D3DXVECTOR3	temp;
	D3DXVECTOR3	temp1;
	D3DXVECTOR3	temp2;
	D3DXVECTOR3	temp3;
/*
	m_OwnerWorld = p_spaceObj->matTransform;

	temp =  *((D3DXVECTOR3 *)&(p_spaceObj->matTransform._11));
	D3DXVec3Normalize(&temp, &temp);
	*((D3DXVECTOR3 *)&(m_OwnerWorld._11)) = temp;

	temp =  *((D3DXVECTOR3 *)&(p_spaceObj->matTransform._21));
	D3DXVec3Normalize(&temp, &temp);
	*((D3DXVECTOR3 *)&(m_OwnerWorld._21)) = temp;

	temp =  *((D3DXVECTOR3 *)&(p_spaceObj->matTransform._31));
	D3DXVec3Normalize(&temp, &temp);
	*((D3DXVECTOR3 *)&(m_OwnerWorld._31)) = temp;
*/

	// Get the projection & view matrix from the camera class
	m_Proj = p_cam->GetProjMatrix();
	m_View = p_cam->GetViewMatrix();
	//m_WorldViewProjection = m_OwnerWorld * m_View * m_Proj;
	m_WorldViewProjection = m_View * m_Proj;

	/*//initialize effect globals
	vLightDir[0]		= gManager->lights[0].Direction;
	vLightDiffuse[0]	= gManager->lights[0].Diffuse;
	*/

	D3DXCOLOR v_colorMtrlDiffuse(1.0f, 1.0f, 1.0f, 1.0f);
	D3DXCOLOR v_colorMtrlAmbient(0.35f, 0.35f, 0.35f, 0);

	p_DXeffect->SetMatrix	( "m_WorldViewProjection", &m_WorldViewProjection );
	p_DXeffect->SetMatrix	( "m_View", &m_View );
	//p_DXeffect->SetMatrix	( "m_World", &m_World );

	/*
	effect->SetValue	( "g_LightDir", vLightDir, sizeof(D3DXVECTOR3)*MAX_LIGHTS );
	effect->SetValue	( "g_LightDiffuse", vLightDiffuse, sizeof(D3DXVECTOR4)*MAX_LIGHTS );
	effect->SetInt		( "g_nNumLights", 1 );    
	*/

	//set material and texture
	p_DXeffect->SetValue	( "v_MaterialAmbientColor", &v_colorMtrlAmbient, sizeof(D3DXCOLOR) );
	p_DXeffect->SetValue	( "v_MaterialDiffuseColor", &v_colorMtrlDiffuse, sizeof(D3DXCOLOR) );    
	p_DXeffect->SetTexture	( "t_MeshTexture", p_Texture);
	p_DXeffect->SetTexture	( "t_colorMap", p_ColorMap);
	
	///****************
	//float f_alpha = 1.0;
	//p_DXeffect->SetValue( "g_alpha", &f_alpha, sizeof(float));
	//*******************
}

void Emitter::setCTime(DWORD * globalTime)
{
	this->globalTime = globalTime;
}

bool Emitter::hasLivingParts()
{
	if(p_partHandler->getAliveSize()!=0) 
		return true;
	else
		return false;
}