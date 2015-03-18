#include "stdafx.h"
#include "cUIStatWindow.h"
#include "cUIImageView.h"
#include "cUIImageButton.h"
#include "cUITextView.h"
#include "cGamePlayableObject.h"

cUIStatWindow::cUIStatWindow()
	: m_fMaxHealth(1000.0f)
	, m_fCurrentHealth(1000.0f)
	, m_fMaxMana(100.0f)
	, m_fCurrentMana(100.0f)
	, m_fTargetMaxHealth(NULL)
	, m_fTargetCurrentHealth(NULL)
	, m_fTargetMaxMana(NULL)
	, m_fTargetCurrentMana(NULL)
	, m_pMana(NULL)
	, m_pHealth(NULL)
{	 
}


cUIStatWindow::~cUIStatWindow()
{
	SAFE_RELEASE(m_pSprite);
	if (m_pUIRoot)
		m_pUIRoot->Destroy();
}

void cUIStatWindow::Setup(){
	D3DXCreateSprite(g_pD3DDevice, &m_pSprite);
	
	// TODO 차후 커스터마이징 할수 있게 할 것
	// 지금은 절대 좌표

	float scaleX = 0.5f;
	float scaleY = 0.4f;

	cUIImageView* pImageView = new cUIImageView(m_pSprite);
	pImageView->SetTextureFilename(std::string("../Resources/UI/UI_CharacterWindow/CharacterWindow_IED.tga"));
	pImageView->SetScale(D3DXVECTOR3(scaleX, scaleY, 1.0f));
	ST_SIZE stSize = pImageView->GetSize();

	RECT tempRect;
	GetClientRect(g_hWnd, &tempRect);
	float middle = tempRect.right / 2.0f;
	float imageMiddle = (stSize.fWidth * scaleX) / 2.0f;

	float f3qfromtop = (tempRect.bottom / 4.0f) * 3.0f;
	float imagehMiddle = (stSize.fHeight* scaleY) / 2.0f;
	pImageView->SetPosition(D3DXVECTOR3(middle - imageMiddle, f3qfromtop - imagehMiddle, 0.0f));

	m_pUIRoot = pImageView;
	stSize = m_pUIRoot->GetSize();

	m_pHealth = new cUIImageView(m_pSprite);
	m_pHealth->SetTextureFilename(std::string("../Resources/UI/UI_CharacterWindow/CharacterWindow_I11D.tga"));
	m_pHealth->SetPosition(D3DXVECTOR3(130.0f, 15.0f, 0));
	/*ST_SIZE stHealthSize = m_pHealth->GetSize();
	float healthMAx = stSize.fWidth / stHealthSize.fWidth;*/
	// 21.4 가 max
	// TODO 차후 체력 수치에 맞도록 스케일링
	// 체력바 색을 좀더 밝게... 폰트 출력으로 수치 출력 추가


	m_pHealth->SetScale(D3DXVECTOR3(21.4, 1.0f, 1.0f));
	m_pUIRoot->AddChild(m_pHealth);
	m_pHealth->Release();

	pImageView = new cUIImageView(m_pSprite);
	pImageView->SetTextureFilename(std::string("../Resources/UI/UI_CharacterWindow/CharacterWindow_IED.tga"));
	/*pImageView->SetPosition(D3DXVECTOR3(0, 20, 0));*/
	//pImageView->SetScale(D3DXVECTOR3(0.9f, 1.0f, 0.0f));
	pImageView->SetPosition(
		D3DXVECTOR3
		(0.0f, stSize.fHeight - 10.0f, 0.0f));

	// 지금은 늘여서 쓰고 있지만...
	m_pMana = new cUIImageView(m_pSprite);
	m_pMana->SetTextureFilename(std::string("../Resources/UI/UI_CharacterWindow/CharacterWindow_I10E.tga"));
	m_pMana->SetPosition(D3DXVECTOR3(130.0f, 14.0f, 0));

	// 21.3 이 좌우 max치
	m_pMana->SetScale(D3DXVECTOR3(21.3f, 1.0f, 1.0f));
	pImageView->AddChild(m_pMana);
	m_pMana->Release();

	m_pUIRoot->AddChild(pImageView);
	SAFE_RELEASE(pImageView);

	pImageView = new cUIImageView(m_pSprite);
	pImageView->SetTextureFilename(std::string("../Resources/UI/UI_CharacterWindow/CharacterWindow_I120.tga"));
	pImageView->SetPosition(D3DXVECTOR3(0, -10.0f, 0));
	pImageView->SetScale(D3DXVECTOR3(2.0f, 2.0f, 1.0f));
	m_pUIRoot->AddChild(pImageView);
	SAFE_RELEASE(pImageView);
}

void cUIStatWindow::Update(float fDelta){
	if (m_pGameObjDelgate){
		float fcurHP = m_pGameObjDelgate->GetPlayerStatInfo()->fCurrentHp;
		float fmaxHp = m_pGameObjDelgate->GetPlayerStatInfo()->fMaxHp;
		float fHPrate = fcurHP / fmaxHp;

		float fcurMP = m_pGameObjDelgate->GetPlayerStatInfo()->fCurrentMana;
		float fmaxMP = m_pGameObjDelgate->GetPlayerStatInfo()->fMaxMana;
		float fMPrate = fcurMP / fmaxMP;

		m_pHealth->SetScale(D3DXVECTOR3(21.4*fHPrate, 1.0f, 1.0f));
		m_pMana->SetScale(D3DXVECTOR3(21.3f*fMPrate, 1.0f, 1.0f));
	}
	if (m_pUIRoot)
		m_pUIRoot->Update(fDelta);
}

void cUIStatWindow::Render(){
	if (m_pSprite)
	{
		m_pUIRoot->Render();	
	}
}
