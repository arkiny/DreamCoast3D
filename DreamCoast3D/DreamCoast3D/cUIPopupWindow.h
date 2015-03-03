#pragma once
#include "cUIWindow.h"

class cUIPopupWindow : public cUIWindow, public iButtonDelegate
{
protected:
	SYNTHESIZE(bool, m_bShow, isPopped);

public:
	cUIPopupWindow();
	virtual ~cUIPopupWindow();

	virtual void Setup();
	virtual void Start();
	virtual void Update(float fDelta);
	virtual void Render();

	virtual void OnClick(cUIImageButton* pSender);
};

