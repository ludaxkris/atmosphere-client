//--------------------------------------------------------------------------------------
// Global variables
//--------------------------------------------------------------------------------------
float4 g_MaterialAmbientColor;      // Material's ambient color
float4 g_MaterialDiffuseColor;      // Material's diffuse color

float3 sunLightDir[4];              // Light's direction in world space
float4 sunLightDiffuse[4];          // Light's diffuse color
float4 sunLightAmbient[4];          // Light's ambient color
int numSunLights;					// Number of Sun lights

texture g_MeshTexture;              // Color texture for mesh

float4x4 g_mWorld;                  // World matrix for object
float4x4 g_mWorldViewProjection;    // World * View * Projection matrix
//--------------------------------------------------------------------------------------
// Texture samplers
//--------------------------------------------------------------------------------------
sampler MeshTextureSampler = 
sampler_state
{
    Texture = <g_MeshTexture>;
    MipFilter = LINEAR;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
};


//--------------------------------------------------------------------------------------
// Vertex shader output structure
//--------------------------------------------------------------------------------------
struct INPUT
{ 
    float4 position   : POSITION0;
    float3 normal	  : NORMAL;
    float2 texUV       : TEXCOORD0;
    float3 tangent    : TANGENT;
    float3 binormal	  : BINORMAL;
};

struct VS_OUTPUT
{
    float4 Position		: POSITION;   // vertex position 
    float2 TextureUV	: TEXCOORD0;  // vertex texture coords 
    float3 Normal		: TEXCOORD1;  // vertex normal
    float3 PPosition	: TEXCOORD2;  // Pixel coordinates
};

//--------------------------------------------------------------------------------------
// This shader computes standard transform and lighting
//--------------------------------------------------------------------------------------
VS_OUTPUT RenderSceneVS( in INPUT In)
{
    VS_OUTPUT Output;
   
    // Transform the position from object space to homogeneous projection space
    Output.Position		= mul(In.position , g_mWorldViewProjection);
       
    // Just copy the texture coordinate through
    Output.TextureUV = In.texUV; 
    
    //Pass values to the pixel shader
    Output.Normal		= mul(In.normal, g_mWorld);
	//Output.PPosition	= mul(In.position, g_mWorldViewProjection).xyz;
	Output.PPosition	= mul(In.position , g_mWorld).xyz;
    
    return Output;  
}


//--------------------------------------------------------------------------------------
// Pixel shader output structure
//--------------------------------------------------------------------------------------
struct PS_OUTPUT
{
    float4 RGBColor : COLOR0;  // Pixel color    
};


//--------------------------------------------------------------------------------------
// This shader outputs the pixel's color by modulating the texture's
//       color with diffuse material color
//--------------------------------------------------------------------------------------
PS_OUTPUT RenderScenePS( VS_OUTPUT In,
                         uniform bool bTexture ) 
{ 
    PS_OUTPUT Output;
    
    //Setup
    float4 diffuse	= float4(0,0,0,0);
	float3 LDiffuse = float3(0,0,0);
	
	//sunlight
    for(int i = 0; i < 4; i++)
		LDiffuse += max(0, dot(normalize(In.Normal), normalize(sunLightDir[i]))*.5);
		
		
	diffuse.rgb	  =	 g_MaterialDiffuseColor * LDiffuse;  	
	diffuse.w = 1; 
	
    // Lookup mesh texture and modulate it with diffuse
    Output.RGBColor = diffuse * g_MaterialDiffuseColor; //tex2D(MeshTextureSampler, In.TextureUV) * diffuse; //

    return Output;
}


//--------------------------------------------------------------------------------------
// Renders scene to render target
//--------------------------------------------------------------------------------------
technique RenderSceneWithTexture1Light
{
    pass P0
    {          
        VertexShader = compile vs_2_0 RenderSceneVS();
        PixelShader  = compile ps_2_0 RenderScenePS( true ); // trivial pixel shader (could use FF instead if desired)
    }
}