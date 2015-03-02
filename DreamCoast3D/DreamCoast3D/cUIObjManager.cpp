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
	for (size_t i = 0; i < m_vecUIObjects.size(); i++){
		m_vecUIObjects[i]->Update(fDelta);
	}
}

void cUIObjManager::Render(){
	for (size_t i = 0; i < m_vecUIObjects.size(); i++){
		SAFE_RENDER(m_vecUIObjects[i]);
	}
}

void cUIObjManager::AddUI(cUIObject* pUIObj){
	if (pUIObj){
		SAFE_ADD_REF(pUIObj);
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
}

void cUIObjManager::Start(){
	for (auto p : m_vecUIObjects){
		p->Start();
	}
}
