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
}

void cScene::Setup(std::string sFilePath){
	//1. 각 씬은 광원이 다를 수 있으므로 상속 받은 곳에서 설정한다.
	// HACK : LightSource 매니저를 따로 뽑을 건지 고민
	//D3DLIGHT9 stLight;
	//D3DXVECTOR3 vDir = D3DXVECTOR3(1.5, -1, 1);

	//ZeroMemory(&stLight, sizeof(D3DLIGHT9));
	//stLight.Type = D3DLIGHT_DIRECTIONAL;
	//D3DXVec3Normalize(&vDir, &vDir);
	//stLight.Direction = vDir;
	//stLight.Ambient = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	//stLight.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	//stLight.Specular = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	//cLightSource* pLightSource = new cLightSource;
	//pLightSource->Setup(stLight, vDir, m_vecLightSources.size());
	//pLightSource->SetDesc("Light Source Example1");
	//m_vecLightSources.push_back(pLightSource);

	// 카메라 설정법인데, 이제는 파일에서 로딩 가능
	/*cCameraEditing* p = new cCameraEditing;
	p->Setup();
	p->SetDesc(std::string("Camera For Scene Example1"));
	p->SetPosition(D3DXVECTOR3(128.0f, 0.0f, 128.0f));
	m_pCamera = p;*/

	m_pGameObjManager = new cGameObjManager;
	m_pGameObjManager->Setup();
	m_pGameObjManager->SetDesc("GameObject Manager for Example1");

	m_pUIObjManager = new cUIObjManager;
	m_pUIObjManager->Setup();
	m_pUIObjManager->SetDesc("UIObject Manager for Example1");

	m_pEffectManager = new cEffectManager;
	m_pEffectManager->Setup();
	m_pEffectManager->SetDesc("EffectManager");

	/*cEffectFireBall* p = new cEffectFireBall;
	p->Setup();

	m_pEffectManager->AddEffect(p);
	p->Release();*/

	m_pGameEventManager = new cGameEventManager;
	m_pGameEventManager->Setup();
	m_pGameEventManager->SetDesc("EventManager");

	/// TODO 차후 UI 및 하늘 역시 DataDriven으로 처리된후 삭제
	
	//cUISystemTest* p = new cUISystemTest;
	//p->Setup();
	//m_pUIObjManager->AddUI(p);
	//SAFE_RELEASE(p);

	///
	//cUIStatWindow* p = new cUIStatWindow;
	//p->Setup();
	//m_pUIObjManager->AddUI(p);
	//SAFE_RELEASE(p);

	//cUISkillShortCut* p2 = new cUISkillShortCut;
	//p2->Setup();
	//m_pUIObjManager->AddUI(p2);
	//SAFE_RELEASE(p2);

	//cUIExpBar* p3 = new cUIExpBar;
	//p3->Setup();
	//m_pUIObjManager->AddUI(p3);
	//SAFE_RELEASE(p3);

	//cUIMinimap* p4 = new cUIMinimap;
	//p4->Setup();
	//m_pUIObjManager->AddUI(p4);
	//SAFE_RELEASE(p4);

	// Sky
	//cGameMapSkyObject* pGMSO = new cGameMapSkyObject;
	//pGMSO->Setup();
	//m_pGameObjManager->AddGameObj(pGMSO);
	//SAFE_RELEASE(pGMSO);


	//SAFE_RELEASE(pAseObject);

	//cGameAIObject* pGameAIObject = new cGameAIObject;
	//pGameAIObject->Setup(std::string("../Resources/Char/Tera/"), std::string("Monster_SnowmanRed.X"));
	//pGameAIObject->SetPosition(D3DXVECTOR3(140.0f, 0.0f, 140.0f));
	//pGameAIObject->SetScale(D3DXVECTOR3(0.03f, 0.03f, 0.03f));
	//m_pGameObjManager->AddGameObj(pGameAIObject);
	//SAFE_RELEASE(pGameAIObject);

	//pGameAIObject = new cGameAIObject;
	//pGameAIObject->Setup(std::string("../Resources/Char/Tera/"), std::string("Monster_SnowmanRed.X"));
	//pGameAIObject->SetPosition(D3DXVECTOR3(110.0f, 0.0f, 110.0f));
	//pGameAIObject->SetScale(D3DXVECTOR3(0.03f, 0.03f, 0.03f));
	//m_pGameObjManager->AddGameObj(pGameAIObject);
	////pGameAIObject->SetTargetObject(m_pGameObjManager->GetPlayerableGameObject());
	//SAFE_RELEASE(pGameAIObject);
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
		m_pGameObjManager->SetCurrentTileSystem(m_pCurrentMap);
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
		// UI매니저 시작
		m_pUIObjManager->Start();
	}
		
	if (m_pEffectManager){
		if (m_pGameObjManager){
			m_pGameObjManager->SetEffectDeligate(m_pEffectManager);
		}
		m_pEffectManager->Start();
	}

	if (m_pGameEventManager)
	{
		m_pGameEventManager->Start();
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

		if (m_pGameObjManager){
			m_pGameObjManager->Update(delta);
		}

		if (m_pCurrentMap && m_pGameObjManager){
			m_pGameObjManager->AdjustYPositionByHeightMap(m_pCurrentMap);
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
	// Create the shadow map
	//D3DVIEWPORT9 vp;
	//g_pD3DDevice->GetViewport(&vp);
	//LPDIRECT3DTEXTURE9 pShadowMap;

	//if (FAILED(g_pD3DDevice->CreateTexture(vp.Width, vp.Height, 1,
	//	D3DUSAGE_RENDERTARGET, D3DFMT_R32F,
	//	D3DPOOL_DEFAULT, &pShadowMap,
	//	NULL)))
	//{
	//	MessageBox(g_hWnd, "Unable to create shadow map!",
	//		"Error", MB_OK | MB_ICONERROR);
	//	//return E_FAIL;
	//}

	//// Grab the texture's surface
	//pShadowMap->GetSurfaceLevel(0, &g_pShadowSurf);

	//D3DXMATRIXA16 matView, matProj, matWorld, matLightViewProj;
	//D3DXVECTOR3 vLightPos(0, 0, 0);
	//D3DXVECTOR3 vLightAim = m_vecLightSources[0]->GetDirection();
	//D3DXVECTOR3 vUp(0, 1, 0);
	//// Ordinary view matrix
	//D3DXMatrixLookAtLH(&matView, &vLightPos, &vLightAim, &vUp);
	//// Projection matrix for the light
	//D3DXMatrixPerspectiveFovLH(&matProj, D3DXToRadian(30.0f), 1.0f, 1.0f, 1024.0f);
	//// Concatenate the world matrix with the above two to get the required matrix
	//matLightViewProj = matWorld * matView * matProj;

	// 현재 맵만 렌더링함
	SAFE_RENDER(m_pCurrentMap);

	SAFE_RENDER(m_pGameObjManager);
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