#pragma once
#include "cUIPopupWindow.h"

class cGameOverWindow : public cUIPopupWindow, public iButtonDelegate
{
public:
	cGameOverWindow();
	virtual ~cGameOverWindow();

	virtual void Setup();
	virtual void Update(float fDelta);
	virtual void Render();

	virtual void OnClick(cUIImageButton* pSender);

	cUIImageButton* m_pReturnButton;
	cUIImageButton* m_pExitButton;
};

