#pragma once
#include "cUIImageButton.h"

class cUIImageButtonMenu : public cUIImageButton
{
protected:
	SYNTHESIZE(iMenuControlDelegate*, m_pMenuDelegate, MenuDelegate);
	SYNTHESIZE(int, m_nTarget, Target);

public:
	cUIImageButtonMenu(LPD3DXSPRITE pSprite);
	virtual ~cUIImageButtonMenu();

	virtual void Setup(
		std::string sNormal,
		std::string sMouseOver,
		std::string sSelected);

	virtual void Update(float fDelta) override;

	virtual void Render() override;
};

