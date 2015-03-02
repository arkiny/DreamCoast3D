#pragma once

class cUIObject;

class cUIObjManager : public cObject
{
protected:
	// TODO : Z-Indexing
	// HACK : Prioriy Queue(Heap)로 처리하면 좋을 듯하다.
	std::vector<cUIObject*> m_vecUIObjects;

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

	virtual void Destroy();
};

