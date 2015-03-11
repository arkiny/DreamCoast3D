#include "stdafx.h"
#include "cShaderManager.h"


cShaderManager::cShaderManager()
{
}


cShaderManager::~cShaderManager()
{
}

LPD3DXEFFECT cShaderManager::GetShader(std::string sPath){
	if (m_mapShader.find(sPath) == m_mapShader.end())
	{
		if (m_mapShader[sPath] != NULL)
			SAFE_RELEASE(m_mapShader[sPath]);

		LPD3DXEFFECT pEffect = NULL;

		D3DXMACRO mac[2] =
		{
			{ "MATRIX_PALETTE_SIZE_DEFAULT", "35" },
			{ NULL, NULL }
		};

		D3DCAPS9 caps;
		D3DXMACRO *pmac = NULL;
		g_pD3DDevice->GetDeviceCaps(&caps);
		if (caps.VertexShaderVersion > D3DVS_VERSION(1, 1))
			pmac = mac;

		DWORD dwShaderFlags = 0;

#if defined( DEBUG ) || defined( _DEBUG )
		// Set the D3DXSHADER_DEBUG flag to embed debug information in the shaders.
		// Setting this flag improves the shader debugging experience, but still allows 
		// the shaders to be optimized and to run exactly the way they will run in 
		// the release configuration of this program.
		dwShaderFlags |= D3DXSHADER_DEBUG;
#endif

#ifdef DEBUG_VS
		dwShaderFlags |= D3DXSHADER_FORCE_VS_SOFTWARE_NOOPT;
#endif
#ifdef DEBUG_PS
		dwShaderFlags |= D3DXSHADER_FORCE_PS_SOFTWARE_NOOPT;
#endif

		ID3DXBuffer* pBuffer = NULL;
		if (FAILED(D3DXCreateEffectFromFile(g_pD3DDevice,
			sPath.c_str(),
			pmac,
			NULL,
			dwShaderFlags,
			NULL,
			&pEffect,
			&pBuffer)))
		{
			// if creation fails, and debug information has been returned, output debug info
			if (pBuffer)
			{
				OutputDebugStringA((char*)pBuffer->GetBufferPointer());
				SAFE_RELEASE(pBuffer);
			}

			return NULL;
		}
		m_mapShader[sPath] = pEffect;
	}
	return m_mapShader[sPath];
}

LPD3DXEFFECT cShaderManager::GetShader(char* szPath){
	return GetShader(std::string(szPath));
}

void cShaderManager::Destroy(){

	// 셰이더 메모리 처리는 나중에....
	// 먼가 알수 없는 이슈가 있는거 같다.
	for (auto p : m_mapShader){
		if (p.second){
			p.second->EndPass();
			p.second->End();
			SAFE_RELEASE(p.second);
			//m_mapShader.erase(p.first);
		}
	}

	//for each (auto p in m_mapShader)
	//{
	//	SAFE_RELEASE(p.second);
	//}
	m_mapShader.clear();

	SAFE_RELEASE(m_pShadowCreateShader);
	SAFE_RELEASE(m_pShadowApplyShader);
	SAFE_RELEASE(m_pShadowRenderTarget);
	SAFE_RELEASE(m_pShadowDepthStencil);
}

void cShaderManager::CreateShadowTargetAndDepthSurface(){
	
	if (m_pShadowRenderTarget){
		//assert(false && "shadow render target should be empty");
		SAFE_RELEASE(m_pShadowRenderTarget);
	}
	if (m_pShadowDepthStencil){
		SAFE_RELEASE(m_pShadowDepthStencil);
		//assert(false && "shadow depth stencil should be empty");
	}

	HRESULT hr =
		g_pD3DDevice->CreateTexture(m_nShadowMapSize, m_nShadowMapSize,
		1, D3DUSAGE_RENDERTARGET, D3DFMT_R32F,
		D3DPOOL_DEFAULT, &m_pShadowRenderTarget, NULL);
	assert(hr == S_OK && "Shadow RenderTarget create Fail");

	hr = g_pD3DDevice->CreateDepthStencilSurface(m_nShadowMapSize, m_nShadowMapSize,
		D3DFMT_D24X8, D3DMULTISAMPLE_NONE, 0, TRUE, &m_pShadowDepthStencil, NULL);
	assert(hr == S_OK && "Shadow Surface create Fail");

}

void cShaderManager::ClearShadowTargetAndDepthSurface(){
	
	g_pD3DDevice->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER), 0xFFFFFFFF, 1.0f, 0);
}