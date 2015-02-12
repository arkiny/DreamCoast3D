#include "stdafx.h"
#include "cGameCore.h"
#include "cSceneManager.h"


cGameCore::cGameCore()
	: m_pSceneManager(NULL)
{
}


cGameCore::~cGameCore()
{
	m_pSceneManager->Destroy();
	g_pFontManager->Destroy();
	g_pTextureManager->Destroy();
	g_pSkinnedMeshManager->Destroy();
	g_pAseManager->Destroy();

	cDeviceManager* pDevice = cDeviceManager::GetInstance();
	pDevice->Destroy();
}

void cGameCore::Setup(){
	m_pSceneManager = new cSceneManager;
	m_pSceneManager->Setup(std::string(""));
}

void cGameCore::Update(float fDelta){
	if (m_pSceneManager){
		m_pSceneManager->Update(fDelta);
	}
}

void cGameCore::Render(){
	g_pD3DDevice->Clear(
		NULL, NULL,
		D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
		D3DCOLOR_XRGB(0, 0, 0),
		1.0f, 0);
	g_pD3DDevice->BeginScene();

	SAFE_RENDER(m_pSceneManager);

	g_pD3DDevice->EndScene();
	g_pD3DDevice->Present(NULL, NULL, NULL, NULL);
}