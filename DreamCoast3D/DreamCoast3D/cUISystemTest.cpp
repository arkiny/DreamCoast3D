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
		std::string("../Resources/UI/UI_BUTTON_LOBBY/TE_1_NORMAL.png"), 
		std::string("../Resources/UI/UI_BUTTON_LOBBY/TE_1_ON.png"),
		std::string("../Resources/UI/UI_BUTTON_LOBBY/TE_1_CLICK.png"));
	pTestButton->SetPosition(D3DXVECTOR3(75, 40, 0));
	pTestButton->SetScale(D3DXVECTOR3(.6f, .6f, 1.0f));
	pTestButton->SetButtonDeligate(this);
	pTestButton->SetTargetScene(5);
	pUIImageView->AddChild(pTestButton);
	pTestButton->Release();
	
	pTestButton = new cUISceneChangeButton(m_pSprite);
	pTestButton->Setup(
		std::string("../Resources/UI/UI_BUTTON_LOBBY/ME_1_NORMAL.png"),
		std::string("../Resources/UI/UI_BUTTON_LOBBY/ME_1_ON.png"),
		std::string("../Resources/UI/UI_BUTTON_LOBBY/ME_1_CLICK.png"));
	pTestButton->SetPosition(D3DXVECTOR3(75, 90, 0));
	pTestButton->SetScale(D3DXVECTOR3(.6f, .6f, 1.0f));
	pTestButton->SetButtonDeligate(this);
	pTestButton->SetTargetScene(3);
	pUIImageView->AddChild(pTestButton);
	pTestButton->Release();

	pTestButton = new cUISceneChangeButton(m_pSprite);
	pTestButton->Setup(
		std::string("../Resources/UI/UI_BUTTON_LOBBY/PP_1_NORMAL.png"),
		std::string("../Resources/UI/UI_BUTTON_LOBBY/PP_1_ON.png"),
		std::string("../Resources/UI/UI_BUTTON_LOBBY/PP_1_CLICK.png"));
	pTestButton->SetPosition(D3DXVECTOR3(75, 140, 0));
	pTestButton->SetScale(D3DXVECTOR3(.6f, .6f, 1.0f));
	pTestButton->SetButtonDeligate(this);
	pTestButton->SetTargetScene(1);
	pUIImageView->AddChild(pTestButton);
	pTestButton->Release();

	pTestButton = new cUISceneChangeButton(m_pSprite);
	pTestButton->Setup(
		std::string("../Resources/UI/UI_BUTTON_LOBBY/PY_1_NORMAL.png"),
		std::string("../Resources/UI/UI_BUTTON_LOBBY/PY_1_ON.png"),
		std::string("../Resources/UI/UI_BUTTON_LOBBY/PY_1_CLICK.png"));
	pTestButton->SetPosition(D3DXVECTOR3(75, 190, 0));
	pTestButton->SetScale(D3DXVECTOR3(.6f, .6f, 1.0f));
	pTestButton->SetButtonDeligate(this);
	pTestButton->SetTargetScene(4);
	pUIImageView->AddChild(pTestButton);
	pTestButton->Release();

	pTestButton = new cUISceneChangeButton(m_pSprite);
	pTestButton->Setup(
		std::string("../Resources/UI/UI_BUTTON_LOBBY/CR_1_NORMAL.png"),
		std::string("../Resources/UI/UI_BUTTON_LOBBY/CR_1_ON.png"),
		std::string("../Resources/UI/UI_BUTTON_LOBBY/CR_1_CLICK.png"));
	pTestButton->SetPosition(D3DXVECTOR3(75, 240, 0));
	pTestButton->SetScale(D3DXVECTOR3(.6f, .6f, 1.0f));
	pTestButton->SetButtonDeligate(this);
	pTestButton->SetTargetScene(2);
	pUIImageView->AddChild(pTestButton);
	pTestButton->Release();

	pTestButton = new cUISceneChangeButton(m_pSprite);
	pTestButton->Setup(
		std::string("../Resources/UI/UI_BUTTON_LOBBY/QG_1_NORMAL.png"),
		std::string("../Resources/UI/UI_BUTTON_LOBBY/QG_1_ON.png"),
		std::string("../Resources/UI/UI_BUTTON_LOBBY/QG_1_CLICK.png"));
	pTestButton->SetPosition(D3DXVECTOR3(75, 290, 0));
	pTestButton->SetScale(D3DXVECTOR3(.6f, .6f, 1.0f));
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