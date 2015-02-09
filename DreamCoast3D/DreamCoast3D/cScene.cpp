#include "stdafx.h"
#include "cScene.h"
#include "cCamera.h"
#include "cLightSource.h"
#include "cGameObjManager.h"
#include "cSphere.h"

cScene::cScene() 
	:m_pCamera(NULL)
	, m_pGameObjManager(NULL)
{
}

cScene::~cScene()
{
	SAFE_RELEASE(m_pCamera);	
	m_pGameObjManager->Destroy();
}

void cScene::Setup(std::string sFilePath){
	m_pCamera = new cCamera;
	m_pCamera->Setup();
	m_pCamera->SetDesc(std::string("Camera For Scene Example1"));

	m_pGameObjManager = new cGameObjManager;
	m_pGameObjManager->Setup();
	m_pGameObjManager->SetDesc("GameObject Manager for Example1");

	g_pD3DDevice->SetRenderState(D3DRS_NORMALIZENORMALS, true);
	g_pD3DDevice->SetRenderState(D3DRS_LIGHTING, true);

}

void cScene::Update(float delta){
	m_pCamera->Update(delta);
	if (m_pGameObjManager){
		m_pGameObjManager->Update(delta);
	}
}

void cScene::Render(){
	SAFE_RENDER(m_pGameObjManager);
}

void cScene::AddGameObj(cGameObject* pGameObj){
	m_pGameObjManager->AddGameObj(pGameObj);
}

void cScene::Destroy(){
	for (auto p : m_vecLightSources){
		SAFE_RELEASE(p);
	}
	this->Release();
}