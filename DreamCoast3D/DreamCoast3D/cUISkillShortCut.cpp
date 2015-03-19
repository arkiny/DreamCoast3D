#include "stdafx.h"
#include <sstream>
#include "cUISkillShortCut.h"
#include "cUIImageView.h"
#include "cUIImageButton.h"
#include "cUITextView.h"
#include "cItemConsume.h"
#include "cSkillIcons.h"

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

	/// Items
	cItemConsume* p = new cItemConsume;
	m_vecItems.push_back(p);
	m_vecDelayCoolTime.push_back(p->GetItemCoolTime());
	
	pImageView = new cUIImageView(m_pSprite);
	pImageView->SetTextureFilename(p->GetTexturePath());
	pImageView->SetScale(D3DXVECTOR3(.72f, .72f, 1.0f));
	pImageView->SetPosition(D3DXVECTOR3(4.8f, 4.8f, 0));
	m_vecSlots[0]->AddChild(pImageView);
	
	
	cUIImageView* pShadow = NULL;

	///
	pShadow = new cUIImageView(m_pSprite);
	pShadow->SetTextureFilename(std::string("../Resources/UI/UI_ShortCut/cooltimeshadow2.png"));
	pShadow->SetScale(D3DXVECTOR3(0.0f, 1.0f, 1.0f));
	pShadow->SetPosition(D3DXVECTOR3(0, 0, 0));
	pImageView->AddChild(pShadow);
	m_vecShadows.push_back(pShadow);
	///
	
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
	
	SAFE_RELEASE(pImageView);
	SAFE_RELEASE(pTextView);
	SAFE_RELEASE(pShadow);

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
	
	///
	pShadow = new cUIImageView(m_pSprite);
	pShadow->SetTextureFilename(std::string("../Resources/UI/UI_ShortCut/cooltimeshadow2.png"));
	pShadow->SetScale(D3DXVECTOR3(0.0f, 1.0f, 1.0f));
	pShadow->SetPosition(D3DXVECTOR3(0, 0, 0));
	pImageView->AddChild(pShadow);
	m_vecShadows.push_back(pShadow);
	///




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

	SAFE_RELEASE(pImageView);
	SAFE_RELEASE(pTextView);
	SAFE_RELEASE(pShadow);


	cSkillIcons* p2 = new cSkillIcons;
	m_vecItems.push_back(p2);
	m_vecDelayCoolTime.push_back(p2->GetCoolTime());
	
	pImageView = new cUIImageView(m_pSprite);
	pImageView->SetTextureFilename(p2->GetTexturePath());
	pImageView->SetScale(D3DXVECTOR3(.72f, .72f, 1.0f));
	pImageView->SetPosition(D3DXVECTOR3(4.8f, 4.8f, 0));
	m_vecSlots[2]->AddChild(pImageView);
	
	///
	pShadow = new cUIImageView(m_pSprite);
	pShadow->SetTextureFilename(std::string("../Resources/UI/UI_ShortCut/cooltimeshadow2.png"));
	pShadow->SetScale(D3DXVECTOR3(0.0f, 1.0f, 1.0f));
	pShadow->SetPosition(D3DXVECTOR3(0, 0, 0));
	pImageView->AddChild(pShadow);
	m_vecShadows.push_back(pShadow);
	SAFE_RELEASE(pShadow);

	pShadow = new cUIImageView(m_pSprite);
	pShadow->SetTextureFilename(std::string("../Resources/UI/UI_ShortCut/cooltimeshadow2.png"));
	pShadow->SetScale(D3DXVECTOR3(0.0f, 1.0f, 1.0f));
	pShadow->SetPosition(D3DXVECTOR3(0, 0, 0));
	pImageView->AddChild(pShadow);
	m_vecShadows.push_back(pShadow);
	///
	SAFE_RELEASE(pImageView);
	SAFE_RELEASE(pShadow);

	m_vecItems.push_back(p);
	p->AddRef();


	m_vecDelayCoolTime.push_back(p->GetItemCoolTime());
}

void cUISkillShortCut::Update(float fDelta){
	float fRate = 0.0f;
	for (size_t i = 0; i < m_vecDelayCoolTime.size(); i++){
		if (m_vecDelayCoolTime[i] >= m_vecItems[i]->GetCoolTime()){

			m_vecDelayCoolTime[i] = m_vecItems[i]->GetCoolTime();
			
			if (m_vecShadows[i]->GetScale().x != 0.0f){
				m_vecShadows[i]->SetScale(D3DXVECTOR3(0.0f, 1.0f, 1.0f));
			}
		
		}
		else{
			m_vecDelayCoolTime[i] += fDelta;
			fRate = 1.0f -(m_vecDelayCoolTime[i] / m_vecItems[i]->GetCoolTime());
			m_vecShadows[i]->SetScale(D3DXVECTOR3(fRate, 1.0f, 1.0f));
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
	
	if (g_pControlManager->GetInputInfo('C') == true || g_pControlManager->GetInputInfo('c') == true){
		// ½ºÅ³ c½½·Ô ½ÇÇà
		if (m_vecDelayCoolTime[2] >= m_vecItems[2]->GetCoolTime()){
			m_vecDelayCoolTime[2] = 0.0f;
		}
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

void	cUISkillShortCut::UpdateInventoryItemCount(int nIndex, int nCount){
	assert(nIndex < m_vecItems.size());
	m_vecItems[nIndex]->SetCount(nCount);
}

int		cUISkillShortCut::GetInventoryItemCount(int nIndex){
	assert(nIndex < m_vecItems.size());
	return m_vecItems[nIndex]->GetCount();
}