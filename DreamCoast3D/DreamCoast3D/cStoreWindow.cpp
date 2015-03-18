#include "stdafx.h"
#include "cStoreWindow.h"
#include "cUIImageView.h"
#include "cUIImageButton.h"
#include "cUITextView.h"

cStoreWindow::cStoreWindow()
{
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
	pUIImageView->SetTextureFilename(std::string("../Resources/UI/UI_Lobby/Store_BackGround.png"));
	pUIImageView->SetScale(D3DXVECTOR3(1.0f, 1.0f, 1.0f));
	pUIImageView->SetPosition(D3DXVECTOR3(0, 0, 0));
	m_pUIRoot = pUIImageView;

	cUIImageButton* pUIImageButton = new cUIImageButton(m_pSprite);
	pUIImageButton->Setup(
		std::string("../Resources/UI/UI_BUTTON_LOBBY/TE_1_NORMAL.png"),
		std::string("../Resources/UI/UI_BUTTON_LOBBY/TE_1_ON.png"),
		std::string("../Resources/UI/UI_BUTTON_LOBBY/TE_1_CLICK.png"));
	pUIImageButton->SetPosition(D3DXVECTOR3(75, 40, 0));
	pUIImageButton->SetScale(D3DXVECTOR3(.6f, .6f, 1.0f));
	pUIImageButton->SetDelegate(this);
	m_pUIRoot->AddChild(pUIImageButton);
	pUIImageButton->Release();
}

void cStoreWindow::Update(float fDelta){
	if (m_pUIRoot){
		m_pUIRoot->Update(fDelta);
	}
}

void cStoreWindow::Render(){
	if (m_pUIRoot){
		m_pUIRoot->Render();
	}
}

void cStoreWindow::OnClick(cUIImageButton* pSender){
	// 버튼 클릭시 행할 것들
}
