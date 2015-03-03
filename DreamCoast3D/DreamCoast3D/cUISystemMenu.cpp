#include "stdafx.h"
#include "cUISystemMenu.h"
#include "cUIPopupWindow.h"
#include "cUIImageButton.h"


cUISystemMenu::cUISystemMenu()
	:m_pPopUpUI(NULL)
{
}


cUISystemMenu::~cUISystemMenu()
{
	SAFE_RELEASE(m_pSprite);
	if (m_pUIRoot)
		m_pUIRoot->Destroy();
	if (m_pPopUpUI){
		m_pPopUpUI->Destroy();
	}
}

void cUISystemMenu::Setup(){
	D3DXCreateSprite(g_pD3DDevice, &m_pSprite);

	cUIPopupWindow* p = new cUIPopupWindow;
	p->Setup();
	m_pPopUpUI = p;

	// 시작 버튼
	cUIImageButton* pTestButton = new cUIImageButton(m_pSprite);
	pTestButton->Setup(
		std::string("../Resources/UI/UI_AcceptButton.png"),
		std::string("../Resources/UI/UI_AcceptButtonOn.png"),
		std::string("../Resources/UI/UI_AcceptButtonClick.png"));
	pTestButton->SetPosition(D3DXVECTOR3(90, 100, 0));
	pTestButton->SetDelegate(this);
	m_pUIRoot = pTestButton;
}

void cUISystemMenu::Update(float fDelta){
	if (m_pUIRoot){
		m_pUIRoot->Update(fDelta);
	}
	if (m_pPopUpUI){
		m_pUIRoot->Update(fDelta);
	}
}

void cUISystemMenu::Render(){
	if (m_pUIRoot){
		m_pUIRoot->Render();
	}
	if (m_pPopUpUI){
		m_pPopUpUI->Render();
	}
}

void cUISystemMenu::OnClick(cUIImageButton* pSender){
	if (m_pPopUpUI){
		if (m_pPopUpUI->GetisPopped() == false){
			m_pPopUpUI->SetisPopped(true);
		}
		else {
			m_pPopUpUI->SetisPopped(false);
		}
	}
}