#pragma once
#include "cUIWindow.h"
class cUICredit :
	public cUIWindow, public iButtonDelegate
{
protected:
	cUIObject* m_pToMainButton;

public:
	cUICredit();
	virtual ~cUICredit();

	virtual void Setup();
	virtual void Update(float fDelta);
	virtual void Render();

	virtual void OnClick(cUIImageButton* pSender){}
};

