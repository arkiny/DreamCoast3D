#pragma once
class cUIImageButton;
__interface iButtonDelegate
{
	virtual void OnClick(cUIImageButton* pSender);
};

class cUIImageButtonMenu;
__interface iMenuControlDelegate
{
	virtual void MenuControl(cUIImageButtonMenu* pSender, int target);
};

