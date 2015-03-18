#include "stdafx.h"
#include "cUIObjManager.h"
#include "cUICursor.h"

cUIObjManager::cUIObjManager()
{
}


cUIObjManager::~cUIObjManager()
{
}

void cUIObjManager::Setup(){
	// ���� ���� �н��� �޾ƿͼ� �ε�
	m_pMouseCursor = new cUICursor;
	std::string sFile = "../Resources/Cursor/cur116.png";
	m_pMouseCursor->SetTextureFilename(sFile);
}

void cUIObjManager::Update(float fDelta){
	if (m_pMouseCursor){
		m_pMouseCursor->Update(fDelta);
	}
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

	m_pEventManager->StoreEvent();

}

void cUIObjManager::Render(){	
	for (auto p : m_vecUIObjects){
		SAFE_RENDER(p);
	}
	SAFE_RENDER(m_pMouseCursor);
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
	if (m_pMouseCursor){
		m_pMouseCursor->Release();
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

void cUIObjManager::SetEventDeligate(iGameEventDelegate* pEventManager)
{
	for (auto p : m_vecUIObjects)
	{
		p->SetEventDeligate(pEventManager);
		pEventManager->SetSceneDelegate(m_pSceneManager);
		pEventManager->SetUIDelegate(this);
	}
	m_pEventManager = pEventManager;
}

void cUIObjManager::SetEffectDeligate(iEffectManagerDelegate* pEffectManager){
	for (auto p : m_vecUIObjects){
		p->SetEffectDelegate(pEffectManager);
	}
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

void cUIObjManager::SetShowCursor(bool isShow){
	if (m_pMouseCursor){
		m_pMouseCursor->SetIsShow(isShow);
	}
}