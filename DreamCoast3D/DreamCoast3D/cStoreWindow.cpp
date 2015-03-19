#include "stdafx.h"
#include "cStoreWindow.h"
#include "cUIImageView.h"
#include "cUIImageButton.h"
#include "cUITextView.h"

cStoreWindow::cStoreWindow()
{
	m_pHealthButton = nullptr;
	m_pManaButton = nullptr;
	m_nScore = 0;
}


cStoreWindow::~cStoreWindow()
{

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
		std::string("../Resources/UI/UI_STORE/Store_Master.png"),
		std::string("../Resources/UI/UI_STORE/Store_Master.png"),
		std::string("../Resources/UI/UI_STORE/Store_Master.png"));
	pUIImageAcceptButton->SetPosition(D3DXVECTOR3(300, 100, 0.f));
	pUIImageAcceptButton->SetScale(D3DXVECTOR3(1.0f, 1.0f, 1.0f));
	pUIImageAcceptButton->SetDelegate(this);
	m_pUIRoot->AddChild(pUIImageAcceptButton);
	pUIImageAcceptButton->Release();

	cUIImageButton* pUIImageCancelButton = new cUIImageButton(m_pSprite);
	pUIImageCancelButton->Setup(
		std::string("../Resources/UI/UI_STORE/Store_Speak.png"),
		std::string("../Resources/UI/UI_STORE/Store_Speak.png"),
		std::string("../Resources/UI/UI_STORE/Store_Speak.png"));
	pUIImageCancelButton->SetPosition(D3DXVECTOR3(500, 100, 0.f));
	pUIImageCancelButton->SetScale(D3DXVECTOR3(.6f, .6f, 1.0f));
	pUIImageCancelButton->SetDelegate(this);
	m_pUIRoot->AddChild(pUIImageCancelButton);
	pUIImageCancelButton->Release();

	cUIImageButton* pUIImageHealthButton = new cUIImageButton(m_pSprite);
	pUIImageHealthButton->Setup(
        std::string("../Resources/ICON/ICON_Item/HP_Potion_Level_33_Tex.tga"),
        std::string("../Resources/ICON/ICON_Item/HP_Potion_Level_33_Tex.tga"),
        std::string("../Resources/ICON/ICON_Item/HP_Potion_Level_33_Tex.tga"));
	pUIImageHealthButton->SetPosition(D3DXVECTOR3(850, 140, 0.f));
	pUIImageHealthButton->SetScale(D3DXVECTOR3(1.f, 1.f, 1.f));
	pUIImageHealthButton->SetDelegate(this);
	m_pUIRoot->AddChild(pUIImageHealthButton);
	m_pHealthButton = pUIImageHealthButton;
	m_pHealthButton->SetButtonType(cUIImageButton::eButtonType::E_HEALTH);
	pUIImageHealthButton->Release();

	cUIImageButton* pUIImageManaButton = new cUIImageButton(m_pSprite);
	pUIImageManaButton->Setup(
		std::string("../Resources/ICON/ICON_Item/MP_Potion_Level_33_Tex.tga"),
		std::string("../Resources/ICON/ICON_Item/MP_Potion_Level_33_Tex.tga"),
		std::string("../Resources/ICON/ICON_Item/MP_Potion_Level_33_Tex.tga"));
	pUIImageManaButton->SetPosition(D3DXVECTOR3(850, 210, 0.f));
	pUIImageHealthButton->SetScale(D3DXVECTOR3(1.f, 1.f, 1.f));
	pUIImageManaButton->SetDelegate(this);
	m_pUIRoot->AddChild(pUIImageManaButton);
	m_pManaButton = pUIImageManaButton;
	m_pManaButton->SetButtonType(cUIImageButton::eButtonType::E_MANA);
	pUIImageManaButton->Release();

}

void cStoreWindow::Update(float fDelta){
	//bool isPasue = this->m_pEventDeligate->GetStoreState();
	if (m_bShow == true)
	{
		this->m_pEventDeligate->SetStoreState(true);

		m_nScore = m_pGameObjDelgate->GetScore();

		if (m_pUIRoot){
			m_pUIRoot->Update(fDelta);
		}
	}
	else
	{
		this->m_pEventDeligate->SetStoreState(false);
	}

	
}

void cStoreWindow::Render(){

	if (m_bShow == true)
	{
		if (m_pUIRoot){
			m_pUIRoot->Render();
		}
		GoldRender();
	}

}

void cStoreWindow::OnClick(cUIImageButton* pSender){
	// 버튼 클릭시 행할 것들

	if (pSender->GetButtonType() == cUIImageButton::eButtonType::E_HEALTH)
	{
		if (m_nScore >= 50)
		{
			int nCount = m_pUIRoot->m_pUIManagerDeligate->GetInventoryControl()->GetInventoryItemCount(0);
			nCount++;
			m_pUIRoot->m_pUIManagerDeligate->GetInventoryControl()->UpdateInventoryItemCount(0, nCount);
			m_nScore -= 50;
			m_pGameObjDelgate->SetScore(m_nScore);
		}
		else
		{
			return;
		}
		
	}

	if (pSender->GetButtonType() == cUIImageButton::eButtonType::E_MANA)
	{
		if (m_nScore >= 50)
		{
			int nCount = m_pUIRoot->m_pUIManagerDeligate->GetInventoryControl()->GetInventoryItemCount(1);
			nCount++;
			m_pUIRoot->m_pUIManagerDeligate->GetInventoryControl()->UpdateInventoryItemCount(1, nCount);
			m_nScore -= 50;
			m_pGameObjDelgate->SetScore(m_nScore);
		}
		else
		{
			return;
		}
	}

}

void cStoreWindow::GoldRender()
{
	RECT rt;
	rt.left = 800;
	rt.top = 300;
	rt.right = 801;
	rt.bottom = 301;

	std::stringstream ss;
	ss << "Score : " << m_nScore << std::endl;

	g_pFontManager->GetFont(g_pFontManager->FONT_DEFAULT)->DrawText(NULL,
		ss.str().c_str(),	 //pString
		-1,					//Count
		&rt,	//pRect
		DT_LEFT | DT_NOCLIP,//Format,
		0xFFFFFFFF);		//Color
}