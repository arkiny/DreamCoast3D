#include "stdafx.h"
#include "cGameEventManager.h"


cGameEventManager::cGameEventManager()
	: m_pGridTileSystem(NULL)
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
		if (p->GetEventType() == cGameObject::eEventType::E_STORE)
		{

		}
	}
}