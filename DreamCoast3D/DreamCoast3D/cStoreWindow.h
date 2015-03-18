#pragma once
#include "cUIWindow.h"

class cStoreWindow : public cUIWindow, public iButtonDelegate
{
public:
	cStoreWindow();
	~cStoreWindow();

	virtual void Setup();
	virtual void Update(float fDelta);
	virtual void Render();

	virtual void OnClick(cUIImageButton* pSender);

	cUIImageButton* m_pHealthButton;
	cUIImageButton* m_pManaButton;
};

