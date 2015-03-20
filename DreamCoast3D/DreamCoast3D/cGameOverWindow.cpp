#include "stdafx.h"
#include "cGameOverWindow.h"
#include "cUIImageView.h"
#include "cUIImageButton.h"
#include "cUITextView.h"

cGameOverWindow::cGameOverWindow()
{
	m_pReturnButton = nullptr;
	m_pExitButton = nullptr;
}


cGameOverWindow::~cGameOverWindow()
{
}

void cGameOverWindow::Setup()
{
	D3DXCreateSprite(g_pD3DDevice, &m_pSprite);
	cUIImageView* pUIImageView = new cUIImageView(m_pSprite);
	pUIImageView->SetTextureFilename(std::string("../Resources/UI/UI_STORE/Store_BackGround.png"));
	pUIImageView->SetScale(D3DXVECTOR3(1.0f, 1.0f, 1.0f));
	pUIImageView->SetPosition(D3DXVECTOR3(0, 0, 0));
	m_pUIRoot = pUIImageView;

	cUIImageButton* pUIImageReturnButton = new cUIImageButton(m_pSprite);
	pUIImageReturnButton->Setup(
		std::string("../Resources/UI/UI_STORE/Store_Master.png"),
		std::string("../Resources/UI/UI_STORE/Store_Master.png"),
		std::string("../Resources/UI/UI_STORE/Store_Master.png"));
	pUIImageReturnButton->SetPosition(D3DXVECTOR3(300, 100, 0.f));
	pUIImageReturnButton->SetScale(D3DXVECTOR3(1.0f, 1.0f, 1.0f));
	pUIImageReturnButton->SetDelegate(this);
	m_pUIRoot->AddChild(pUIImageReturnButton);
	pUIImageReturnButton->Release();

	cUIImageButton* pUIImageExitButton = new cUIImageButton(m_pSprite);
	pUIImageExitButton->Setup(
		std::string("../Resources/UI/UI_STORE/Store_Speak.png"),
		std::string("../Resources/UI/UI_STORE/Store_Speak.png"),
		std::string("../Resources/UI/UI_STORE/Store_Speak.png"));
	pUIImageExitButton->SetPosition(D3DXVECTOR3(500, 100, 0.f));
	pUIImageExitButton->SetScale(D3DXVECTOR3(.6f, .6f, 1.0f));
	pUIImageExitButton->SetDelegate(this);
	m_pUIRoot->AddChild(pUIImageExitButton);
	pUIImageExitButton->Release();
}

void cGameOverWindow::Update(float fDelta)
{
}

void cGameOverWindow::Render()
{
}

void cGameOverWindow::OnClick(cUIImageButton* pSender)
{

}