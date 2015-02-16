#pragma once

class cUIObject;

class cUIObjManager : public cObject
{
protected:
	// TODO : Z-Indexing
	// HACK : Prioriy Queue(Heap)�� ó���ϸ� ���� ���ϴ�.
	std::vector<cUIObject*> m_vecUIObjects;

public:
	cUIObjManager();
	virtual ~cUIObjManager();

	virtual void Setup();
	virtual void Update(float fDelta);
	virtual void Render();

	virtual void AddUI(cUIObject* pUIRoot);
	virtual void RemoveUI();

	virtual void Destroy();
};
