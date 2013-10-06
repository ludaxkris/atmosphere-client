//-----------------------------------------------------------------------------
// File: SkyBox.fx
//
// Desc: 
// 
// Copyright (c) Microsoft Corporation. All rights reserved.
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Global variables
//-----------------------------------------------------------------------------
float4x4 g_WorldViewProjection;
float4x4 g_mWorld;
float4x4 mView;
float g_fAlpha;
float g_fScale;

texture cubeTexture;

sampler CubeTexture = sampler_state
{ 
    Texture = (cubeTexture);
    MipFilter = LINEAR;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
    ADDRESSU = Clamp;
    ADDRESSV = Clamp;
};


//-----------------------------------------------------------------------------
// Skybox stuff
//-----------------------------------------------------------------------------
struct SkyboxVS_Input
{
    float4 Pos : POSITION;
};

struct SkyboxVS_Output
{
    float4 Pos : POSITION;
    float3 Tex : TEXCOORD0;
};

SkyboxVS_Output SkyboxVS( SkyboxVS_Input Input )
{
    SkyboxVS_Output Output;
	float4x4 matViewNoTrans = float4x4(mView[0], 
							 mView[1],
							 mView[2],
							 float4(0,0,0,1));		
	   
    //Output.Pos = mul(Input.Pos, matViewNoTrans);           
    Output.Pos = mul(Input.Pos, g_WorldViewProjection);
    Output.Tex =  Input.Pos;
      
    return Output;
}

float4 SkyboxPS( SkyboxVS_Output Input ) : COLOR
{
    float4 color = texCUBE( CubeTexture, normalize(Input.Tex));
    color.a = 1;
    return color;
}

technique Skybox
{
    pass p0
    {
        VertexShader = compile vs_2_0 SkyboxVS();
        PixelShader = compile ps_2_0 SkyboxPS();
    }
}




 