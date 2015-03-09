#include "stdafx.h"
#include "cGameCore.h"
#include "cSceneManager.h"
#include "cGameLoader.h"

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
	g_pShaderManager->Destroy();

}

void cGameCore::Setup(){
	m_pSceneManager = new cSceneManager;
	cGameLoader cGL;
	cGL.LoadFromGameFile(m_pSceneManager, std::string("../Resources/MAINDATA.txt"));
	m_pSceneManager->Setup(std::string(""), std::string(""));
	m_pSceneManager->Start();
}

void cGameCore::Update(float fDelta){
	if (m_pSceneManager){
		m_pSceneManager->Update(fDelta);
	}
}

void cGameCore::Render(){
	D3DCOLOR bgColour = 0xFF555555;	// 배경색상 - 파랑
	g_pD3DDevice->Clear(
		NULL, NULL,
		D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
		bgColour,
		1.0f, 0);
	g_pD3DDevice->BeginScene();

	SAFE_RENDER(m_pSceneManager);

	g_pD3DDevice->EndScene();
	g_pD3DDevice->Present(NULL, NULL, NULL, NULL);
}