#pragma once
#include "cUIImageButton.h"

class cUISceneChangeButton :
	public cUIImageButton
{
protected:
	SYNTHESIZE(int, m_nTargetScene, TargetScene);

public:
	cUISceneChangeButton(LPD3DXSPRITE pSprite);
	virtual ~cUISceneChangeButton();

	virtual void Update(float fDelta) override;
};

