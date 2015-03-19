#include "stdafx.h"
#include "cUIGameScore.h"
#include "cUITextView.h"
#include "cUIImageView.h"

cUIGameScore::cUIGameScore()
{
}


cUIGameScore::~cUIGameScore()
{
	if (m_pUIRoot){
		m_pUIRoot->Destroy();
	}
	SAFE_RELEASE(m_pSprite);
}

void cUIGameScore::Setup(){
	D3DXCreateSprite(g_pD3DDevice, &m_pSprite);
	//moneyscoreIcon

	cUIImageView* pImageView = new cUIImageView(m_pSprite);
	pImageView->SetTextureFilename(std::string("../Resources/UI/UI_ShortCut/moneyscoreIcon.png"));
	pImageView->SetScale(D3DXVECTOR3(1.0f, 1.0f, 1.0f));
	pImageView->SetPosition(D3DXVECTOR3(0, 0, 0));
	m_pUIRoot = pImageView;

	cUITextView* pTextView = new cUITextView(m_pSprite);
	std::stringstream ss;
	if (m_pGameObjDelgate){
		ss << m_pGameObjDelgate->GetScore();
	}
	pTextView->SetText(ss.str());
	ss.str("");
	pTextView->SetDrawTextFormat(DT_CENTER | DT_VCENTER | DT_WORDBREAK);
	pTextView->SetTextColor(D3DCOLOR_XRGB(255, 255, 255));
	pTextView->SetPosition(0, 0);

	ST_SIZE stSize = m_pUIRoot->GetSize();
	pTextView->SetSize(stSize);
	pImageView->AddChild(pTextView);
	m_pText = pTextView;
	SAFE_RELEASE(pTextView);
}

void cUIGameScore::Update(float fDelta){
	std::stringstream ss;
	if (m_pUIRoot){
		if (m_pGameObjDelgate){
			ss << m_pGameObjDelgate->GetScore();
		}
		m_pText->SetText(ss.str());
		m_pUIRoot->Update(fDelta);
		ss.str("");
	}
}

void cUIGameScore::Render(){
	if (m_pUIRoot){
		m_pUIRoot->Render();
	}
}