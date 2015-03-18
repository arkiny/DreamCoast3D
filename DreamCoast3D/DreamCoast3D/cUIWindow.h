#pragma once

class cUIWindow : public cUIObject
{
protected:
	RECT					m_rectClickArea;
	//cUIObject*			m_pUIRoot;
	SYNTHESIZE(cUIObject*, m_pUIRoot, UIRoot); //현재 윈도우의 최상위 부모. 이 Root부터 Child를 순회하는 구조임. : 민우

public:
	cUIWindow();
	virtual ~cUIWindow();

	virtual void Setup();
	virtual void Update(float fDelta);
	virtual void Render();

	void SetUIManagerDeligate(iUIManagerDeligate* pUImanagerDeligate);
};

