#include "stdafx.h"
#include "cUIExpBar.h"
#include "cUIImageView.h"
#include "cUIImageButton.h"
#include "cUITextView.h"

cUIExpBar::cUIExpBar() :
m_fCurrentExp(100.0f),
m_fMaxExp(100.0f)
{
}


cUIExpBar::~cUIExpBar()
{
	SAFE_RELEASE(m_pSprite);
	if (m_pUIRoot)
		m_pUIRoot->Destroy();
}

void cUIExpBar::Setup(){
	RECT clientRect;
	GetClientRect(g_hWnd, &clientRect);

	D3DXCreateSprite(g_pD3DDevice, &m_pSprite);

	cUIImageView* pImageView = new cUIImageView(m_pSprite);
	pImageView->SetTextureFilename(std::string(
		"../Resources/UI/UI_ExpBar/ExpBar_I5.tga"));
	

	ST_SIZE stSize = pImageView->GetSize();
	pImageView->SetPosition(D3DXVECTOR3(0, clientRect.bottom-stSize.fHeight, 0.0f));
	pImageView->SetScale(D3DXVECTOR3((clientRect.right / stSize.fWidth), 1.0f, 1.0f));

	m_pUIRoot = pImageView;

	//ExpBar_I8
	pImageView = new cUIImageView(m_pSprite);
	pImageView->SetTextureFilename(std::string(
		"../Resources/UI/UI_ExpBar/ExpBar_I8.tga"));
	pImageView->SetPosition(D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	// 슬라이드바 비율로 늘리는 공식
	float fInitialRate = m_fCurrentExp / m_fMaxExp;
	ST_SIZE stSlideSize = pImageView->GetSize();
	float max = stSize.fWidth / stSlideSize.fWidth;

	pImageView->SetScale(D3DXVECTOR3(max, 1.0f, 1.0f));
	m_pUIRoot->AddChild(pImageView);
	SAFE_RELEASE(pImageView);

	float period = stSize.fWidth / 10.0f;
	for (int i = 1; i < 10; i++){
		pImageView = new cUIImageView(m_pSprite);
		//ExpBar_I10
		pImageView->SetTextureFilename(std::string(
			"../Resources/UI/UI_ExpBar/ExpBar_I10.tga"));
		pImageView->SetPosition(D3DXVECTOR3(i*period, 0.0f, 0.0f));
		m_pUIRoot->AddChild(pImageView);
		SAFE_RELEASE(pImageView);
	}

	
}

void cUIExpBar::Update(float fDelta){
	if (m_pUIRoot)
		m_pUIRoot->Update(fDelta);
}

void cUIExpBar::Render(){
	if (m_pSprite)
	{
		m_pUIRoot->Render();
	}
}