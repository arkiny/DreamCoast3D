#include "stdafx.h"
#include "cUICredit.h"
#include "cUIImageView.h"
#include "cUISceneChangeButton.h"

cUICredit::cUICredit()
{
}


cUICredit::~cUICredit()
{
	if (m_pUIRoot){
		m_pUIRoot->Destroy();
	}
	SAFE_RELEASE(m_pSprite);
}

void cUICredit::Setup(){
	D3DXCreateSprite(g_pD3DDevice, &m_pSprite);
	cUIImageView* pUIImageView = new cUIImageView(m_pSprite);
	pUIImageView->SetTextureFilename(std::string("../Resources/UI/UI_CREDIT/UI_CREDIT_CANVAS.png"));
	pUIImageView->SetScale(D3DXVECTOR3(1.0f, 1.0f, 1.0f));
	pUIImageView->SetPosition(D3DXVECTOR3(0, 0, 0));
	m_pUIRoot = pUIImageView;

	// ���� �޴� 0, �ͷ��� ������ 5, �ʿ����� 3, �ΰ��� 1, ������ ���� �ΰ��� 4, ����� 2
	// ���� ��ư
	// TODO ���Ŀ� QuitToMainMenu�� �ٲ����
	cUISceneChangeButton* pTestButton = new cUISceneChangeButton(m_pSprite);
	pTestButton->Setup(
		std::string("../Resources/UI/UI_BUTTON_LOBBY/MM_1_NORMAL.png"),
		std::string("../Resources/UI/UI_BUTTON_LOBBY/MM_1_ON.png"),
		std::string("../Resources/UI/UI_BUTTON_LOBBY/MM_1_CLICK.png"));
	pTestButton->SetPosition(D3DXVECTOR3(1120, 640, 0));
	pTestButton->SetScale(D3DXVECTOR3(.6f, .6f, 1.0f));
	pTestButton->SetButtonDeligate(this);
	pTestButton->SetTargetScene(0);
	m_pUIRoot->AddChild(pTestButton);
	SAFE_RELEASE(pTestButton);
}

void cUICredit::Update(float fDelta){
	if (m_pUIRoot){
		m_pUIRoot->Update(fDelta);
	}
}

void cUICredit::Render(){
	if (m_pUIRoot){
		m_pUIRoot->Render();
	}
}

void cUICredit::SetSceneDeligate(iSceneDelegate* pSceneDeligate){
	if (m_pUIRoot){
		m_pUIRoot->SetSceneDeligate(pSceneDeligate);
	}
}