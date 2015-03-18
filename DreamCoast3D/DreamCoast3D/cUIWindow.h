#pragma once

class cUIWindow : public cUIObject
{
protected:
	RECT					m_rectClickArea;
	//cUIObject*			m_pUIRoot;
	SYNTHESIZE(cUIObject*, m_pUIRoot, UIRoot); //���� �������� �ֻ��� �θ�. �� Root���� Child�� ��ȸ�ϴ� ������. : �ο�

public:
	cUIWindow();
	virtual ~cUIWindow();

	virtual void Setup();
	virtual void Update(float fDelta);
	virtual void Render();

	void SetUIManagerDeligate(iUIManagerDeligate* pUImanagerDeligate);
};

