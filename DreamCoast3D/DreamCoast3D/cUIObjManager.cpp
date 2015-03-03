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

	// XXX 
	// �ڱⰡ �����Ǵ� ������ ���� ������ ��� ������ ������
	// ����Ǿ�� �Ѵ�.
	// ������ �ڱⰡ ������ �ڿ��� ��� ���� �����̴�.
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