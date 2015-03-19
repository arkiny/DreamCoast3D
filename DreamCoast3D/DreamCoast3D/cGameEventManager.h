#pragma once
#include "iGameEventDelegate.h"

class cGameEvent;


class cGameEventManager : public cObject, public iGameEventDelegate
{
protected:
	std::set<cGameEvent*> m_setEvent;
	std::vector<cGameEvent*> m_vecEvent;

private:
	iGridTileSystem* m_pGridTileSystem;
	

public:
	cGameEventManager();
	virtual ~cGameEventManager();

	virtual void Setup();
	virtual void Start();
	virtual void Update(float delta);

	virtual void Exit();

	virtual void AddEvent(cGameEvent* pEvnet);
	virtual void DeleteEvent(cGameEvent* pEvent);

	virtual void Destory();

	void SetCurrentTileSystem(iGridTileSystem* pGridSystem);


	/////

	void CheckEventFromRange(cGameObject* pFrom, int nRange);
	void SetSceneDelegate(iSceneDelegate* pSceneDelegate);
	void SetUIDelegate(iUIManagerDeligate* m_pUIDelegate);

private:
	iSceneDelegate* m_pSceneDelegate;
	iUIManagerDeligate* m_pUIDelegate;
	
	bool m_isPause;
	bool m_isStoreButton;
	float m_fPassTime;
	void StoreEvent();
	void StoreKeyUpdate(float fDelta);
	bool GetStoreState()
	{
		return m_isPause;
	}
	void SetStoreState(bool isPause)
	{
		m_isPause = isPause;
	}


};

