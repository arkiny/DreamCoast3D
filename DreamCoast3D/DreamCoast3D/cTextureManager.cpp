#include "stdafx.h"
#include "cTextureManager.h"


cTextureManager::cTextureManager()
{
}


cTextureManager::~cTextureManager()
{
}

LPDIRECT3DTEXTURE9 cTextureManager::GetTexture(std::string sPath)
{
	if (m_mapImageInfo.find(sPath) == m_mapImageInfo.end() ||
		m_mapTexture.find(sPath) == m_mapTexture.end())
	{
		if (m_mapTexture[sPath] != NULL)
			SAFE_RELEASE(m_mapTexture[sPath]);

		D3DXCreateTextureFromFileEx(
			g_pD3DDevice,
			sPath.c_str(),
			D3DX_DEFAULT_NONPOW2,
			D3DX_DEFAULT_NONPOW2,
			D3DX_DEFAULT,
			0,
			D3DFMT_UNKNOWN,
			D3DPOOL_MANAGED,
			D3DX_FILTER_NONE,
			D3DX_DEFAULT,
			0,
			&m_mapImageInfo[sPath],
			NULL,
			&m_mapTexture[sPath]);
	}
	return m_mapTexture[sPath];
}

LPDIRECT3DTEXTURE9 cTextureManager::GetTexture(char* szPath)
{
	return GetTexture(std::string(szPath));
}

LPDIRECT3DTEXTURE9 cTextureManager::GetTexture(std::string sPath, D3DXIMAGE_INFO* pImageInfo)
{
	if (m_mapImageInfo.find(sPath) == m_mapImageInfo.end() ||
		m_mapTexture.find(sPath) == m_mapTexture.end())
	{
		if (m_mapTexture[sPath] != NULL)
			SAFE_RELEASE(m_mapTexture[sPath]);

		D3DXCreateTextureFromFileEx(
			g_pD3DDevice,
			sPath.c_str(),
			D3DX_DEFAULT_NONPOW2,
			D3DX_DEFAULT_NONPOW2,
			D3DX_DEFAULT,
			0,
			D3DFMT_UNKNOWN,
			D3DPOOL_MANAGED,
			D3DX_FILTER_NONE,
			D3DX_DEFAULT,
			0,
			&m_mapImageInfo[sPath],
			NULL,
			&m_mapTexture[sPath]);
	}
	*pImageInfo = m_mapImageInfo[sPath];
	return m_mapTexture[sPath];
}

LPDIRECT3DTEXTURE9 cTextureManager::GetTexture(char* szPath, D3DXIMAGE_INFO* pImageInfo)
{
	return GetTexture(std::string(szPath), pImageInfo);
}

void cTextureManager::Destroy()
{
	for each (auto p in m_mapTexture)
	{
		SAFE_RELEASE(p.second);
	}
	m_mapTexture.clear();
	m_mapImageInfo.clear();
}
