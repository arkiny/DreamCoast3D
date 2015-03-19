#pragma once

class cUIObject;
class cUICursor;

class cUIObjManager : public cObject, public iUIManagerDeligate
{
protected:
	// TODO : Z-Indexing
	// HACK : Prioriy Queue(Heap)로 처리하면 좋을 듯하다.
	std::vector<cUIObject*> m_vecUIObjects;

	

	// Update Stack을 없애기 위함
	bool m_bSceneChange = false;
	iSceneDelegate* m_pSceneManager;
	int m_nNextScene = -1;

	cUICursor* m_pMouseCursor;

	iGameEventDelegate* m_pEventManager;
	iInventoryControl* m_pInventoryControl;
	iGameObjectDelegate* m_pGameManager;
	SYNTHESIZE(bool, m_isTrap, Trap);
	SYNTHESIZE(bool, m_isCursorUpdate, CursorUpdate);
private:
	void UpdateTrap();

public:
	cUIObjManager();
	virtual ~cUIObjManager();

	virtual void Start();
	virtual void Setup();
	virtual void Update(float fDelta);
	virtual void Render();

	virtual void AddUI(cUIObject* pUIRoot);
	virtual void RemoveUI();

	virtual void SetGameObjDeligate(iGameObjectDelegate* pGameManager);
	virtual void SetSceneDeligate(iSceneDelegate* pSceneManager);
	virtual void SetEventDeligate(iGameEventDelegate* pEventManager);
	virtual void SetEffectDeligate(iEffectManagerDelegate* pEffectManager);

	virtual void Destroy();

	// override iUIManagerDeligate
	virtual void ChangeScene(int nScene, cUIObject* pSender) override;
	virtual void SetShowCursor(bool isShow);

	void SetInventoryControl(iInventoryControl* pInventoryControl)
	{
		m_pInventoryControl = pInventoryControl;
	}
	iInventoryControl* GetInventoryControl()
	{
		return m_pInventoryControl;
	}


};

