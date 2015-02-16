#include "stdafx.h"
#include "cScene.h"
#include "cCamera.h"
#include "cLightSource.h"
#include "cGameObjManager.h"
#include "cGameMapObject.h"
#include "cSphere.h"
#include "cUIObjManager.h"
#include "cCameraEditing.h"

//
#include "cUIStatWindow.h"
#include "cUISkillShortCut.h"
#include "cUIExpBar.h"

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

	cUIStatWindow* p = new cUIStatWindow;
	p->Setup();
	m_pUIObjManager->AddUI(p);
	SAFE_RELEASE(p);

	cUISkillShortCut* p2 = new cUISkillShortCut;
	p2->Setup();
	m_pUIObjManager->AddUI(p2);
	SAFE_RELEASE(p2);

	cUIExpBar* p3 = new cUIExpBar;
	p3->Setup();
	m_pUIObjManager->AddUI(p3);
	SAFE_RELEASE(p3);

}

void cScene::Start(){
	g_pD3DDevice->SetRenderState(D3DRS_NORMALIZENORMALS, true);
	g_pD3DDevice->SetRenderState(D3DRS_LIGHTING, true);
	
	// 해당 씬의 라이트 적용
	for (auto p : m_vecLightSources){
		p->Start();
	}
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
	// 현재 맵만 렌더링함
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