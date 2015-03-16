#pragma once

#define g_pShaderManager cShaderManager::GetInstance()

class cShaderManager
{
	SINGLETONE(cShaderManager)
private:
	std::map<std::string, LPD3DXEFFECT>			m_mapShader;

	LPD3DXEFFECT m_pShadowCreateShader;
	LPD3DXEFFECT m_pShadowApplyShader;

	// �׸��ڸ� Ÿ��
	LPDIRECT3DTEXTURE9	m_pShadowRenderTarget = NULL;
	// �׸��ڸ� ����
	LPDIRECT3DSURFACE9	m_pShadowDepthStencil = NULL;
	const int m_nShadowMapSize = 2500;

public:
 	LPD3DXEFFECT GetShader(std::string sPath);
	LPD3DXEFFECT GetShader(char* szPath);

	void CreateShadowTargetAndDepthSurface();
	void ClearShadowTargetAndDepthSurface();

	LPD3DXEFFECT GetShadowCreateShader();
	LPD3DXEFFECT GetShadowApplyShader();
	LPDIRECT3DTEXTURE9 GetShadowRenderTarget() { return m_pShadowRenderTarget; }
	LPDIRECT3DSURFACE9 GetShadowDepthStencil() { return m_pShadowDepthStencil; }

	void Destroy();
};