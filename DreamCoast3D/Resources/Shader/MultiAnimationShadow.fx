//**************************************************************//
//  Effect File exported by RenderMonkey 1.6
//
//  - Although many improvements were made to RenderMonkey FX  
//    file export, there are still situations that may cause   
//    compilation problems once the file is exported, such as  
//    occasional naming conflicts for methods, since FX format 
//    does not support any notions of name spaces. You need to 
//    try to create workspaces in such a way as to minimize    
//    potential naming conflicts on export.                    
//    
//  - Note that to minimize resulting name collisions in the FX 
//    file, RenderMonkey will mangle names for passes, shaders  
//    and function names as necessary to reduce name conflicts. 
//**************************************************************//

//--------------------------------------------------------------//
// CreateShadowShader
//--------------------------------------------------------------//
//--------------------------------------------------------------//
// CreateShadow
//--------------------------------------------------------------//
#include "MultiAnimation.vsh"

string CreateShadowShader_CreateShadow_Torus : ModelData = ".\\Torus.x";

texture ShadowMap_Tex : RenderColorTarget
<
float2 RenderTargetDimensions = { 2048, 2048 };
string Format = "D3DFMT_R32F";
float  ClearDepth = 1.000000;
int    ClearColor = -1;
>;
struct VS_INPUT
{
	float4 mPosition: POSITION;
	float3  BlendWeights: BLENDWEIGHT;
	float4  BlendIndices: BLENDINDICES;
	float3  Normal      : NORMAL;
};

struct VS_OUTPUT
{
	float4 mPosition: POSITION;
	float4 mClipPosition: TEXCOORD1;
};

float4x4 gWorldMatrix : World;
float4x4 gLightViewMatrix
<
string UIName = "gLightViewMatrix";
string UIWidget = "Numeric";
bool UIVisible = false;
> = float4x4(1.00, 0.00, 0.00, 0.00, 0.00, 1.00, 0.00, 0.00, 0.00, 0.00, 1.00, 0.00, 0.00, 0.00, 0.00, 1.00);
float4x4 gLightProjectionMatrix : Projection;
float4x4 gWorldViewProjectionMatrix : WorldViewProjection;
float4 gWorldLightPosition;

VS_OUTPUT CreateShadowShader_CreateShadow_Vertex_Shader_vs_main(VS_INPUT Input, uniform int nNumBones)
{
	VS_OUTPUT   Output;

	float4 oriPosition = Input.Pos;

	// skin VB inputs
	VS_SKIN_INPUT vsi = { Input.Pos, Input.BlendWeights, Input.BlendIndices, Input.Normal };
	VS_SKIN_OUTPUT vso = VS_Skin(vsi, nNumBones);

	Output.Pos = mul(float4(vso.vPos.xyz, 1.0f), gWorldViewProjectionMatrix);
	Output.mUV = Input.TexCoord;

	float4 worldPosition = mul(float4(vso.vPos.xyz, 1.0f), gWorldMatrix);
	
	float3 worldNormal = mul(vso.vNor, (float3x3)gWorldMatrix);
	
	Output.mClipPosition = mul(Output.Pos, lightViewMatrix);
	Output.mClipPosition = mul(Output.mClipPosition, gLightProjectionMatrix);

	return Output;
}

struct PS_INPUT
{
	float4 mClipPosition: TEXCOORD1;
};

float4 CreateShadowShader_CreateShadow_Pixel_Shader_ps_main(PS_INPUT Input) : COLOR
{
	float depth = Input.mClipPosition.z / Input.mClipPosition.w;
	return float4(depth.xxx, 1);
}

float4x4 CreateShadowShader_CreateShadow_Pixel_Shader_gWorldMatrix;
float4x4 CreateShadowShader_CreateShadow_Pixel_Shader_gLightViewMatrix;
float4x4 CreateShadowShader_CreateShadow_Pixel_Shader_gLightProjectionMatrix;

float4 CreateShadowShader_CreateShadow_Pixel_Shader_gWorldLightPosition;

int CurNumBones = 10;

VertexShader vsArray20[4] = {
	compile vs_2_0 CreateShadowShader_CreateShadow_Vertex_Shader_vs_main(1),
	compile vs_2_0 CreateShadowShader_CreateShadow_Vertex_Shader_vs_main(2),
	compile vs_2_0 CreateShadowShader_CreateShadow_Vertex_Shader_vs_main(3),
	compile vs_2_0 CreateShadowShader_CreateShadow_Vertex_Shader_vs_main(4) };

//--------------------------------------------------------------//
// Technique Section for CreateShadowShader
//--------------------------------------------------------------//
technique CreateShadowShader
{
	pass CreateShadow
		<
		string Script = "RenderColorTarget0 = ShadowMap_Tex;"
		"ClearColor = (255, 255, 255, 255);"
		"ClearDepth = 1.000000;";
		>
	{
		VertexShader = (vsArray20[CurNumBones]);
		PixelShader = compile ps_2_0 CreateShadowShader_CreateShadow_Pixel_Shader_ps_main();
	}
}

