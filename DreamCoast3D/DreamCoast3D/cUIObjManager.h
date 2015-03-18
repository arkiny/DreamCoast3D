#pragma once

class cUIObject;
class cUICursor;

class cUIObjManager : public cObject, public iUIManagerDeligate
{
protected:
	// TODO : Z-Indexing
	// HACK : Prioriy Queue(Heap)�� ó���ϸ� ���� ���ϴ�.
	std::vector<cUIObject*> m_vecUIObjects;

	// Update Stack�� ���ֱ� ����
	bool m_bSceneChange = false;
	iSceneDelegate* m_pSceneManager;
	int m_nNextScene = -1;

	cUICursor* m_pMouseCursor;

	iGameEventDelegate* m_pEventManager;

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

	virtual void Destroy();

	// override iUIManagerDeligate
	virtual void ChangeScene(int nScene, cUIObject* pSender) override;
	virtual void SetShowCursor(bool isShow);
};

