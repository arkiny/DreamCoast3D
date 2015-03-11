#include "stdafx.h"
#include "cScene.h"
#include "cCamera.h"
#include "cLightSource.h"
#include "cGameObjManager.h"
#include "cGameMapObject.h"
#include "cSphere.h"
#include "cUIObjManager.h"
#include "cCameraEditing.h"
#include "cTransform.h"
#include "cGameObjLoader.h"
#include "cMapLoader.h"
#include "cUILoader.h"
#include "cEffectManager.h"
#include "cGameEventManager.h"

/// TODO
// 차후 데이타 드리븐 완료후 삭제해야할것들
#include "cUIStatWindow.h"
#include "cUISkillShortCut.h"
#include "cUIExpBar.h"
#include "cUIMinimap.h"
#include "cGameMapSkyObject.h"
#include "cGamePlayableObject.h"
#include "cGameAIObject.h"
#include "cUISystemTest.h"
#include "cEffectFireBall.h"


cScene::cScene() 
	:m_pCamera(NULL)
	, m_pGameObjManager(NULL)
	, m_pCurrentMap(NULL)
	, m_pUIObjManager(NULL)
	, m_pPlayableObject(NULL)
	, m_pDelegate(NULL)
	, m_pEffectManager(NULL)
	, m_pGameEventManager(NULL)
{
}

cScene::~cScene()
{
	SAFE_RELEASE(m_pCamera);	
	
	if (m_pGameObjManager){
		m_pGameObjManager->Destroy();
	}
	if (m_pUIObjManager){
		m_pUIObjManager->Destroy();
	}
	if (m_pEffectManager){
		m_pEffectManager->Destroy();
	}
	SAFE_RELEASE(m_pCurrentMap);
	for (auto p : m_vecGameMaps){
		SAFE_RELEASE(p);
	}	
	m_vecGameMaps.clear();
	for (auto p : m_vecLightSources){
		SAFE_RELEASE(p);
	}
	m_vecLightSources.clear();

	// MS
	if (m_pGameEventManager)
	{
		m_pGameEventManager->Destory();
	}

	//SAFE_RELEASE(gpShadowRenderTarget );
	//SAFE_RELEASE(gpShadowDepthStencil );
	//SAFE_RELEASE(gpApplyShadowShader  );
	//SAFE_RELEASE(gpCreateShadowShader )

}

void cScene::Setup(std::string sFilePath){

	m_pGameObjManager = new cGameObjManager;
	m_pGameObjManager->Setup();
	m_pGameObjManager->SetDesc("GameObject Manager for Example1");

	m_pUIObjManager = new cUIObjManager;
	m_pUIObjManager->Setup();
	m_pUIObjManager->SetDesc("UIObject Manager for Example1");

	m_pEffectManager = new cEffectManager;
	m_pEffectManager->Setup();
	m_pEffectManager->SetDesc("EffectManager");


	m_pGameEventManager = new cGameEventManager;
	m_pGameEventManager->Setup();
	m_pGameEventManager->SetDesc("EventManager");
}

void cScene::Start(){
	/// 리소스 로딩
	cGameObjLoader cGOL;
	cGOL.LoadGameObjectsFromFile(this->GetGameObjMng(), m_sGameObjPath);

	cMapLoader cML;
	cML.LoadGameMapFromFile(this, m_sGameMapPath);

	cUILoader cUL;
	cUL.LoadGameUIFromFile(this->GetUIObjMng(), m_sGameUIPath);
	/// end 리소스 로딩

	g_pD3DDevice->SetRenderState(D3DRS_NORMALIZENORMALS, true);
	g_pD3DDevice->SetRenderState(D3DRS_LIGHTING, true);
	
	// 해당 씬의 라이트 적용
	for (auto p : m_vecLightSources){
		p->Start();
	}



	// 첫 맵 설정
	// TODO : 차후 맵간 이동이 있을 것이므로 해당 맵간 로딩도 멀티쓰레드 로딩을 생각해볼 것


	if (!m_vecGameMaps.empty()){
		this->SetCurrentMap(0);
		// 카메라에 타겟 설정
		if (m_pGameObjManager->GetPlayerableGameObject()){
			m_pCamera->SetTarget(m_pGameObjManager->GetPlayerableGameObject()->GetTransform()->getPosPointer());
		}
		// 게임 오브젝트매니저에 카메라 설정
		m_pGameObjManager->SetCameraDeligate(m_pCamera);

		// 타일맵에 현재 맵 추가
		m_pGameObjManager->SetEventDeligate(m_pGameEventManager);
		m_pGameObjManager->SetCurrentTileSystem(m_pCurrentMap);
		m_pGameObjManager->SetTileSystemForEvent();
	}
	if (m_pUIObjManager){
		// UI에 게임오브젝트 델리게이트 어태치
		if (m_pGameObjManager){
			m_pUIObjManager->SetGameObjDeligate(m_pGameObjManager);
		}
		if (m_pDelegate){
			// UI에 씬매니저 델리게이트 어태치
			m_pUIObjManager->SetSceneDeligate(m_pDelegate);
		}
		if (m_pGameEventManager)
		{
			m_pUIObjManager->SetEventDeligate(m_pGameEventManager);
		}
		// UI매니저 시작
		m_pUIObjManager->Start();
	}
		
	if (m_pEffectManager){
		if (m_pGameObjManager){
			m_pGameObjManager->SetEffectDeligate(m_pEffectManager);
		}
		m_pEffectManager->Start();
	}


}

void cScene::Update(float delta){
	if (m_pCurrentMap){
		m_pCamera->Update(delta);
		m_pPlayableObject = (cGamePlayableObject*)m_pGameObjManager->GetPlayerableGameObject();
		m_pCamera->UpdateAngle(m_pPlayableObject->GetPlayerAngle());
	
		if (m_pCurrentMap){
			m_pCurrentMap->Update(delta);
		}

		if (m_pCurrentMap && m_pGameObjManager){
			m_pGameObjManager->AdjustYPositionByHeightMap(m_pCurrentMap);
		}

		if (m_pGameObjManager){
			m_pGameObjManager->Update(delta);
		}


	}

	if (m_pEffectManager){
		m_pEffectManager->Update(delta);
	}

	if (m_pUIObjManager){
		m_pUIObjManager->Update(delta);
	}

	if (m_pGameEventManager)
	{
		m_pGameEventManager->Update(delta);
	}


}

void cScene::Render(){
	// 씬을 렌더한다.

	// 현재 맵만 렌더링함
	SAFE_RENDER(m_pGameObjManager);
	SAFE_RENDER(m_pCurrentMap);

	SAFE_RENDER(m_pUIObjManager);
	SAFE_RENDER(m_pCamera);
	SAFE_RENDER(m_pEffectManager);
}

void cScene::Exit(){
	// 리소스 해제
	
}

void cScene::SetDelegate(iSceneDelegate* pSceneDeligate){
	m_pDelegate = pSceneDeligate;
	if (m_pUIObjManager){
		m_pUIObjManager->SetSceneDeligate(pSceneDeligate);
	}
}

void cScene::AddGameObj(cGameObject* pGameObj){
	m_pGameObjManager->AddGameObj(pGameObj);
}

void cScene::AddUIObj(cUIObject* pUIObj){
	m_pUIObjManager->AddUI(pUIObj);
}

void cScene::AddStaticGameObj(cGameObject* pGameObj){
	m_pGameObjManager->AddStaticGameObjects(pGameObj);
}

void cScene::Destroy(){
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

		m_pCamera->SetMap(m_pCurrentMap);
		
		SAFE_ADD_REF(m_pCurrentMap);
	}
}

void cScene::SetCamera(cCamera* pCamera) {
	SAFE_ADD_REF(pCamera);
	m_pCamera = pCamera;
}

void cScene::AddLightSrc(cLightSource* pLightSource){
	SAFE_ADD_REF(pLightSource);
	m_vecLightSources.push_back(pLightSource);
}