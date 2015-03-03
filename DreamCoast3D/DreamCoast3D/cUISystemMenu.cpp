#include "stdafx.h"
#include "cUISystemMenu.h"
#include "cUIPopupWindow.h"
#include "cUIImageButton.h"


cUISystemMenu::cUISystemMenu()
	:m_pPopUpSystemMenuUI(NULL)
{
}


cUISystemMenu::~cUISystemMenu()
{
	SAFE_RELEASE(m_pSprite);
	//SAFE_RELEASE(m_pPopUpSystemMenuUI);
	if (m_pUIRoot)
		m_pUIRoot->Destroy();
}

void cUISystemMenu::Setup(){
	D3DXCreateSprite(g_pD3DDevice, &m_pSprite);

	//cUIPopupWindow* p = new cUIPopupWindow;
	//p->Setup();
	//m_pPopUpSystemMenuUI = p;

	// 시작 버튼
	cUIImageButton* pTestButton = new cUIImageButton(m_pSprite);
	pTestButton->Setup(
		std::string("../Resources/UI/UI_AcceptButton.png"),
		std::string("../Resources/UI/UI_AcceptButtonOn.png"),
		std::string("../Resources/UI/UI_AcceptButtonClick.png"));
	pTestButton->SetDelegate(this);

	RECT tempRect;
	GetClientRect(g_hWnd, &tempRect);

	float x = tempRect.right - pTestButton->GetSize().fWidth;
	float y = tempRect.bottom - pTestButton->GetSize().fHeight;
	pTestButton->SetPosition(D3DXVECTOR3(x, y - 10.0f, 0));

	m_pUIRoot = pTestButton;
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
	if (m_pPopUpSystemMenuUI){
		if (m_pPopUpSystemMenuUI->GetisPopped() == false){
			m_pPopUpSystemMenuUI->SetisPopped(true);
		}
		else {
			m_pPopUpSystemMenuUI->SetisPopped(false);
		}
	}
}