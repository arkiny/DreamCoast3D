#include "stdafx.h"
#include "cScene.h"
#include "cCamera.h"
#include "cLightSource.h"
#include "cGameObjManager.h"
#include "cGameMapObject.h"
#include "cSphere.h"
#include "cUIObjManager.h"
#include "cCameraEditing.h"

cScene::cScene() 
	:m_pCamera(NULL)
	, m_pGameObjManager(NULL)
	, m_pCurrentMap(NULL)
	, m_pUIObjManager(NULL)
{
}

cScene::~cScene()
{
	SAFE_RELEASE(m_pCamera);	
	
	m_pGameObjManager->Destroy();
	m_pUIObjManager->Destroy();

	SAFE_RELEASE(m_pCurrentMap);
	for (auto p : m_vecGameMaps){
		SAFE_RELEASE(p);
	}	
}

void cScene::Setup(std::string sFilePath){
	m_pCamera = new cCameraEditing;
	m_pCamera->Setup();
	m_pCamera->SetDesc(std::string("Camera For Scene Example1"));

	m_pGameObjManager = new cGameObjManager;
	m_pGameObjManager->Setup();
	m_pGameObjManager->SetDesc("GameObject Manager for Example1");

	m_pUIObjManager = new cUIObjManager;
	m_pUIObjManager->Setup();
	m_pUIObjManager->SetDesc("UIObject Manager for Example1");

	g_pD3DDevice->SetRenderState(D3DRS_NORMALIZENORMALS, true);
	g_pD3DDevice->SetRenderState(D3DRS_LIGHTING, true);
}

void cScene::Update(float delta){
	m_pCamera->Update(delta);
	
	if (m_pUIObjManager){
		m_pUIObjManager->Update(delta);
	}

	if (m_pCurrentMap){
		m_pCurrentMap->Update(delta);
	}
	
	if (m_pGameObjManager){
		m_pGameObjManager->Update(delta);
	}

	if (m_pCurrentMap && m_pGameObjManager){
		m_pGameObjManager->AdjustYPositionByHeightMap(m_pCurrentMap);
	}
}

void cScene::Render(){
	// ÇöÀç ¸Ê¸¸ ·»´õ¸µÇÔ
	SAFE_RENDER(m_pCurrentMap);

	SAFE_RENDER(m_pGameObjManager);
	SAFE_RENDER(m_pUIObjManager);

	SAFE_RENDER(m_pCamera);
}

void cScene::AddGameObj(cGameObject* pGameObj){
	m_pGameObjManager->AddGameObj(pGameObj);
}

void cScene::AddUIObj(cUIObject* pUIObj){
	m_pUIObjManager->AddUI(pUIObj);
}

void cScene::Destroy(){
	for (auto p : m_vecLightSources){
		SAFE_RELEASE(p);
	}
	this->Release();
}

void cScene::AddMap(cGameMapObject* pGameMap){
	if (pGameMap){
		if ( pGameMap->GetIndex() >= 0 &&  pGameMap->GetIndex() < (int)m_vecGameMaps.size()){
			if (m_vecGameMaps[pGameMap->GetIndex()] != pGameMap){
				SAFE_ADD_REF(pGameMap);
				cGameMapObject* p = m_vecGameMaps[pGameMap->GetIndex()];
				SAFE_RELEASE(p);
				m_vecGameMaps[pGameMap->GetIndex()] = pGameMap;
			}
			else{
				_ASSERT(false && "Index Should smaller than vector size");
			}
		}
		else{
			m_vecGameMaps.push_back(pGameMap);
			pGameMap->SetIndex(m_vecGameMaps.size() - 1);
		}		
	}
}

void cScene::SetCurrentMap(int nIndex){
	_ASSERT(nIndex >= 0 && nIndex < (int)m_vecGameMaps.size() 
		&& "Map Index Should be larger than 0, smaller than MapVector");

	if (m_pCurrentMap != m_vecGameMaps[nIndex]){
		SAFE_RELEASE(m_pCurrentMap);
		m_pCurrentMap = m_vecGameMaps[nIndex];
		SAFE_ADD_REF(m_pCurrentMap);
	}
}