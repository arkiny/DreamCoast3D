#include "stdafx.h"
#include "cUISet4Editor.h"
#include "cUIImageView.h"
#include "cUISceneChangeButton.h"

cUISet4Editor::cUISet4Editor()
{
}


cUISet4Editor::~cUISet4Editor()
{
	SAFE_RELEASE(m_pSprite);
	if (m_pUIRoot)
		m_pUIRoot->Destroy();
}

void cUISet4Editor::Setup(){
	D3DXCreateSprite(g_pD3DDevice, &m_pSprite);
	cUIImageView* pUIImageView = new cUIImageView(m_pSprite);
	pUIImageView->SetTextureFilename(std::string("../Resources/UI/UI_EDITOR/UI_EDITOR_Canvas.png"));
	pUIImageView->SetScale(D3DXVECTOR3(1.0f, 1.0f, 1.0f));
	pUIImageView->SetPosition(D3DXVECTOR3(0, 0, 0));
	m_pUIRoot = pUIImageView;

	// 메인 메뉴 0, 터레인 에디터 5, 맵에디터 3, 인게임 1, 저장한 게임 인게임 4, 실험씬 2
	// 시작 버튼
	// TODO 차후에 QuitToMainMenu로 바꿔야함
	cUISceneChangeButton* pTestButton = new cUISceneChangeButton(m_pSprite);
	pTestButton->Setup(
		std::string("../Resources/UI/UI_BUTTON_LOBBY/QG_1_NORMAL.png"),
		std::string("../Resources/UI/UI_BUTTON_LOBBY/QG_1_ON.png"),
		std::string("../Resources/UI/UI_BUTTON_LOBBY/QG_1_CLICK.png"));
	pTestButton->SetPosition(D3DXVECTOR3(1120, 640, 0));
	pTestButton->SetScale(D3DXVECTOR3(.6f, .6f, 1.0f));
	pTestButton->SetButtonDeligate(this);
	pTestButton->SetTargetScene(0);
	pUIImageView->AddChild(pTestButton);
	pTestButton->Release();
}

void cUISet4Editor::Start(){
	if (m_pUIRoot){
		m_pUIRoot->SetSceneDeligate(m_pSceneDeligate);
	}
}

void cUISet4Editor::Update(float fDelta){
	if (m_pUIRoot){
		m_pUIRoot->Update(fDelta);
	}
}

void cUISet4Editor::Render(){
	if (m_pSprite)
	{
		m_pUIRoot->Render();
	}
}

void cUISet4Editor::OnClick(cUIImageButton* pSender){
	if (m_pSceneDeligate){
		m_pSceneDeligate->ChangeScene(1);
	}
}