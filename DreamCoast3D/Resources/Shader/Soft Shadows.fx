/*-----------------------------------------------------------------------------
	Name	: Soft Shadows.fx
	Desc	: Soft shadows effect file.
	Author	: Anirudh S Shastry. Copyright (c) 2004.
	Date	: 22nd June, 2004.
-----------------------------------------------------------------------------*/

//--------------------------------------------
// Global variables
//--------------------------------------------
matrix	g_matWorldViewProj	: WorldViewProjection;
matrix	g_matLightViewProj	: LightViewProjection;
matrix	g_matWorld			: World;
matrix	g_matWorldIT		: WorldInverseTranspose;
matrix	g_matTexture		: Texture;

vector	g_vLightPos			: LightPosition;
vector	g_vEyePos			: EyePosition;
vector	g_vLightColor		: LightColor;

float2  g_vSampleOffsets[15];
float   g_fSampleWeights[15];

texture tShadowMap;
texture tScreenMap;
texture tBlurHMap;
texture tBlurVMap;
texture	tColorMap;
texture tSpotMap;

sampler ShadowSampler = sampler_state
{
	texture = (tShadowMap);

	MipFilter = Linear;
	MinFilter = Linear;
	MagFilter = Linear;
	
	AddressU = Clamp;
	AddressV = Clamp;
};

sampler ScreenSampler = sampler_state
{
	texture = (tScreenMap);

	MipFilter = Linear;
	MinFilter = Linear;
	MagFilter = Linear;
	
	AddressU = Clamp;
	AddressV = Clamp;
};

sampler BlurHSampler = sampler_state
{
	texture = (tBlurHMap);

	MipFilter = Linear;
	MinFilter = Linear;
	MagFilter = Linear;
	
	AddressU = Clamp;
	AddressV = Clamp;
};

sampler BlurVSampler = sampler_state
{
	texture = (tBlurVMap);

	MipFilter = Linear;
	MinFilter = Linear;
	MagFilter = Linear;
	
	AddressU = Clamp;
	AddressV = Clamp;
};

sampler ColorSampler = sampler_state
{
	texture = (tColorMap);

	MipFilter = Linear;
	MinFilter = Linear;
	MagFilter = Linear;
	
	AddressU = Wrap;
	AddressV = Wrap;
};

sampler SpotSampler = sampler_state
{
	texture = (tSpotMap);

	MipFilter = Linear;
	MinFilter = Linear;
	MagFilter = Linear;
	
	AddressU = Clamp;
	AddressV = Clamp;
};

//--------------------------------------------
// Vertex shaders
//--------------------------------------------
struct VSOUTPUT_SHADOW
{
	float4 vPosition	: POSITION;
	float  fDepth		: TEXCOORD0;
};

// Shadow generation vertex shader
VSOUTPUT_SHADOW VS_Shadow( float4 inPosition : POSITION )
{
	// Output struct
	VSOUTPUT_SHADOW OUT = (VSOUTPUT_SHADOW)0;

	// Output the transformed position
	OUT.vPosition = mul( inPosition, g_matLightViewProj );

	// Output the scene depth
	OUT.fDepth = OUT.vPosition.z;

	return OUT;
}

// Shadow mapping vertex shader
struct VSOUTPUT_UNLIT
{
	float4 vPosition	: POSITION;
	float4 vTexCoord	: TEXCOORD0;
};

VSOUTPUT_UNLIT VS_Unlit( float4 inPosition : POSITION )
{
	// Output struct
	VSOUTPUT_UNLIT OUT = (VSOUTPUT_UNLIT)0;

	// Output the transformed position
	OUT.vPosition = mul( inPosition, g_matWorldViewProj );
		
	// Output the projective texture coordinates
	OUT.vTexCoord = mul( inPosition, g_matTexture );

	return OUT;
}

struct VSOUTPUT_BLUR
{
	float4 vPosition	: POSITION;
	float2 vTexCoord	: TEXCOORD0;
};

// Gaussian filter vertex shader
VSOUTPUT_BLUR VS_Blur( float4 inPosition : POSITION, float2 inTexCoord : TEXCOORD0 )
{
	// Output struct
	VSOUTPUT_BLUR OUT = (VSOUTPUT_BLUR)0;

	// Output the position
	OUT.vPosition = inPosition;

	// Output the texture coordinates
	OUT.vTexCoord = inTexCoord;

	return OUT;
}
		
struct VSOUTPUT_SCENE
{
	float4 vPosition		: POSITION;
	float2 vTexCoord		: TEXCOORD0;
	float4 vProjCoord		: TEXCOORD1;
	float4 vScreenCoord		: TEXCOORD2;
	float3 vNormal			: TEXCOORD3;
	float3 vLightVec		: TEXCOORD4;
	float3 vEyeVec			: TEXCOORD5;
};

// Scene pixel shader
VSOUTPUT_SCENE VS_Scene( float4 inPosition : POSITION, float3 inNormal : NORMAL, float2 inTexCoord : TEXCOORD0 )
{
	VSOUTPUT_SCENE OUT = (VSOUTPUT_SCENE)0;

	// Output the transformed position
	OUT.vPosition = mul( inPosition, g_matWorldViewProj );

	// Output the texture coordinates
	OUT.vTexCoord = inTexCoord;

	// Output the projective texture coordinates
	OUT.vProjCoord = mul( inPosition, g_matTexture );

	// Output the screen-space texture coordinates
	OUT.vScreenCoord.x = ( OUT.vPosition.x * 0.5 + OUT.vPosition.w * 0.5 );
	OUT.vScreenCoord.y = ( OUT.vPosition.w * 0.5 - OUT.vPosition.y * 0.5 );
	OUT.vScreenCoord.z = OUT.vPosition.w;
	OUT.vScreenCoord.w = OUT.vPosition.w;

	// Get the world space vertex position
	float4 vWorldPos = mul( inPosition, g_matWorld );

	// Output the world space normal
	OUT.vNormal = mul( inNormal, g_matWorldIT );

	// Move the light vector into tangent space
	OUT.vLightVec = g_vLightPos.xyz - vWorldPos.xyz;

	// Move the eye vector into tangent space
	OUT.vEyeVec = g_vEyePos.xyz - vWorldPos.xyz;
	
	return OUT;
}

//--------------------------------------------
// Pixel shaders
//--------------------------------------------
float4  PS_Shadow( VSOUTPUT_SHADOW IN ) : COLOR0
{
	// Output the scene depth
	return float4( IN.fDepth, IN.fDepth, IN.fDepth, 1.0f );
}
	
// Shadow mapping pixel shader
float4  PS_Unlit( VSOUTPUT_UNLIT IN ) : COLOR0
{
	// Generate the 9 texture co-ordinates for a 3x3 PCF kernel
	float4 vTexCoords[9];

	// Texel size
	float fTexelSize = 1.0f / 512.0f;

	// Generate the tecture co-ordinates for the specified depth-map size
	// 4 3 5
	// 1 0 2
	// 7 6 8
	vTexCoords[0] = IN.vTexCoord;

	vTexCoords[1] = IN.vTexCoord + float4( -fTexelSize, 0.0f, 0.0f, 0.0f );
	vTexCoords[2] = IN.vTexCoord + float4(  fTexelSize, 0.0f, 0.0f, 0.0f );
	vTexCoords[3] = IN.vTexCoord + float4( 0.0f, -fTexelSize, 0.0f, 0.0f );
	vTexCoords[6] = IN.vTexCoord + float4( 0.0f,  fTexelSize, 0.0f, 0.0f );

	vTexCoords[4] = IN.vTexCoord + float4( -fTexelSize, -fTexelSize, 0.0f, 0.0f );
	vTexCoords[5] = IN.vTexCoord + float4(  fTexelSize, -fTexelSize, 0.0f, 0.0f );
	vTexCoords[7] = IN.vTexCoord + float4( -fTexelSize,  fTexelSize, 0.0f, 0.0f );
	vTexCoords[8] = IN.vTexCoord + float4(  fTexelSize,  fTexelSize, 0.0f, 0.0f );

	// Sample each of them checking whether the pixel under test is shadowed or not
	float fShadowTerm = 0.0f;

	for( int i = 0; i < 9; i++ )
	{
		float A = tex2Dproj( ShadowSampler, vTexCoords[i] ).r;
		float B = (IN.vTexCoord.z - 0.001f);
		
		// Texel is shadowed
		fShadowTerm += A < B ? 0.1f : 1.0f;
	}

	// Get the average
	fShadowTerm /= 9.0f;
	
	// Uncomment this to disable 3x3 PCF
	// float fShadowTerm = tex2Dproj( ShadowSampler, IN.vTexCoord ) < (IN.vTexCoord.z - 0.001f) ? 0.1f : 1.0f;
	
	return fShadowTerm;
}

// Horizontal blur pixel shader
float4 PS_BlurH( VSOUTPUT_BLUR IN ) : COLOR0
{
	// Accumulated color
	float4 vAccum = float4( 0.0f, 0.0f, 0.0f, 0.0f );

	// Sample the taps (g_vSampleOffsets holds the texel offsets and g_fSampleWeights holds the texel weights)
	for( int i = 0; i < 15; i++ )
	{
		vAccum += tex2D( ScreenSampler, IN.vTexCoord + g_vSampleOffsets[i] ) * g_fSampleWeights[i];
	}

	return vAccum;
}

// Vertical blur pixel shader
float4 PS_BlurV( VSOUTPUT_BLUR IN ) : COLOR0
{
	// Accumulated color
	float4 vAccum = float4( 0.0f, 0.0f, 0.0f, 0.0f );

	// Sample the taps (g_vSampleOffsets holds the texel offsets and g_fSampleWeights holds the texel weights)
	for( int i = 0; i < 15; i++ )
	{
		vAccum += tex2D( BlurHSampler, IN.vTexCoord + g_vSampleOffsets[i] ) * g_fSampleWeights[i];
	}

	return vAccum;
}

float4 PS_Scene( VSOUTPUT_SCENE IN ) : COLOR0
{
	// Normalize the normal, light and eye vectors
	IN.vNormal	 = normalize( IN.vNormal );
	IN.vLightVec = normalize( IN.vLightVec );
	IN.vEyeVec   = normalize( IN.vEyeVec );

	// Sample the color and normal maps
	float4 vColor  = tex2D( ColorSampler, IN.vTexCoord );

	// Compute the diffuse and specular lighting terms
	float diffuse  = max( dot( IN.vNormal, IN.vLightVec ), 0 );
	float specular = pow( max( dot( 2 * dot( IN.vNormal, IN.vLightVec ) * IN.vNormal - IN.vLightVec, IN.vEyeVec ), 0 ), 8 );

	if( diffuse == 0 ) specular = 0;

	// Grab the shadow term
	float fShadowTerm = tex2Dproj( BlurVSampler, IN.vScreenCoord );

	// Grab the spot term
	float fSpotTerm = tex2Dproj( SpotSampler, IN.vProjCoord );

	// Compute the final color
	return (diffuse * vColor * g_vLightColor * fShadowTerm * fSpotTerm) +
		   (specular * vColor * g_vLightColor.a * fShadowTerm * fSpotTerm);
}

float4 PS_SceneHard( VSOUTPUT_SCENE IN ) : COLOR0
{
	// Normalize the normal, light and eye vectors
	IN.vNormal	 = normalize( IN.vNormal );
	IN.vLightVec = normalize( IN.vLightVec );
	IN.vEyeVec   = normalize( IN.vEyeVec );

	// Sample the color and normal maps
	float4 vColor  = tex2D( ColorSampler, IN.vTexCoord );

	// Compute the ambient, diffuse and specular lighting terms
	float diffuse  = max( dot( IN.vNormal, IN.vLightVec ), 0 );
	float specular = pow( max( dot( 2 * dot( IN.vNormal, IN.vLightVec ) * IN.vNormal - IN.vLightVec, IN.vEyeVec ), 0 ), 8 );

	if( diffuse == 0 ) specular = 0;

	// Grab the spot term
	float fSpotTerm = tex2Dproj( SpotSampler, IN.vProjCoord );

	// Grab the shadow term
	float fShadowTerm = 0.0f;
	fShadowTerm = tex2Dproj( ShadowSampler, IN.vProjCoord ) < (IN.vProjCoord.z - 0.001f) ? 0.1f : 1.0f;
	fShadowTerm *= fSpotTerm;
		
/*	// Generate the 9 texture co-ordinates for a 3x3 PCF kernel
	float4 vTexCoords[9];

	// Texel size
	float fTexelSize = 1.0f / 512.0f;

	// Generate the tecture co-ordinates for the specified depth-map size
	// 4 3 5
	// 1 0 2
	// 7 6 8
	vTexCoords[0] = IN.vProjCoord;

	vTexCoords[1] = IN.vProjCoord + float4( -fTexelSize, 0.0f, 0.0f, 0.0f );
	vTexCoords[2] = IN.vProjCoord + float4(  fTexelSize, 0.0f, 0.0f, 0.0f );
	vTexCoords[3] = IN.vProjCoord + float4( 0.0f, -fTexelSize, 0.0f, 0.0f );
	vTexCoords[6] = IN.vProjCoord + float4( 0.0f,  fTexelSize, 0.0f, 0.0f );

	vTexCoords[4] = IN.vProjCoord + float4( -fTexelSize, -fTexelSize, 0.0f, 0.0f );
	vTexCoords[5] = IN.vProjCoord + float4(  fTexelSize, -fTexelSize, 0.0f, 0.0f );
	vTexCoords[7] = IN.vProjCoord + float4( -fTexelSize,  fTexelSize, 0.0f, 0.0f );
	vTexCoords[8] = IN.vProjCoord + float4(  fTexelSize,  fTexelSize, 0.0f, 0.0f );

	// Sample each of them checking whether the pixel under test is shadowed or not
	for( int i = 0; i < 9; i++ )
	{
		float A = tex2Dproj( ShadowSampler, vTexCoords[i] ).r;
		float B = IN.vProjCoord.z - 0.001f;
		
		// Texel is shadowed
		fShadowTerm += A < B ? 0.1f : 1.0f;
	}
	
	// Get the average
	fShadowTerm = fShadowTerm * fSpotTerm / 9.0f;
*/	
	// Compute the final color
	return (diffuse * vColor * g_vLightColor * fShadowTerm) +
		   (specular * vColor * g_vLightColor.a * fShadowTerm);
}

//--------------------------------------------
// Techniques
//--------------------------------------------
technique techShadow
{
	pass p0
	{
		Lighting	= False;
		CullMode	= CCW;
		
		VertexShader = compile vs_2_0 VS_Shadow();
		PixelShader  = compile ps_2_0 PS_Shadow();
	}
}

technique techUnlit
{
	pass p0
	{
		Lighting	= False;
		CullMode	= CCW;
		
		VertexShader = compile vs_2_0 VS_Unlit();
		PixelShader  = compile ps_2_0 PS_Unlit();
	}
}

technique techBlurH
{
	pass p0
	{
		Lighting	= False;
		CullMode	= None;
		
		VertexShader = compile vs_2_0 VS_Blur();
		PixelShader  = compile ps_2_0 PS_BlurH();
	}
}

technique techBlurV
{
	pass p0
	{
		Lighting	= False;
		CullMode	= None;
		
		VertexShader = compile vs_2_0 VS_Blur();
		PixelShader  = compile ps_2_0 PS_BlurV();
	}
}

technique techScene
{
	pass p0
	{
		Lighting	= False;
		CullMode	= CCW;
		
		VertexShader = compile vs_2_0 VS_Scene();
		PixelShader  = compile ps_2_0 PS_Scene();
	}
}

technique techSceneHard
{
	pass p0
	{
		Lighting	= False;
		CullMode	= CCW;
		
		VertexShader = compile vs_2_0 VS_Scene();
		PixelShader  = compile ps_2_0 PS_SceneHard();
	}
}