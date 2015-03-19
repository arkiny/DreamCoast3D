#pragma once
#include "cUIWindow.h"

class cUIPopupWindow;
class cUIImageButtonMenu;

class cUISystemMenu : public cUIWindow, public iButtonDelegate, public iMenuControlDelegate
{
protected:
	std::vector<cUIPopupWindow*> m_vecPopUpWindow;
	std::vector<cUIImageButtonMenu*> m_vecButtons;
	//SYNTHESIZE(cUIPopupWindow*, m_pPopUpSystemMenuUI, PopUpSystemMenuUI);

public:
	cUISystemMenu();
	virtual ~cUISystemMenu();

	virtual void Setup();
	virtual void Update(float fDelta);
	virtual void Render();
	virtual void AddPopUpWindowAndButton(cUIImageButtonMenu* pButton, cUIPopupWindow* pWindow);

	
	virtual void OnClick(cUIImageButton* pSender) override;
	virtual void MenuControl(cUIImageButtonMenu* pSender, int target) override;

private:

	//MS
	bool m_isStoreButton;
	float m_fPassTime;

	void StoreEvent(float fDelta);
};

