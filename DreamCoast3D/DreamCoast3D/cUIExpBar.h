#pragma once
class cUIExpBar : public cUIWindow
{
protected:
	RECT							m_rectClickArea;

	float							m_fCurrentExp;
	float							m_fMaxExp;

public:
	cUIExpBar();
	virtual ~cUIExpBar();

	virtual void Setup();
	virtual void Update(float fDelta);
	virtual void Render();
};

