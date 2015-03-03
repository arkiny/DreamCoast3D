#pragma once
#include "cUIWindow.h"

class cUIPopupWindow;

class cUISystemMenu : public cUIWindow, public iButtonDelegate
{
protected:
	cUIPopupWindow*	m_pPopUpUI;
	bool			m_bPoppedUP;

public:
	cUISystemMenu();
	virtual ~cUISystemMenu();

	virtual void Setup();
	virtual void Update(float fDelta);
	virtual void Render();

	virtual void OnClick(cUIImageButton* pSender) override;
};

