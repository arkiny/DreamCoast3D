#pragma once

class cGameObject;

__interface iGameEventDelegate
{
	virtual void CheckEventFromRange(cGameObject* pFrom, int nRange);
	virtual void SetCurrentTileSystem(iGridTileSystem* pGridSystem);
	virtual void SetSceneDelegate(iSceneDelegate* pSceneDelegate);
	virtual void StoreEvent();
	virtual void SetUIDelegate(iUIManagerDeligate* m_pUIDelegate);
	virtual bool GetStoreState();
	virtual void SetStoreState(bool isPause);
	virtual void PlayerDead(cGameObject* pFrom);
	virtual void DeadScene(bool isDead);
};