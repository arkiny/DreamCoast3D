#include "stdafx.h"
#include "cUIObjManager.h"


cUIObjManager::cUIObjManager()
{
}


cUIObjManager::~cUIObjManager()
{
}

void cUIObjManager::Setup(){
	// ���� ���� �н��� �޾ƿͼ� �ε�
}

void cUIObjManager::Update(float fDelta){
	for (auto p : m_vecUIObjects){
		p->Update(fDelta);
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
		m_vecUIObjects.push_back(pUIObj);
	}
}

void cUIObjManager::RemoveUI(){
	// TODO : UI ����?
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
