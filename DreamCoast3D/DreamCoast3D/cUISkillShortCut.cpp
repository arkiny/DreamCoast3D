#include "stdafx.h"
#include "cUISkillShortCut.h"
#include "cUIImageView.h"
#include "cUIImageButton.h"
#include "cUITextView.h"

cUISkillShortCut::cUISkillShortCut()
{
}


cUISkillShortCut::~cUISkillShortCut()
{
	SAFE_RELEASE(m_pSprite);
	if (m_pUIRoot)
		m_pUIRoot->Destroy();
}

void cUISkillShortCut::Setup(){
	RECT clientRect;
	GetClientRect(g_hWnd, &clientRect);
	
	D3DXCreateSprite(g_pD3DDevice, &m_pSprite);
	cUIImageView* pImageView = new cUIImageView(m_pSprite);
	pImageView->SetTextureFilename(std::string("../Resources/UI/UI_ShortCut/pageBg.tga"));
	pImageView->SetScale(D3DXVECTOR3(1.0f, 1.0f, 1.0f));
	
	RECT drawArea = { 0, 0, 77, 112 };
	pImageView->SetPosition(D3DXVECTOR3(60.0f, clientRect.bottom - drawArea.bottom - 30.f, 0));
	
	pImageView->SetDrawArea(drawArea);
	m_pUIRoot = pImageView;

	//slotFrame
	for (int i = 0; i < 2; i++){
		pImageView = new cUIImageView(m_pSprite);
		pImageView->SetTextureFilename(std::string("../Resources/UI/UI_ShortCut/slotFrame.tga"));
		pImageView->SetScale(D3DXVECTOR3(.9f, .9f, 1.0f));
		pImageView->SetPosition(D3DXVECTOR3(i * pImageView->GetSize().fWidth*.9f + 30.0f, 4.0f, 0));
		m_pUIRoot->AddChild(pImageView);

		m_vecSlots.push_back(pImageView);
		
		SAFE_RELEASE(pImageView);
	}

	for (int i = 0; i < 2; i++){
		pImageView = new cUIImageView(m_pSprite);
		pImageView->SetTextureFilename(std::string("../Resources/UI/UI_ShortCut/slotFrame.tga"));
		pImageView->SetScale(D3DXVECTOR3(.9f, .9f, 1.0f));
		pImageView->SetPosition(
			D3DXVECTOR3(i * pImageView->GetSize().fWidth*.9f + 30.0f,
			pImageView->GetSize().fHeight*.9f, 0));
		m_pUIRoot->AddChild(pImageView);

		m_vecSlots.push_back(pImageView);

		SAFE_RELEASE(pImageView);
	}
}

void cUISkillShortCut::Update(float fDelta){
	if (m_pUIRoot){
		m_pUIRoot->Update(fDelta);
	}
	
	if (g_pControlManager->GetInputInfo('Z') == true || g_pControlManager->GetInputInfo('z') == true){
		if (m_pGameObjDelgate->GetPlayerStatInfo()->fMaxHp > m_pGameObjDelgate->GetPlayerStatInfo()->fCurrentHp){
			m_pGameObjDelgate->GetPlayerStatInfo()->fCurrentHp += 10;
			// effect Ãß°¡
		}
	}

	if (g_pControlManager->GetInputInfo('X') == true || g_pControlManager->GetInputInfo('x') == true){

	}

	if (g_pControlManager->GetInputInfo('c')){

	}

	if (g_pControlManager->GetInputInfo('v')){

	}
}

void cUISkillShortCut::Render(){
	if (m_pSprite)
	{
		m_pUIRoot->Render();
	}
}

void cUISkillShortCut::AddShortCutIntoBox(cUISkillIcon* pSkill){

}