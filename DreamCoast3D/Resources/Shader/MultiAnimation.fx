#include "MultiAnimation.vsh"



float4x4 gWorldMatrix : World;
float4x4 gWorldViewProjectionMatrix : WorldViewProjection;
float4 gWorldLightPosition
<
string UIName = "gWorldLightPosition";
string UIWidget = "Direction";
bool UIVisible = false;
float4 UIMin = float4(-10.00, -10.00, -10.00, -10.00);
float4 UIMax = float4(10.00, 10.00, 10.00, 10.00);
bool Normalize = false;
> = float4(500.00, 500.00, -500.00, 1.00);
float4 gWorldCameraPosition : ViewPosition;
float4x4 gLightViewMatrix
<
string UIName = "gLightViewMatrix";
string UIWidget = "Numeric";
bool UIVisible = false;
> = float4x4(1.00, 0.00, 0.00, 0.00, 0.00, 1.00, 0.00, 0.00, 0.00, 0.00, 1.00, 0.00, 0.00, 0.00, 0.00, 1.00);
float4x4 gLightProjectionMatrix : Projection;
float4x4 gViewProjectionMatrix : ViewProjection;

//--------------------------------------------------------------------------------------
struct VS_INPUT
{
	float4  Pos         : POSITION;
	float3  BlendWeights: BLENDWEIGHT;
	float4  BlendIndices: BLENDINDICES;
	float3  Normal      : NORMAL;
	float3	mTangent: TANGENT;
	float3	mBinormal: BINORMAL;
	float3  TexCoord    : TEXCOORD0;
};

struct VS_OUTPUT
{
	float4 Pos			: POSITION;
	float2 mUV: TEXCOORD1;
	float3 mLightDir : TEXCOORD2;
	float3 mViewDir: TEXCOORD3;
	float3 T : TEXCOORD4;
	float3 B : TEXCOORD5;
	float3 N : TEXCOORD6;
	float4 mClipPosition: TEXCOORD7;
};

VS_OUTPUT VertSkinning( VS_INPUT Input, uniform int nNumBones )
{
	VS_OUTPUT   Output;
	
	float4 oriPosition = Input.Pos;

	// skin VB inputs
	VS_SKIN_INPUT vsi = { Input.Pos, Input.BlendWeights, Input.BlendIndices, Input.Normal };
	VS_SKIN_OUTPUT vso = VS_Skin( vsi, nNumBones );

	Output.Pos = mul(float4(vso.vPos.xyz, 1.0f), gWorldViewProjectionMatrix);
	Output.mUV = Input.TexCoord;

	float4 worldPosition = mul(float4(vso.vPos.xyz, 1.0f), gWorldMatrix);
	float3 lightDir = worldPosition.xyz - gWorldLightPosition.xyz;
	Output.mLightDir = normalize(lightDir);

	float3 viewDir = normalize(worldPosition.xyz - gWorldCameraPosition.xyz);
	Output.mViewDir = viewDir;

	float3 worldNormal = mul(vso.vNor, (float3x3)gWorldMatrix);
	Output.N = normalize(worldNormal);

	float3 worldTangent = mul(Input.mTangent, (float3x3)gWorldMatrix);
	Output.T = normalize(worldTangent);

	float3 worldBinormal = mul(Input.mBinormal, (float3x3)gWorldMatrix);
	Output.B = normalize(worldBinormal);

	float4x4 lightViewMatrix = gLightViewMatrix;
	worldPosition = mul(oriPosition, gWorldMatrix);
	Output.mClipPosition = mul(worldPosition, lightViewMatrix);
	Output.mClipPosition = mul(Output.mClipPosition, gLightProjectionMatrix);

	return Output;
}



int CurNumBones = 10;

VertexShader vsArray20[ 4 ] = { 
	compile vs_2_0 VertSkinning( 1 ),
	compile vs_2_0 VertSkinning( 2 ),
	compile vs_2_0 VertSkinning( 3 ),
	compile vs_2_0 VertSkinning( 4 ) };


struct PS_INPUT
{
	float2 mUV : TEXCOORD1;
	float3 mLightDir : TEXCOORD2;
	float3 mViewDir: TEXCOORD3;
	float3 T : TEXCOORD4;
	float3 B : TEXCOORD5;
	float3 N : TEXCOORD6;
	float4 mClipPosition: TEXCOORD7;
};

texture DiffuseMap_Tex
<
string ResourceName = "..\\Documents\\GitHub\\DreamCoast3D\\DreamCoast3D\\Resources\\Char\\Tera\\Popori_F_L03_Body_diff.tga";
>;
sampler2D DiffuseSampler = sampler_state
{
	Texture = (DiffuseMap_Tex);
	MAGFILTER = LINEAR;
	MINFILTER = LINEAR;
};
texture SpecularMap_Tex
<
string ResourceName = "..\\Documents\\GitHub\\DreamCoast3D\\DreamCoast3D\\Resources\\Char\\Tera\\Popori_F_L03_Body_spec.tga";
>;
sampler2D SpecularSampler = sampler_state
{
	Texture = (SpecularMap_Tex);
	MAGFILTER = LINEAR;
	MINFILTER = LINEAR;
};
texture NormalMap_Tex
<
string ResourceName = "..\\Documents\\GitHub\\DreamCoast3D\\DreamCoast3D\\Resources\\Char\\Tera\\Popori_F_L03_Body_norm.tga";
>;
sampler2D NormalSampler = sampler_state
{
	Texture = (NormalMap_Tex);
	MAGFILTER = LINEAR;
	MINFILTER = LINEAR;
};
texture ShadowMap_Tex
<
string ResourceName = ".\\";
>;
sampler2D ShadowSampler = sampler_state
{
	Texture = (ShadowMap_Tex);
};


float3 gLightColor
<
string UIName = "gLightColor";
string UIWidget = "Numeric";
bool UIVisible = false;
float UIMin = -1.00;
float UIMax = 1.00;
> = float3(1.00, 1.00, 1.00);

float4 NormalMapping_Pass_0_Pixel_Shader_ps_main(PS_INPUT Input) : COLOR
{
	float3 tangentNormal = tex2D(NormalSampler, Input.mUV).xyz;
	tangentNormal = normalize(tangentNormal * 2 - 1);

	float3x3 TBN = float3x3(normalize(Input.T), normalize(Input.B), normalize(Input.N));
		TBN = transpose(TBN);
	float3 worldNormal = mul(TBN, tangentNormal);

		float4 albedo = tex2D(DiffuseSampler, Input.mUV);
		float3 lightDir = normalize(Input.mLightDir);
		float3 diffuse = saturate(dot(worldNormal, -lightDir));
		diffuse = gLightColor * albedo.rgb * diffuse;

	float3 specular = 0;
	if (diffuse.x > 0)
	{
		float3 reflection = reflect(lightDir, worldNormal);
			float3 viewDir = normalize(Input.mViewDir);

			specular = saturate(dot(reflection, -viewDir));
		specular = pow(specular, 20.0f);

		float4 specularIntensity = tex2D(SpecularSampler, Input.mUV);
			specular *= specularIntensity.rgb * gLightColor;
	}

	float3 ambient = float3(0.5f, 0.5f, 0.5f) * albedo;

	float3 ret = float3(ambient + diffuse + specular);

	// shadow
	float currentDepth = Input.mClipPosition.z / Input.mClipPosition.w;
	float2 uv = Input.mClipPosition.xy / Input.mClipPosition.w;
		uv.y = -uv.y;
	uv = uv * 0.5 + 0.5;

	float shadowDepth = tex2D(ShadowSampler, uv).r;

	if (currentDepth > shadowDepth + 0.0000125f)
	{
		ret *= 0.5f;
	}

	return float4(ret, 1);
}

//--------------------------------------------------------------------------------------
// Techniques
//--------------------------------------------------------------------------------------

technique Skinning20
{
	pass p0
	{
		VertexShader = ( vsArray20[ CurNumBones ] );
		PixelShader = compile ps_2_0 NormalMapping_Pass_0_Pixel_Shader_ps_main();
	}
}