#include "stdafx.h"
#include "cGameEventManager.h"


cGameEventManager::cGameEventManager()
{
}


cGameEventManager::~cGameEventManager()
{
}

void cGameEventManager::Destory()
{
	for (auto p : m_setEvent)
	{
		SAFE_DELETE(p);
	}
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