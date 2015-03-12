#pragma once
#include "cUIImageView.h"
class cUICursor :
	public cUIImageView
{
	SYNTHESIZE(bool, m_bIsShow, IsShow);
public:
	cUICursor();
	virtual ~cUICursor();

	cUICursor(LPD3DXSPRITE pSprite);
	virtual void Update(float fDelta) override;
	virtual void Render() override;
};

