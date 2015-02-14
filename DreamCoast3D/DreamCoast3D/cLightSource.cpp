#include "stdafx.h"
#include "cLightSource.h"


cLightSource::cLightSource()
	: m_nIndex(-1)
{
	ZeroMemory(&m_stLight, sizeof(D3DLIGHT9));
	ZeroMemory(&m_vDir, sizeof(D3DXVECTOR3));
}


cLightSource::~cLightSource()
{
}

void cLightSource::Setup(D3DLIGHT9& stLight, D3DXVECTOR3& vDirection, int nIndex){
	m_stLight = stLight;
	m_vDir = vDirection;
	m_nIndex = nIndex;

	// 광원 인덱스는 8보다 작아야 한다.
	_ASSERT(m_nIndex < 8 && m_nIndex >= 0);
}

void cLightSource::Start(){
	g_pD3DDevice->SetLight(m_nIndex, &m_stLight);
	g_pD3DDevice->LightEnable(m_nIndex, true);
}