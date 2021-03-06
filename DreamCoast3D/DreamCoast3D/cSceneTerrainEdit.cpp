#include "stdafx.h"
#include "cSceneTerrainEdit.h"
#include "cHeightMapTerrainEdit.h"
#include "cMousePicking.h"
#include "cEffectManager.h"
#include "cLightSource.h"
#include "cGameObjManager.h"
#include "cUIObjManager.h"
#include "cTransform.h"
#include "cUILoader.h"
#include "cEffectManager.h"
#include "cMousePicking.h"
#include "cCamera.h"
#include "cEditCursor.h"
#include "cUISet4Editor.h"
#include <sstream>

cSceneTerrainEdit::cSceneTerrainEdit()
	:m_pMousPicking(NULL),
	m_pCursor(NULL),
	m_pEditMap(NULL)
{
}


cSceneTerrainEdit::~cSceneTerrainEdit()
{
	//SAFE_RELEASE(m_pCurrentMap);
	SAFE_DELETE(m_pMousPicking);
	SAFE_RELEASE(m_pCursor);
	SAFE_RELEASE(m_pEditMap);
}


void cSceneTerrainEdit::Setup(std::string sFilePath){
	cScene::Setup(sFilePath);
	m_pMousPicking = new cMousePicking;
	m_pMousPicking->SetEffectDelegate(m_pEffectManager);

	m_pCursor = new cEditCursor;
	m_pCursor->Setup();
}

// Start에서 리소스를 로딩한다.
void cSceneTerrainEdit::Start(){
	cUILoader cUL;
	cUL.LoadGameUIFromFile(this->GetUIObjMng(), m_sGameUIPath);
	
	///
	cUISet4Editor* p = new cUISet4Editor;
	p->Setup();
	m_pUIObjManager->AddUI(p);
	p->Release();
	///

	for (auto p : m_vecLightSources){
		p->Start();
	}

	m_pEditMap = new cHeightMapTerrainEdit;
	m_pEditMap->Setup();
	m_pCurrentMap = m_pEditMap;
	m_pEditMap->AddRef();

	if (m_pUIObjManager){
		if (m_pDelegate){
			// UI에 씬매니저 델리게이트 어태치
			m_pUIObjManager->SetSceneDeligate(m_pDelegate);
		}
		// UI매니저 시작
		m_pUIObjManager->Start();
	}
}

void cSceneTerrainEdit::Update(float delta){
	m_fDelayAcuum += delta;
	if (m_fDelayAcuum > m_fDelay){
		m_fDelayAcuum = m_fDelay+1.0f;
	}

	if (m_pCamera){
		m_pCamera->Update(delta);
	}
	if (m_pCurrentMap){
		m_pCurrentMap->Update(delta);
	}	
	if (m_pMousPicking && m_pCurrentMap){
		m_pMousPicking->SetVertex(m_pCurrentMap->GetVertexVectorByRef());
		m_pMousPicking->Update();

		if (m_pCursor){
			m_pCursor->SetPosition(m_pMousPicking->GetPickingPoint());
		}

	}
	if (m_pEffectManager){
		m_pEffectManager->Update(delta);
	}

	// 해당 칸 선택
	if (GetAsyncKeyState(VK_RBUTTON))
	{
		m_pEditMap->SetSelectTile(m_pMousPicking->GetPickingPoint());
		m_pEditMap->SetRButton(true);
	}
	
	if (!GetAsyncKeyState(VK_RBUTTON))
	{
		m_pEditMap->SetSelectTile(D3DXVECTOR3(-1.f, -1.f, -1.f));
		m_pEditMap->SetRButton(false);
	}

	if (GetAsyncKeyState(VK_LBUTTON)){
		m_bIsClickDown = true;
		m_pEditMap->SetClickFrom(D3DXVECTOR3(-1, -1, -1));
		m_pEditMap->SetClickTo(D3DXVECTOR3(-1, -1, -1));
		m_pEditMap->SetTileFrom(D3DXVECTOR3(-1, -1, -1));
		m_pEditMap->SetTileTo(D3DXVECTOR3(-1, -1, -1));
	}

	if (GetAsyncKeyState(VK_LBUTTON) == false && m_bIsClickDown == true){
		m_pEditMap->SetClickFrom(m_pMousPicking->GetPickingPoint());
		m_pEditMap->SetClickTo(m_pMousPicking->GetPickingPoint());
		m_pEditMap->SetTileFrom(m_pMousPicking->GetPickingPoint());
		D3DXVECTOR3 pos = m_pMousPicking->GetPickingPoint();
		pos.x += 1.0f;
		pos.z += 1.0f;
		m_pEditMap->SetTileTo(pos);
		m_bIsClickDown = false;
	}

	if (g_pControlManager->GetInputInfo('P') && m_fDelayAcuum > m_fDelay){
		m_fDelayAcuum = 0;
		D3DXVECTOR3 tileto = m_pEditMap->GetTileTo();
		tileto.x += m_pEditMap->GetHeight();
		tileto.z += m_pEditMap->GetWidth();
		m_pEditMap->SetTileTo(tileto);
		m_bOnKeyDown = true;
	}
	if (g_pControlManager->GetInputInfo('O') && m_fDelayAcuum > m_fDelay){
		m_fDelayAcuum = 0;
		D3DXVECTOR3 tileto = m_pEditMap->GetTileTo();
		tileto.x -= m_pEditMap->GetHeight();
		tileto.z -= m_pEditMap->GetWidth();
		m_pEditMap->SetTileTo(tileto);
		m_bOnKeyDown = true;
	}

	if (m_bOnKeyDown
		&& (g_pControlManager->GetInputInfo('O') == false && g_pControlManager->GetInputInfo('P') == false)){
		m_bOnKeyDown = false;
		m_fDelayAcuum = m_fDelay + 1.0f;
	}

	if (m_pUIObjManager){
		m_pUIObjManager->Update(delta);
	}
}

void cSceneTerrainEdit::Render(){
	if (m_pCurrentMap){
		m_pCurrentMap->Render();
	}
	if (m_pEffectManager){
		m_pEffectManager->Render();
	}
	if (m_pUIObjManager){
		m_pUIObjManager->Render();
	}
	if (m_pCursor){
		m_pCursor->Render();
	}
}

void cSceneTerrainEdit::Exit(){

}