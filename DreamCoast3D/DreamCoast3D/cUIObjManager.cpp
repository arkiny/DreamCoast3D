#include "stdafx.h"
#include "cUIObjManager.h"


cUIObjManager::cUIObjManager()
{
}


cUIObjManager::~cUIObjManager()
{
}

void cUIObjManager::Setup(){
	// 차후 파일 패스를 받아와서 로드
}

void cUIObjManager::Update(float fDelta){
	for (auto p : m_vecUIObjects){
		p->Update(fDelta);
	}

	// XXX 
	// 자기가 삭제되는 위험한 경우는 무조건 모든 루프가 끝난뒤
	// 실행되어야 한다.
	// 루프는 자기가 삭제된 뒤에도 계속 돌기 때문이다.
	if (m_bSceneChange){
		if (m_nNextScene >= 0){
			m_pSceneManager->ChangeScene(m_nNextScene);
		}
		else if (m_nNextScene < -1){
			PostQuitMessage(NULL);
		}
	}
}

void cUIObjManager::Render(){
	for (auto p : m_vecUIObjects){
		SAFE_RENDER(p);
	}
}

void cUIObjManager::AddUI(cUIObject* pUIObj){
	if (pUIObj){
		SAFE_ADD_REF(pUIObj);
		pUIObj->SetUIManagerDeligate(this);
		m_vecUIObjects.push_back(pUIObj);
	}
}

void cUIObjManager::RemoveUI(){
	// TODO : UI 삭제?
}

void cUIObjManager::Destroy(){
	for (auto p : m_vecUIObjects){
		SAFE_RELEASE(p);
	}
	this->Release();
}

void cUIObjManager::SetGameObjDeligate(iGameObjectDelegate* pGameManager){
	for (auto p : m_vecUIObjects){
		p->SetGameObjDeligate(pGameManager);
	}
}

void cUIObjManager::SetSceneDeligate(iSceneDelegate* pSceneManager){
	for (auto p : m_vecUIObjects){
		p->SetSceneDeligate(pSceneManager);
	}
	m_pSceneManager = pSceneManager;
}

void cUIObjManager::Start(){
	for (auto p : m_vecUIObjects){
		p->Start();
	}
}

void cUIObjManager::ChangeScene(int nScene, cUIObject* pSender){
	m_nNextScene = nScene;
	m_bSceneChange = true;
}