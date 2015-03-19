#include "stdafx.h"
#include "cGameEventManager.h"
#include "cGamePlayableObject.h"


cGameEventManager::cGameEventManager()
	: m_pGridTileSystem(NULL)
	, m_pSceneDelegate(NULL)
	, m_pUIDelegate(NULL)
	, m_isPause(false)
	, m_isStoreButton(false)
	, m_fPassTime(0.f)
	, m_fDeadAccumTime(0.f)
{
}


cGameEventManager::~cGameEventManager()
{
}

void cGameEventManager::Destory()
{
	//for (auto p : m_setEvent)
	//{
	//	SAFE_DELETE(p);
	//}
	this->Release();
}

void cGameEventManager::Setup()
{

}

void cGameEventManager::Start()
{

}

void cGameEventManager::Update(float delta)
{
	StoreKeyUpdate(delta);
}

void cGameEventManager::Exit()
{

}

void cGameEventManager::AddEvent(cGameEvent* pEvnet)
{
	if (pEvnet)
	{
		m_setEvent.insert(pEvnet);
	}
}

void cGameEventManager::DeleteEvent(cGameEvent* pEvnet)
{

}

///

void cGameEventManager::SetCurrentTileSystem(iGridTileSystem* pGridSystem)
{
	m_pGridTileSystem = pGridSystem;
}

///


void cGameEventManager::CheckEventFromRange(cGameObject* pFrom, int nRange)
{
	D3DXVECTOR3 vFrom(0.f, 0.f, 0.f);
	vFrom = pFrom->GetPosition();

	std::vector<cGameObject*> vecGameObject;
	vecGameObject = m_pGridTileSystem->GetAdjObjectCustomer(vFrom.x, vFrom.z, nRange);


	for (auto p : vecGameObject)
	{
		if (p == pFrom)
		{
			continue;
		}
		if (p->GetEventType() == cGameObject::eEventType::E_PORTAL)
		{
			m_pSceneDelegate->ChangeScene(4);
		}
	}
}

void cGameEventManager::SetSceneDelegate(iSceneDelegate* pSceneDelegate)
{
	m_pSceneDelegate = pSceneDelegate;
}

void cGameEventManager::StoreEvent()
{
	//if (m_isStoreButton == false)
	//{
	//	if (g_pControlManager->GetInputInfo('I'))
	//	{
	//		if (m_isPause == false)
	//		{
	//			m_isPause = true;
	//		}
	//		else if (m_isPause == true)
	//		{
	//			m_isPause = false;
	//		}
	//	}
	//}
	m_pSceneDelegate->PauseObjectUpdate(m_isPause);
}

void cGameEventManager::StoreKeyUpdate(float fDelta)
{
	if (g_pControlManager->GetInputInfo('I') && m_isStoreButton == false)
	{
		m_isStoreButton = true;
	}
	if (m_isStoreButton == true)
	{
		m_fPassTime += fDelta;
	}
	if (m_fPassTime >= 0.1f)
	{
		m_isStoreButton = false;
		m_fPassTime = 0.f;
	}
}

void cGameEventManager::SetUIDelegate(iUIManagerDeligate* m_pUIDelegate)
{
	m_pUIDelegate = m_pUIDelegate;
}

void cGameEventManager::PlayerDead(cGameObject* pFrom)
{
	pFrom->ChangeState(cGamePlayableObject::EPLAYABLESTATE::EPLAYABLESTATE_DEAD);
}

void cGameEventManager::DeadScene()
{
	m_pSceneDelegate->ChangeScene(0);
}