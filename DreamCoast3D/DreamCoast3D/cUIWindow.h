#pragma once
class cUIWindow : public cUIObject
{
protected:
	RECT					m_rectClickArea;
	cUIObject*				m_pUIRoot;

public:
	cUIWindow();
	virtual ~cUIWindow();

	virtual void Setup();
	virtual void Update(float fDelta);
	virtual void Render();
};

