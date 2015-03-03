#include "stdafx.h"
#include "cUISystemMenu.h"
#include "cUIPopupWindow.h"
#include "cUIImageButton.h"
#include "cUIImageButtonMenu.h"


cUISystemMenu::cUISystemMenu()
{
}


cUISystemMenu::~cUISystemMenu()
{
	SAFE_RELEASE(m_pSprite);
	for (auto p : m_vecButtons){
		SAFE_RELEASE(p);
	}
	for (auto p : m_vecPopUpWindow){
		SAFE_RELEASE(p);
	}
	//SAFE_RELEASE(m_pPopUpSystemMenuUI);
	if (m_pUIRoot)
		m_pUIRoot->Destroy();
}

void cUISystemMenu::Setup(){
	D3DXCreateSprite(g_pD3DDevice, &m_pSprite);

	//cUIPopupWindow* p = new cUIPopupWindow;
	//p->Setup();
	//m_pPopUpSystemMenuUI = p;

	

	//m_pUIRoot = pTestButton;
}

void cUISystemMenu::Update(float fDelta){
	if (m_pUIRoot){
		m_pUIRoot->Update(fDelta);
	}
}

void cUISystemMenu::Render(){
	if (m_pUIRoot){
		m_pUIRoot->Render();
	}
}

void cUISystemMenu::OnClick(cUIImageButton* pSender){
	//if (m_pPopUpSystemMenuUI){
	//	if (m_pPopUpSystemMenuUI->GetisPopped() == false){
	//		m_pPopUpSystemMenuUI->SetisPopped(true);
	//	}
	//	else {
	//		m_pPopUpSystemMenuUI->SetisPopped(false);
	//	}
	//}
}

void cUISystemMenu::AddPopUpWindowAndButton(cUIImageButtonMenu* pButton, cUIPopupWindow* pWindow){
	if (pWindow){
		SAFE_ADD_REF(pWindow);
		m_vecPopUpWindow.push_back(pWindow);
	}
	if (pButton){
		SAFE_ADD_REF(pButton);
		m_vecButtons.push_back(pButton);
		if (!m_vecPopUpWindow.empty()){
			pButton->SetTarget(m_vecPopUpWindow.size() - 1);
			pButton->SetDelegate(this);
			pButton->SetMenuDelegate(this);
		}
	}
}

void cUISystemMenu::MenuControl(cUIImageButtonMenu* pSender, int target){
	if (m_vecPopUpWindow[target]->GetisPopped()){
		m_vecPopUpWindow[target]->SetisPopped(false);
	}
	else{
		m_vecPopUpWindow[target]->SetisPopped(true);
	}	
}