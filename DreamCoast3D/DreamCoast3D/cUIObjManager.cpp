#include "stdafx.h"
#include "cUIObjManager.h"
#include "cUICursor.h"
#include "cUIPopupWindow.h"

cUIObjManager::cUIObjManager()
{
	m_isTrap = false;
	m_isCursorUpdate = false;
	m_pEventManager = nullptr;
	m_pGameManager = nullptr;
	m_isRender = false;
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
		if (m_isRender == false)
		{
			p->Update(fDelta);
		}
		else
		{
			m_vecUIGameOver[0]->Update(fDelta);
		}

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
	if (m_pEventManager)
	{
		m_pEventManager->StoreEvent();
	}
	UpdateTrap();

}

void cUIObjManager::Render(){	
	for (auto p : m_vecUIObjects){
		if (m_isRender == false)
		{
			SAFE_RENDER(p);
		}
		else
		{
			m_vecUIGameOver[0]->Render();
		}

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

void cUIObjManager::AddGameOver(cUIObject* pGameOver){
	if (pGameOver){
		m_vecUIGameOver.push_back(pGameOver);
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
	m_pGameManager = pGameManager;
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

void cUIObjManager::UpdateTrap()
{
	if (m_pGameManager)
	{
		if (m_pGameManager->GetPlayerData())
		{
			if (m_isCursorUpdate == false)
			{
				if (g_pControlManager->GetInputInfo('T'))
				{
					m_isTrap = false;
					this->SetShowCursor(true);
				}
				else
				{
					m_isTrap = true;
					this->SetShowCursor(false);
				}
			}
		}
		else
		{
			m_isTrap = false;
			this->SetShowCursor(true);
		}
	}
}

void cUIObjManager::ActiveGameOver(bool isGameOver)
{
	if (m_vecUIGameOver.size() > 0)
	{
		cUIPopupWindow* pGameOver = (cUIPopupWindow*)m_vecUIGameOver[0];
		pGameOver->SetisPopped(isGameOver);
		m_isRender = isGameOver;
	}

}