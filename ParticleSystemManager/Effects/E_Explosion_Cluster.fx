//--------------------------------------------------------------------------------------
// Global variables
//--------------------------------------------------------------------------------------


float4 v_MaterialAmbientColor;			// Material's ambient color
float4 v_MaterialDiffuseColor;			// Material's diffuse color
texture t_MeshTexture;              // Color texture for mesh
texture t_colorMap;

float4x4 m_View;
float4x4 m_WorldViewProjection;    // World * View * Projection matrix


//--------------------------------------------------------------------------------------
// Texture samplers
//--------------------------------------------------------------------------------------
sampler MeshTextureSampler = 
sampler_state
{

    Texture = <t_MeshTexture>;
    MipFilter = LINEAR;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
};

sampler colorMapTex = sampler_state
{
    Texture = <t_colorMap>;
    MipFilter = LINEAR;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
    
    ADDRESSU = Clamp;
    ADDRESSV = Clamp;
};


//--------------------------------------------------------------------------------------
// Vertex shader output structure
//--------------------------------------------------------------------------------------
struct VS_Output{
	float4 Position : POSITION; //vertex position
	float4 Color: COLOR0; //vertex diffuse color
	float1 PSize: PSIZE;
	float2 DiffuseUV: TEXCOORD0; //vertex texture coords
};

//------------------------ --------------------------------------------------------------
// This shader computes standard transform and lighting
//--------------------------------------------------------------------------------------
VS_Output RenderParticleVS( float4 v_Pos: POSITION,
				            float4 v_Color: COLOR,
				            float1 PSize: PSIZE0,
				            float2 DiffuseUV: TEXCOORD0
							)
{
	VS_Output output;
	//float4x4 temp = mul(m_World, m_WorldViewProjection);
	//output.Position = mul(v_Pos, temp);
	output.Position = mul(v_Pos, m_WorldViewProjection);
	output.Color = v_Color;
	
	output.PSize = PSize;
	output.DiffuseUV = DiffuseUV;
	

	return output;
}

//--------------------------------------------------------------------------------------
// Pixel shader output structure
//--------------------------------------------------------------------------------------
struct PS_Output{
	float4 Color: COLOR0;
};


//--------------------------------------------------------------------------------------
// This shader outputs the pixel's color by modulating the texture's
//       color with diffuse material color
//--------------------------------------------------------------------------------------
PS_Output RenderParticlePS(VS_Output In) {

	PS_Output output;
	output.Color= In.Color;
	output.Color.rgb = float4(tex1D(colorMapTex, 1-In.Color.a )).rgb;
	output.Color.a = tex2D(MeshTextureSampler, In.DiffuseUV) * In.Color.a;
	return output;
}


//--------------------------------------------------------------------------------------
// Renders scene to render target
//--------------------------------------------------------------------------------------
technique RenderTest
{
    pass P0
    {          
		PointSpriteEnable = true;
		Wrap0 = 0;
		AlphaBlendEnable = true;
		SrcBlend = SrcAlpha;
		DestBlend = InvSrcAlpha;
		//
		ZWriteEnable = false;
		
        VertexShader = compile vs_2_0 RenderParticleVS();
        PixelShader  = compile ps_2_0 RenderParticlePS();
    }
    
    pass STATERESTORE
    {
		PointSpriteEnable = false;
		AlphaBlendEnable = false;
		ZWriteEnable = true;
    }
}