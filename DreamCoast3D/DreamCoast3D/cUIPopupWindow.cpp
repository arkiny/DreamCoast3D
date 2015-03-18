#include "stdafx.h"
#include "cUIPopupWindow.h"
#include "cUIImageView.h"
#include "cUIImageButton.h"
#include "cUISceneChangeButton.h"


cUIPopupWindow::cUIPopupWindow()
	:m_bShow(false)
{
}


cUIPopupWindow::~cUIPopupWindow()
{
	SAFE_RELEASE(m_pSprite);
	if (m_pUIRoot)
		m_pUIRoot->Destroy();
}

void cUIPopupWindow::Setup(){
	D3DXCreateSprite(g_pD3DDevice, &m_pSprite);
	cUIImageView* pUIImageView = new cUIImageView(m_pSprite);
	pUIImageView->SetTextureFilename(std::string("../Resources/UI/UI_Lobby/Login_I3C.tga"));
	pUIImageView->SetScale(D3DXVECTOR3(1.0f, 1.0f, 1.0f));
	pUIImageView->SetPosition(D3DXVECTOR3(500.0f, 200.0f, 0.0f));
	m_pUIRoot = pUIImageView;

	/// 시작 버튼
	cUISceneChangeButton* pTestButton = new cUISceneChangeButton(m_pSprite);
	pTestButton->Setup(
		std::string("../Resources/UI/UI_BUTTON_LOBBY/M_MM_1_NORMAL.png"),
		std::string("../Resources/UI/UI_BUTTON_LOBBY/M_MM_1_ON.png"),
		std::string("../Resources/UI/UI_BUTTON_LOBBY/M_MM_1_CLICK.png"));
	pTestButton->SetPosition(D3DXVECTOR3(90, 100, 0));
	pTestButton->SetButtonDeligate(this);
	pTestButton->SetTargetScene(0);
	pUIImageView->AddChild(pTestButton);
	pTestButton->Release();

	pTestButton = new cUISceneChangeButton(m_pSprite);
	pTestButton->Setup(
		std::string("../Resources/UI/UI_BUTTON_LOBBY/M_QG_1_NORMAL.png"),
		std::string("../Resources/UI/UI_BUTTON_LOBBY/M_QG_1_ON.png"),
		std::string("../Resources/UI/UI_BUTTON_LOBBY/M_QG_1_CLICK.png"));
	pTestButton->SetPosition(D3DXVECTOR3(90, 150, 0));
	pTestButton->SetButtonDeligate(this);
	pTestButton->SetTargetScene(-2);
	pUIImageView->AddChild(pTestButton);
	pTestButton->Release();
}

void cUIPopupWindow::Start(){

}

void cUIPopupWindow::Update(float fDelta){
	if (m_pUIRoot && m_bShow){
		m_pUIRoot->Update(fDelta);
	}
}

void cUIPopupWindow::Render(){
	if (m_pUIRoot && m_bShow){
		m_pUIRoot->Render();
	}
}

void cUIPopupWindow::OnClick(cUIImageButton* pSender){
	if (m_pUIManagerDeligate){
		m_pUIManagerDeligate->ChangeScene(0, this);
	}
}