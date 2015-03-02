#include "stdafx.h"
#include "cUISystemTest.h"
#include "cUISceneChangeButton.h"


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
	cUISceneChangeButton* pTestButton = new cUISceneChangeButton(m_pSprite);
	pTestButton->Setup(
		std::string("../Resources/UI/UI_AcceptButton.png"), 
		std::string("../Resources/UI/UI_AcceptButtonOn.png"),
		std::string("../Resources/UI/UI_AcceptButtonClick.png"));
	pTestButton->SetPosition(D3DXVECTOR3(0, 100, 0));
	pTestButton->SetButtonDeligate(this);
	pTestButton->SetTargetScene(1);
	m_pUIRoot = pTestButton;
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