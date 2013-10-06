//--------------------------------------------------------------------------------------
// Global variables
//--------------------------------------------------------------------------------------

//global lights
float3 sunLightDir[4];              // Light's direction in world space
float4 sunLightDiffuse[4];          // Light's diffuse color
float4 sunLightAmbient[4];          // Light's ambient color
int numSunLights;					// Number of Sun lights


float4 planetMatAmbient;			// Material's ambient color
float4 planetMatDiffuse;			// Material's diffuse color

//textures
texture colorMap;					// Color map for the terrain
texture normalMap;					// Color map for the terrain
texture cubeTexture;				// light cube map

//globals
float4 camPos;						// Camera Position
float4x4 g_mWorld;                  // World matrix for object
float4x4 g_mWorldViewProjection;    // World * View * Projection matrix

//other lightsources
float3	lightPositions[4];			// Position of ship
float4	lightDiffuse[4];			// Ship light color
int		numLights;					// number of ships

//--------------------------------------------------------------------------------------
// Texture samplers
//--------------------------------------------------------------------------------------
sampler normalMapTex = sampler_state
{
    Texture = <normalMap>;
    MipFilter = LINEAR;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
    
   // ADDRESSU = Clamp;
    //ADDRESSV = Clamp;
};

sampler colorMapTex = sampler_state
{
    Texture = <colorMap>;
    MipFilter = LINEAR;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
    
    ADDRESSU = Clamp;
    ADDRESSV = Clamp;
};


sampler CubeTexture = sampler_state
{ 
    Texture = (cubeTexture);
    MipFilter = LINEAR;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
    ADDRESSU = Clamp;
    ADDRESSV = Clamp;
};


//--------------------------------------------------------------------------------------
// Vertex shader output structure
//--------------------------------------------------------------------------------------
struct INPUT
{ 
    float4 position   : POSITION0;
    float3 normal	  : NORMAL;
    float2 texUV      : TEXCOORD0;
    float3 tangent    : TANGENT;
    float3 binormal	  : BINORMAL;
};

struct VS_OUTPUT
{
    float4 Position		: POSITION;   // vertex position 
    float2 TextureUV	: TEXCOORD0;  // vertex texture coords 
    float3 Normal		: TEXCOORD1;  // vertex normal
    float3 PPosition	: TEXCOORD2;  // Pixel coordinates
    float3 vLight		: TEXCOORD3;  // tangent
    float3x3 mat		: TEXCOORD4;
};


//vertex to pixel shader structure
struct v2p
{
     float4 position	: POSITION;
     float2 tex0		: TEXCOORD0;
     float3 binormal	: TEXCOORD1;
     float3 tangent		: TEXCOORD2;  
     float3 normal		: TEXCOORD3;     
     float3 PPosition	: TEXCOORD4;
};

struct PS_OUTPUT
{
    float4 RGBColor : COLOR0;  // Pixel color    
};


//--------------------------------------------------------------------------------------
// This shader computes standard transform and lighting
//--------------------------------------------------------------------------------------
v2p RenderSceneVS1( in INPUT IN)
{
	v2p OUT;
	
    //getting to position to object space
    OUT.position = mul(IN.position, g_mWorldViewProjection);

    //getting the position of the vertex in the world
    float4 posWorld = mul(IN.position, g_mWorld);
	OUT.PPosition	= posWorld;

    //calculating the binormal and setting the Tangent Binormal and Normal matrix
    //float3x3 TBNMatrix = float3x3(IN.tangent, IN.binormal , IN.normal);

	OUT.binormal = mul(IN.binormal , g_mWorld);
    OUT.normal = mul(IN.normal , g_mWorld);
    OUT.tangent = mul(IN.tangent , g_mWorld);
 
    OUT.tex0 = IN.texUV*5;   
   
    return OUT;
}

//--------------------------------------------------------------------------------------
// This shader outputs the pixel's color by modulating the texture's
//       color with diffuse material color
//--------------------------------------------------------------------------------------
PS_OUTPUT PSMed(in v2p IN, uniform int numSunLight) 
{ 
    PS_OUTPUT OUT;
    
    //Move normal map to worl space
    //////////////////////////////////////////////////////////	
    float3x3 TBNMatrix	= float3x3(		normalize(IN.tangent),				  
										normalize(IN.binormal),
										normalize(IN.normal)							  						   										
								  );    
	TBNMatrix= transpose(TBNMatrix);										// inverse the matrix
    float3 nNormal		= 2.0f * tex2D(normalMapTex, IN.tex0).rgb - 1.0f;	// normal map normals   
    nNormal				= mul(TBNMatrix,nNormal);							// transform to world normal    
    
    
    //ship's light source
    //////////////////////////////////////////////////////////
    float3 lightDir		= float3(0,0,0);	    
	float3 shipDiffuse	= float3(0,0,0);
	float len; 								
        
    for(int i=0;i<4; i++)
	{
		lightDir		= lightPositions[i] - IN.PPosition;	    
		len				= length(lightDir); 								
		lightDir		/= len;	
	
		//Diffuse from ship
		shipDiffuse		+= lightDiffuse[i] * saturate(dot(nNormal, lightDir))*  (10/(len));
	}       
    

	//calculate reflection
	//////////////////////////////////////////////////////////
	//loock vector to pixel
	
    //sunlight
    //////////////////////////////////////////////////////////
    float3 sDiffuse		= float3(0,0,0);
	sDiffuse			+= (1-sDiffuse)* saturate(dot(nNormal, sunLightDir[0]));
	sDiffuse			+= (1-sDiffuse)* saturate(dot(nNormal, sunLightDir[1]));
	
	//set color
	//////////////////////////////////////////////////////////
	//float  height = (length(IN.PPosition.xyz)-18)/3.6;	//value between 0 and 1	
	float  height = (length(IN.PPosition.xyz)-44.8)/6.9;	//value between 0 and 1
	float4 color  = float4(tex1D(colorMapTex, height));
	
    OUT.RGBColor.rgb	= color*sDiffuse+shipDiffuse;
    OUT.RGBColor.a		= 1;
      
    return OUT;
}

PS_OUTPUT PSHigh(in v2p IN, uniform int numSunLight) 
{ 
    PS_OUTPUT OUT;
    
    //Move normal map to worl space
    //////////////////////////////////////////////////////////	
    float3x3 TBNMatrix	= float3x3(		normalize(IN.tangent),				  
										normalize(IN.binormal),
										normalize(IN.normal)							  						   										
								  );    
	TBNMatrix= transpose(TBNMatrix);										// inverse the matrix
    float3 nNormal		= 2.0f * tex2D(normalMapTex, IN.tex0).rgb - 1.0f;	// normal map normals   
    nNormal				= mul(TBNMatrix,nNormal);							// transform to world normal    
    
    //look vector to pixel
	float3 V	= normalize(IN.PPosition-camPos.xyz);
    
      //sunlight
    //////////////////////////////////////////////////////////
    float3 sDiffuse		= float3(0,0,0);
	sDiffuse			+= (1-sDiffuse)* saturate(dot(nNormal, sunLightDir[0]));
	sDiffuse			+= (1-sDiffuse)* saturate(dot(nNormal, sunLightDir[1]));
	sDiffuse			= pow(sDiffuse,2.5);	
	
    //ship's light source
    //////////////////////////////////////////////////////////
    float3 lightDir		= float3(0,0,0);	    
	float3 shipDiffuse	= float3(0,0,0);
	float len; 								
        
    float3 specularShip	= float3(0,0,0);
	float3 RShip;
	    
	//shadow    
	float shadow		= float3(0,0,0);
	float dim			= 0;
	float light			= 0;
	    
    for(int i=0;i<4; i++)
	{
		lightDir		= lightPositions[i] - IN.PPosition;	    
		len				= length(lightDir); 								
		lightDir		/= len;	
	
	
		//cacl specular for ship
		RShip			= lightDir-2*(dot(nNormal,lightDir)*nNormal);
		specularShip	+= pow(max(0,dot(RShip,V)),5) * .000000030;
		
		//Diffuse from ship
		light			= pow(saturate(dot(nNormal, lightDir)),2);
		shipDiffuse		+= (1-sDiffuse)*lightDiffuse[i] * light *  (10/len);
		
		dim				= pow(saturate(dot(lightDir,nNormal)),2);
		light			= pow(saturate(dot(lightDir, sunLightDir[0])),4);
		shadow			+= (1-shadow)* light * dim;		
		
		light			= pow(saturate(dot(lightDir, sunLightDir[1])),4);
		shadow			+= (1-shadow)* light * dim;		
		
	}       
    
    //smooth out shadow
	if(shadow < .999)
		shadow /= (1/(shadow*shadow*shadow));
	
	
	//shadow *= .955;

	//calculate reflection
	//////////////////////////////////////////////////////////
	float3 lookReflec	= V-2*(dot(nNormal,V)*nNormal);        	
	float3 reflection	= texCUBE( CubeTexture, normalize(lookReflec));
		
	//set color
	//////////////////////////////////////////////////////////
	//float  height = (length(IN.PPosition.xyz)-18)/3.6;	//value between 0 and 1
	float  height = (length(IN.PPosition.xyz)-44.8)/6.9;	//value between 0 and 1
	float4 color  = float4(tex1D(colorMapTex, height));
	
    OUT.RGBColor.rgb	= (reflection*.3)+color*(sDiffuse-shadow)+(shipDiffuse)+specularShip;
    OUT.RGBColor.a		= 1;
      
    return OUT;
}

//--------------------------------------------------------------------------------------
// Renders scene to render target
//--------------------------------------------------------------------------------------
technique RenderMed
{
    pass P0
    {          
        VertexShader = compile vs_2_0 RenderSceneVS1();
        PixelShader  = compile ps_2_0 PSMed(2); 
    }    
}

technique RenderHigh
{
    pass P0
    {          
        VertexShader = compile vs_2_0 RenderSceneVS1();
        PixelShader  = compile ps_3_0 PSHigh(2); 
    }    
}