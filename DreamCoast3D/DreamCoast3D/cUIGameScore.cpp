#include "stdafx.h"
#include "cUIGameScore.h"
#include "cUITextView.h"

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
	cUITextView* pTextView = new cUITextView(m_pSprite);
	std::stringstream ss;
	if (m_pGameObjDelgate){
		ss << m_pGameObjDelgate->GetScore();
	}
	pTextView->SetText(ss.str());
	ss.str("");
	pTextView->SetDrawTextFormat(DT_LEFT | DT_TOP | DT_WORDBREAK);
	pTextView->SetTextColor(D3DCOLOR_XRGB(255, 255, 255));
	pTextView->SetPosition(10, 30);
	ST_SIZE stSize = { 200, 200 };
	pTextView->SetSize(stSize);
	m_pUIRoot = pTextView;
	m_pText = pTextView;
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