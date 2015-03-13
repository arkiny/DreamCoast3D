#pragma once
#include "cUIWindow.h"

class cUISet4Editor : public cUIWindow, public iButtonDelegate
{
public:
	cUISet4Editor();
	virtual ~cUISet4Editor();

	virtual void Setup();
	virtual void Start();
	virtual void Update(float fDelta);
	virtual void Render();

	void OnClick(cUIImageButton* pSender) override;
};

