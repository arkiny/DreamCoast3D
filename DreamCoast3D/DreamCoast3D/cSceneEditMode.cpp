#include "stdafx.h"
#include "cSceneEditMode.h"
#include "cUILoader.h"
#include "cLightSource.h"
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
#include "cMapLoader4Edit.h"
#include "cGameMapHeightGridSystem.h"
#include "cMousePicking.h"
#include <sstream>

static CRITICAL_SECTION				gCriticalSectionEditMode;

cSceneEditMode::cSceneEditMode()
	:m_pMousPicking(NULL)
{
	InitializeCriticalSection(&gCriticalSectionEditMode);
	m_rectFontArea = { 0, 0, 200, 200 };
}


cSceneEditMode::~cSceneEditMode()
{
	WaitForSingleObject(LoadNextMap, INFINITE);
	WaitForSingleObject(LoadNextObj, INFINITE);
	LeaveCriticalSection(&gCriticalSectionEditMode);
	DeleteCriticalSection(&gCriticalSectionEditMode);

	for (auto p : m_mapLoadedMap){
		SAFE_RELEASE(p.second);
	}
}

void cSceneEditMode::Setup(std::string sFilePath){
	cScene::Setup(sFilePath);
	m_pMousPicking = new cMousePicking;
	m_pMousPicking->SetEffectDelegate(m_pEffectManager);
}

// Start에서 리소스를 로딩한다.
void cSceneEditMode::Start(){
	// 일단 UI는 기존 세팅이 되있는 UI를 이용한다.
	cUILoader cUL;
	cUL.LoadGameUIFromFile(this->GetUIObjMng(), m_sGameUIPath);

	cMapLoader4Edit cML;
	cML.LoadGameMapFromFile(this, m_sGameMapPath);

	g_pD3DDevice->SetRenderState(D3DRS_NORMALIZENORMALS, true);
	g_pD3DDevice->SetRenderState(D3DRS_LIGHTING, true);

	// 역시 기존 씬의 광원을 이용한다.
	for (auto p : m_vecLightSources){
		p->Start();
	}

	if (m_pEffectManager){
		if (m_pGameObjManager){
			m_pGameObjManager->SetEffectDeligate(m_pEffectManager);
		}
		m_pEffectManager->Start();
	}

	assert(m_pCurrentMap == NULL && "m_pCurrentMap Should not has pointer in EditMode");
	assert(m_pEffectManager && m_pGameObjManager && m_pUIObjManager && m_pCamera && "All Manager Should have begun");
}

void cSceneEditMode::Update(float delta){
	if (m_pCamera){
		m_pCamera->Update(delta);
	}
	
	if (m_pCurrentMap){
		if (m_pCurrentMap){
			m_pCurrentMap->Update(delta);
		}

		if (m_pGameObjManager){
			m_pGameObjManager->Update(delta);
		}

		// 매 프레임 게임 오브젝트의 Y축을 조정해준다.
		if (m_pCurrentMap && m_pGameObjManager && m_pCurrentMap){
			m_pGameObjManager->AdjustYPositionByHeightMap(m_pCurrentMap);
		}
	}

	if (m_pEffectManager){
		m_pEffectManager->Update(delta);
	}

	if (m_pUIObjManager){
		m_pUIObjManager->Update(delta);
	}
	//
	if (m_pMousPicking){
		m_pMousPicking->Update();
	}
	
	//
	if (GetAsyncKeyState(VK_NEXT)){
		if (m_pCurrentMap == NULL){
			cGameMapHeightGridSystem* ret = new cGameMapHeightGridSystem;
			ret->LoadFromFiles(m_vecMapRawPath[0], m_vecMapTexturePath[0]);
			m_mapLoadedMap[m_vecMapRawPath[0]] = ret;
			m_pCurrentMap = ret;
			m_pMousPicking->SetVertex(ret->GetVertexVectorByRef());
			m_pCurrentMap->AddRef();
		}
		else {
			if (m_nCurrentMapIndex == m_vecMapRawPath.size() - 1){
				m_nCurrentMapIndex = 0;
			}
			else {
				m_nCurrentMapIndex++;
			}
			if (m_mapLoadedMap.find(m_vecMapRawPath[m_nCurrentMapIndex]) == m_mapLoadedMap.end()){
				cGameMapHeightGridSystem* ret = new cGameMapHeightGridSystem;
				ret->LoadFromFiles(m_vecMapRawPath[m_nCurrentMapIndex], m_vecMapTexturePath[m_nCurrentMapIndex]);
				m_mapLoadedMap[m_vecMapRawPath[m_nCurrentMapIndex]] = ret;
				m_pMousPicking->SetVertex(ret->GetVertexVectorByRef());
				m_pCurrentMap->Release();
				m_pCurrentMap = ret;
				m_pCurrentMap->AddRef();
			}
			else{
				m_pCurrentMap->Release();
				m_pCurrentMap = m_mapLoadedMap[m_vecMapRawPath[m_nCurrentMapIndex]];
				cGameMapHeightGridSystem* ret = (cGameMapHeightGridSystem*)m_pCurrentMap;
				m_pMousPicking->SetVertex(ret->GetVertexVectorByRef());
				m_pCurrentMap->AddRef();
			}
		}
	}
}

void cSceneEditMode::Render(){
	cScene::Render();

	std::stringstream s;
	s.precision(2);
	if (m_pCurrentMap){
		s << "Map Binded: " << m_vecMapRawPath[m_nCurrentMapIndex] << std::endl;
	}
	
	g_pFontManager->GetFont(g_pFontManager->FONT_DEFAULT)->DrawText(NULL,				 //pSprite
					s.str().c_str(),	 //pString
					-1,					//Count
					&m_rectFontArea,	//pRect
					DT_LEFT | DT_NOCLIP,//Format,
					0xFFFFFFFF);		//Color
}

// 모든 리소스를 해제한다.
void cSceneEditMode::Exit(){
	cScene::Exit();	
}

void cSceneEditMode::AddMapPath(std::string sRawPath, std::string sTexturePath){
	m_vecMapRawPath.push_back(sRawPath);
	m_vecMapTexturePath.push_back(sTexturePath);
}

void cSceneEditMode::LoadNextMap(LPVOID pParam){
	// todo
}
void cSceneEditMode::LoadNextObj(LPVOID pParam){
	// todo
}