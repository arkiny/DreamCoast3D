#pragma once
#include "cUIWindow.h"

class cUIPopupWindow;

class cUISystemMenu : public cUIWindow, public iButtonDelegate
{
protected:
	SYNTHESIZE(cUIPopupWindow*, m_pPopUpSystemMenuUI, PopUpSystemMenuUI);

public:
	cUISystemMenu();
	virtual ~cUISystemMenu();

	virtual void Setup();
	virtual void Update(float fDelta);
	virtual void Render();

	virtual void OnClick(cUIImageButton* pSender) override;
};

