#include "stdafx.h"
#include <sstream>
#include "cUISkillShortCut.h"
#include "cUIImageView.h"
#include "cUIImageButton.h"
#include "cUITextView.h"
#include "cItemConsume.h"

cUISkillShortCut::cUISkillShortCut()
{
}


cUISkillShortCut::~cUISkillShortCut()
{
	SAFE_RELEASE(m_pSprite);

	for (auto p : m_vecItems){
		SAFE_RELEASE(p);
	}

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

	/// test
	cItemConsume* p = new cItemConsume;
	m_vecItems.push_back(p);
	m_vecDelayCoolTime.push_back(p->GetItemCoolTime());
	pImageView = new cUIImageView(m_pSprite);
	pImageView->SetTextureFilename(p->GetTexturePath());
	pImageView->SetScale(D3DXVECTOR3(.72f, .72f, 1.0f));
	pImageView->SetPosition(D3DXVECTOR3(4.8f, 4.8f, 0));
	m_vecSlots[0]->AddChild(pImageView);
	SAFE_RELEASE(pImageView);
	cUITextView* pTextView = new cUITextView(m_pSprite);

	std::stringstream ss;
	ss << p->GetCount();
	pTextView->SetText(ss.str());
	ss.str("");
	pTextView->SetDrawTextFormat(DT_RIGHT | DT_TOP | DT_WORDBREAK);
	pTextView->SetTextColor(D3DCOLOR_XRGB(255, 255, 255));
	pTextView->SetPosition(10, 30);
	ST_SIZE stSize = m_vecSlots[0]->GetSize();
	stSize.fWidth -= 25;
	pTextView->SetSize(stSize);
	m_vecSlots[0]->AddChild(pTextView);
	m_vecItemNums.push_back(pTextView);
	SAFE_RELEASE(pTextView);

	///
	p = new cItemConsume;
	p->SetConsumeType(p->E_CONSUME_MP_UP);
	m_vecItems.push_back(p);
	m_vecDelayCoolTime.push_back(p->GetItemCoolTime());
	pImageView = new cUIImageView(m_pSprite);
	pImageView->SetTextureFilename(p->GetTexturePath());
	pImageView->SetScale(D3DXVECTOR3(.72f, .72f, 1.0f));
	pImageView->SetPosition(D3DXVECTOR3(4.8f, 4.8f, 0));
	m_vecSlots[1]->AddChild(pImageView);
	SAFE_RELEASE(pImageView);
	pTextView = new cUITextView(m_pSprite);
	ss << p->GetCount();
	pTextView->SetText(ss.str());
	ss.str("");
	pTextView->SetDrawTextFormat(DT_RIGHT | DT_TOP | DT_WORDBREAK);
	pTextView->SetTextColor(D3DCOLOR_XRGB(255, 255, 255));
	pTextView->SetPosition(10, 30);
	stSize = m_vecSlots[1]->GetSize();
	stSize.fWidth -= 25;
	pTextView->SetSize(stSize);
	m_vecSlots[1]->AddChild(pTextView);
	m_vecItemNums.push_back(pTextView);
	SAFE_RELEASE(pTextView);


	///
	m_vecItems.push_back(p);
	p->AddRef();
	m_vecItems.push_back(p);
	p->AddRef();

	m_vecDelayCoolTime.push_back(p->GetItemCoolTime());
	m_vecDelayCoolTime.push_back(p->GetItemCoolTime());

}

void cUISkillShortCut::Update(float fDelta){
	for (size_t i = 0; i < m_vecDelayCoolTime.size(); i++){
		if (m_vecDelayCoolTime[i] >= m_vecItems[i]->GetCoolTime()){
			m_vecDelayCoolTime[i] = m_vecItems[i]->GetCoolTime();
		}
		else{
			m_vecDelayCoolTime[i] += fDelta;
		}
	}

	if (m_pUIRoot){
		m_pUIRoot->Update(fDelta);
		
		if (m_vecItems[0]->GetCount() == 0){
			m_vecSlots[0]->SetisShowingChildren(false);
		}
		else {
			m_vecSlots[0]->SetisShowingChildren(true);
		}
		if (m_vecItems[1]->GetCount() == 0){
			m_vecSlots[1]->SetisShowingChildren(false);
		}
		else {
			m_vecSlots[1]->SetisShowingChildren(true);
		}

		std::stringstream ss;
		for (size_t i = 0; i < 2/*m_vecItems.size()*/; i++){
			ss << m_vecItems[i]->GetCount();
			m_vecItemNums[i]->SetText(ss.str());
			ss.str("");
		}		
	}
	
	if (g_pControlManager->GetInputInfo('Z') == true || g_pControlManager->GetInputInfo('z') == true){
		// ¾ÆÀÌÅÛ z½½·Ô ½ÇÇà
		if (m_vecDelayCoolTime[0] >= m_vecItems[0]->GetCoolTime()){
			m_vecItems[0]->Activate(m_pGameObjDelgate, m_pEffectManagerDelegate, NULL, NULL, NULL);
			m_vecDelayCoolTime[0] = 0.0f;
		}
	}

	if (g_pControlManager->GetInputInfo('X') == true || g_pControlManager->GetInputInfo('x') == true){
		// ¾ÆÀÌÅÛ x½½·Ô ½ÇÇà
		if (m_vecDelayCoolTime[1] >= m_vecItems[1]->GetCoolTime()){
			m_vecItems[1]->Activate(m_pGameObjDelgate, m_pEffectManagerDelegate, NULL, NULL, NULL);
			m_vecDelayCoolTime[1] = 0.0f;
		}
	}
	
	if (g_pControlManager->GetInputInfo('c')){
		// ½ºÅ³ c½½·Ô ½ÇÇà
	}

	if (g_pControlManager->GetInputInfo('v')){
		// ½½·Ô x½½·Ô ½ÇÇà
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