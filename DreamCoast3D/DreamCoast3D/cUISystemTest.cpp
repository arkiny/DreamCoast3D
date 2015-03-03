#include "stdafx.h"
#include "cUISystemTest.h"
#include "cUISceneChangeButton.h"
#include "cUIImageView.h"
#include "cUITextView.h"

cUISystemTest::cUISystemTest()
{
}


cUISystemTest::~cUISystemTest()
{
	SAFE_RELEASE(m_pSprite);
	if (m_pUIRoot)
		m_pUIRoot->Destroy();
}

void cUISystemTest::Setup(){
	D3DXCreateSprite(g_pD3DDevice, &m_pSprite);
	cUIImageView* pUIImageView = new cUIImageView(m_pSprite);
	pUIImageView->SetTextureFilename(std::string("../Resources/UI/UI_Lobby/tempMain.png"));
	pUIImageView->SetScale(D3DXVECTOR3(1.0f, 1.0f, 1.0f));
	pUIImageView->SetPosition(D3DXVECTOR3(0, 0, 0));
	m_pUIRoot = pUIImageView;

	pUIImageView = new cUIImageView(m_pSprite);
	pUIImageView->SetTextureFilename(std::string("../Resources/UI/UI_Lobby/Login_I3C.tga"));
	pUIImageView->SetScale(D3DXVECTOR3(1.0f, 1.0f, 1.0f));
	pUIImageView->SetPosition(D3DXVECTOR3(500.0f, 200.0f, 0.0f));
	m_pUIRoot->AddChild(pUIImageView);
	pUIImageView->Release();

	// 시작 버튼
	cUISceneChangeButton* pTestButton = new cUISceneChangeButton(m_pSprite);
	pTestButton->Setup(
		std::string("../Resources/UI/UI_AcceptButton.png"), 
		std::string("../Resources/UI/UI_AcceptButtonOn.png"),
		std::string("../Resources/UI/UI_AcceptButtonClick.png"));
	pTestButton->SetPosition(D3DXVECTOR3(90, 100, 0));
	pTestButton->SetButtonDeligate(this);
	pTestButton->SetTargetScene(1);
	pUIImageView->AddChild(pTestButton);
	pTestButton->Release();


	pTestButton = new cUISceneChangeButton(m_pSprite);
	pTestButton->Setup(
		std::string("../Resources/UI/UI_AcceptButton.png"),
		std::string("../Resources/UI/UI_AcceptButtonOn.png"),
		std::string("../Resources/UI/UI_AcceptButtonClick.png"));
	pTestButton->SetPosition(D3DXVECTOR3(90, 150, 0));
	pTestButton->SetButtonDeligate(this);
	pTestButton->SetTargetScene(-2);
	pUIImageView->AddChild(pTestButton);
	pTestButton->Release();
}

void cUISystemTest::Start(){
	if (m_pUIRoot){
		m_pUIRoot->SetSceneDeligate(m_pSceneDeligate);
	}
}

void cUISystemTest::Update(float fDelta){
	if (m_pUIRoot){
		m_pUIRoot->Update(fDelta);
	}
}

void cUISystemTest::Render(){
	if (m_pSprite)
	{
		m_pUIRoot->Render();
	}
}

void cUISystemTest::OnClick(cUIImageButton* pSender){
	if (m_pSceneDeligate){
		m_pSceneDeligate->ChangeScene(1);
	}
}