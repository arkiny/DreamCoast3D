#pragma once
#include "cUIWindow.h"
#include "cUIPopupWindow.h"

class cStoreWindow : public cUIPopupWindow, public iButtonDelegate
{
public:
	cStoreWindow();
	virtual ~cStoreWindow();

	virtual void Setup();
	virtual void Update(float fDelta);
	virtual void Render();

	virtual void OnClick(cUIImageButton* pSender);

	cUIImageButton* m_pHealthButton;
	cUIImageButton* m_pManaButton;
};

