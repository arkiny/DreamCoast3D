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
	pUIImageView->SetTextureFilename(std::string("../Resources/UI/UI_Lobby/UI_Canvas.png"));
	pUIImageView->SetScale(D3DXVECTOR3(1.0f, 1.0f, 1.0f));
	pUIImageView->SetPosition(D3DXVECTOR3(0, 0, 0));
	m_pUIRoot = pUIImageView;

	pUIImageView = new cUIImageView(m_pSprite);
	pUIImageView->SetTextureFilename(std::string("../Resources/UI/UI_Lobby/Login_I3C.tga"));
	pUIImageView->SetScale(D3DXVECTOR3(1.0f, 1.0f, 1.0f));
	pUIImageView->SetPosition(D3DXVECTOR3(800.0f, 300.0f, 0.0f));
	m_pUIRoot->AddChild(pUIImageView);
	pUIImageView->Release();



	// 메인 메뉴 0, 터레인 에디터 5, 맵에디터 3, 인게임 1, 저장한 게임 인게임 4, 실험씬 2

	// 시작 버튼
	cUISceneChangeButton* pTestButton = new cUISceneChangeButton(m_pSprite);
	pTestButton->Setup(
		std::string("../Resources/UI/UI_AcceptButton.png"), 
		std::string("../Resources/UI/UI_AcceptButtonOn.png"),
		std::string("../Resources/UI/UI_AcceptButtonClick.png"));
	pTestButton->SetPosition(D3DXVECTOR3(90, 50, 0));
	pTestButton->SetButtonDeligate(this);
	pTestButton->SetTargetScene(5);
	pUIImageView->AddChild(pTestButton);
	pTestButton->Release();
	
	pTestButton = new cUISceneChangeButton(m_pSprite);
	pTestButton->Setup(
		std::string("../Resources/UI/UI_AcceptButton.png"),
		std::string("../Resources/UI/UI_AcceptButtonOn.png"),
		std::string("../Resources/UI/UI_AcceptButtonClick.png"));
	pTestButton->SetPosition(D3DXVECTOR3(90, 100, 0));
	pTestButton->SetButtonDeligate(this);
	pTestButton->SetTargetScene(3);
	pUIImageView->AddChild(pTestButton);
	pTestButton->Release();

	pTestButton = new cUISceneChangeButton(m_pSprite);
	pTestButton->Setup(
		std::string("../Resources/UI/UI_AcceptButton.png"),
		std::string("../Resources/UI/UI_AcceptButtonOn.png"),
		std::string("../Resources/UI/UI_AcceptButtonClick.png"));
	pTestButton->SetPosition(D3DXVECTOR3(90, 150, 0));
	pTestButton->SetButtonDeligate(this);
	pTestButton->SetTargetScene(1);
	pUIImageView->AddChild(pTestButton);
	pTestButton->Release();

	pTestButton = new cUISceneChangeButton(m_pSprite);
	pTestButton->Setup(
		std::string("../Resources/UI/UI_AcceptButton.png"),
		std::string("../Resources/UI/UI_AcceptButtonOn.png"),
		std::string("../Resources/UI/UI_AcceptButtonClick.png"));
	pTestButton->SetPosition(D3DXVECTOR3(90, 200, 0));
	pTestButton->SetButtonDeligate(this);
	pTestButton->SetTargetScene(4);
	pUIImageView->AddChild(pTestButton);
	pTestButton->Release();

	pTestButton = new cUISceneChangeButton(m_pSprite);
	pTestButton->Setup(
		std::string("../Resources/UI/UI_AcceptButton.png"),
		std::string("../Resources/UI/UI_AcceptButtonOn.png"),
		std::string("../Resources/UI/UI_AcceptButtonClick.png"));
	pTestButton->SetPosition(D3DXVECTOR3(90, 250, 0));
	pTestButton->SetButtonDeligate(this);
	pTestButton->SetTargetScene(2);
	pUIImageView->AddChild(pTestButton);
	pTestButton->Release();

	pTestButton = new cUISceneChangeButton(m_pSprite);
	pTestButton->Setup(
		std::string("../Resources/UI/UI_AcceptButton.png"),
		std::string("../Resources/UI/UI_AcceptButtonOn.png"),
		std::string("../Resources/UI/UI_AcceptButtonClick.png"));
	pTestButton->SetPosition(D3DXVECTOR3(90, 300, 0));
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
	std::string sCopyRight = "All Right Reserved DreamCoast3D™ ⓒ Copyright 2015";
	std::string sVersion = "Version Alpha";
	RECT rectCopyRight = { 10, 670, 10, 701 };
	g_pFontManager->GetFont(g_pFontManager->FONT_DEFAULT)
		->DrawText(NULL, sCopyRight.c_str(), -1, &rectCopyRight, DT_LEFT | DT_NOCLIP, 0xFFFFFFFF);

	rectCopyRight = { 10, 5, 16, 6 };
	g_pFontManager->GetFont(g_pFontManager->FONT_DEFAULT)
		->DrawText(NULL, sVersion.c_str(), -1, &rectCopyRight, DT_LEFT | DT_NOCLIP, 0xFF000000);


}

void cUISystemTest::OnClick(cUIImageButton* pSender){
	if (m_pSceneDeligate){
		m_pSceneDeligate->ChangeScene(1);
	}
}