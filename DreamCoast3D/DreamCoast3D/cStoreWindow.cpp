#include "stdafx.h"
#include "cStoreWindow.h"
#include "cUIImageView.h"
#include "cUIImageButton.h"
#include "cUITextView.h"

cStoreWindow::cStoreWindow()
{
	m_pHealthButton = nullptr;
	m_pManaButton = nullptr;
}


cStoreWindow::~cStoreWindow()
{
	SAFE_RELEASE(m_pSprite);
	if (m_pUIRoot)
		m_pUIRoot->Destroy();
}

void cStoreWindow::Setup(){
	// 배경이 될 그림

	D3DXCreateSprite(g_pD3DDevice, &m_pSprite);
	cUIImageView* pUIImageView = new cUIImageView(m_pSprite);
	pUIImageView->SetTextureFilename(std::string("../Resources/UI/UI_STORE/Store_BackGround.png"));
	pUIImageView->SetScale(D3DXVECTOR3(1.0f, 1.0f, 1.0f));
	pUIImageView->SetPosition(D3DXVECTOR3(0, 0, 0));
	m_pUIRoot = pUIImageView;

	cUIImageButton* pUIImageAcceptButton = new cUIImageButton(m_pSprite);
	pUIImageAcceptButton->Setup(
		std::string("../Resources/UI/UI_BUTTON_LOBBY/TE_1_NORMAL.png"),
		std::string("../Resources/UI/UI_BUTTON_LOBBY/TE_1_ON.png"),
		std::string("../Resources/UI/UI_BUTTON_LOBBY/TE_1_CLICK.png"));
	pUIImageAcceptButton->SetPosition(D3DXVECTOR3(0, 100, 0.f));
	pUIImageAcceptButton->SetScale(D3DXVECTOR3(.6f, .6f, 1.0f));
	pUIImageAcceptButton->SetDelegate(this);
	m_pUIRoot->AddChild(pUIImageAcceptButton);
	pUIImageAcceptButton->Release();

	cUIImageButton* pUIImageCancelButton = new cUIImageButton(m_pSprite);
	pUIImageCancelButton->Setup(
		std::string("../Resources/UI/UI_BUTTON_LOBBY/TE_1_NORMAL.png"),
		std::string("../Resources/UI/UI_BUTTON_LOBBY/TE_1_ON.png"),
		std::string("../Resources/UI/UI_BUTTON_LOBBY/TE_1_CLICK.png"));
	pUIImageCancelButton->SetPosition(D3DXVECTOR3(200, 100, 0.f));
	pUIImageCancelButton->SetScale(D3DXVECTOR3(.6f, .6f, 1.0f));
	pUIImageCancelButton->SetDelegate(this);
	m_pUIRoot->AddChild(pUIImageCancelButton);
	pUIImageCancelButton->Release();

	cUIImageButton* pUIImageHealthButton = new cUIImageButton(m_pSprite);
	pUIImageHealthButton->Setup(
		std::string("../Resources/UI/UI_STORE/Store_HealthPotion_Normal.png"),
		std::string("../Resources/UI/UI_STORE/Store_HealthPotion_On.png"),
		std::string("../Resources/UI/UI_STORE/Store_HealthPotion_Click.png"));
	pUIImageHealthButton->SetPosition(D3DXVECTOR3(0, 0, 0.f));
	pUIImageHealthButton->SetScale(D3DXVECTOR3(1.f, 1.f, 1.f));
	pUIImageHealthButton->SetDelegate(this);
	m_pUIRoot->AddChild(pUIImageHealthButton);
	m_pHealthButton = pUIImageHealthButton;
	m_pHealthButton->SetButtonType(cUIImageButton::eButtonType::E_HEALTH);
	pUIImageHealthButton->Release();

	cUIImageButton* pUIImageManaButton = new cUIImageButton(m_pSprite);
	pUIImageManaButton->Setup(
		std::string("../Resources/UI/UI_STORE/Store_ManaPotion_Normal.png"),
		std::string("../Resources/UI/UI_STORE/Store_ManaPotion_On.png"),
		std::string("../Resources/UI/UI_STORE/Store_ManaPotion_Click.png"));
	pUIImageManaButton->SetPosition(D3DXVECTOR3(30, 0, 0.f));
	pUIImageHealthButton->SetScale(D3DXVECTOR3(1.f, 1.f, 1.f));
	pUIImageManaButton->SetDelegate(this);
	m_pUIRoot->AddChild(pUIImageManaButton);
	m_pManaButton = pUIImageManaButton;
	m_pManaButton->SetButtonType(cUIImageButton::eButtonType::E_MANA);
	pUIImageManaButton->Release();
}

void cStoreWindow::Update(float fDelta){
	bool isPasue = this->m_pEventDeligate->GetStoreState();
	if (isPasue == true)
	{
		if (m_pUIRoot){
			m_pUIRoot->Update(fDelta);
		}
	}

}

void cStoreWindow::Render(){
	bool isPasue = this->m_pEventDeligate->GetStoreState();
	if (isPasue == true)
	{
		if (m_pUIRoot){
			m_pUIRoot->Render();
		}
	}
	

}

void cStoreWindow::OnClick(cUIImageButton* pSender){
	// 버튼 클릭시 행할 것들

	if (pSender->GetButtonType() == cUIImageButton::eButtonType::E_HEALTH)
	{
		int nCount = m_pUIRoot->m_pUIManagerDeligate->GetInventoryControl()->GetInventoryItemCount(0);
		nCount++;
		m_pUIRoot->m_pUIManagerDeligate->GetInventoryControl()->UpdateInventoryItemCount(0, nCount);
	}

	if (pSender->GetButtonType() == cUIImageButton::eButtonType::E_MANA)
	{
		int nCount = m_pUIRoot->m_pUIManagerDeligate->GetInventoryControl()->GetInventoryItemCount(1);
		nCount++;
		m_pUIRoot->m_pUIManagerDeligate->GetInventoryControl()->UpdateInventoryItemCount(1, nCount);
	}

}
