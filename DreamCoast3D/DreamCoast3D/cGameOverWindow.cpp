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
	pUIImageView->SetTextureFilename(std::string("../Resources/UI/UI_GameOver/Game_Over.jpg"));
	pUIImageView->SetScale(D3DXVECTOR3(1.34f, 1.34f, 1.0f));
	pUIImageView->SetPosition(D3DXVECTOR3(0.f, 0.f, 0.f));
	m_pUIRoot = pUIImageView;
}

void cGameOverWindow::Update(float fDelta)
{
	if (m_bShow)
	{
		if (g_pControlManager->GetInputInfo(VK_RETURN))
		{
			m_pSceneDeligate->ChangeScene(0);
		}

		if (m_pUIRoot)
		{
			m_pUIRoot->Update(fDelta);
		}
	}

}

void cGameOverWindow::Render()
{
	if (m_bShow)
	{
		if (m_pUIRoot)
		{
			m_pUIRoot->Render();
		}
	}

}

void cGameOverWindow::OnClick(cUIImageButton* pSender)
{

}