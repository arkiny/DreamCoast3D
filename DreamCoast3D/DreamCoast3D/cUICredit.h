#pragma once
#include "cUIWindow.h"
class cUICredit :
	public cUIWindow, public iButtonDelegate
{
public:
	cUICredit();
	virtual ~cUICredit();

	virtual void Setup();
	virtual void Update(float fDelta);
	virtual void Render();

	virtual void OnClick(cUIImageButton* pSender){}
	virtual void SetSceneDeligate(iSceneDelegate* pSceneDeligate);
};

