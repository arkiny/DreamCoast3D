#include "stdafx.h"
#include "cUIExample.h"
#include "cUIImageView.h"
#include "cUIImageButton.h"
#include "cUITextView.h"

cUIExample::cUIExample()
{
}

cUIExample::~cUIExample()
{
	SAFE_RELEASE(m_pSprite);
	if (m_pUIRoot)
		m_pUIRoot->Destroy();
	
	//this->Destroy();
}

void cUIExample::Setup(){
	D3DXCreateSprite(g_pD3DDevice, &m_pSprite);

	cUIImageView* pImageView = new cUIImageView(m_pSprite);
	pImageView->SetPosition(D3DXVECTOR3(200, 100, 0));
	//pImageView->SetScale(D3DXVECTOR3(0.5f, 0.5f, 0.5f));
	pImageView->SetScale(D3DXVECTOR3(1.0f, 1.0f, 1.0f));
	pImageView->SetTextureFilename(std::string("../Resources/UI/UI_basicBox.png"));
	m_pUIRoot = pImageView;

	cUITextView* pTextView = new cUITextView(m_pSprite);
	pTextView->SetText(std::string("퀘 스트 퀘 스 트 스 트"));
	pTextView->SetDrawTextFormat(DT_CENTER | DT_TOP | DT_WORDBREAK);
	pTextView->SetTextColor(D3DCOLOR_XRGB(80, 80, 80));
	pTextView->SetPosition(12, 44);
	ST_SIZE stSize = pImageView->GetSize();
	stSize.fWidth -= 20;
	pTextView->SetSize(stSize);
	m_pUIRoot->AddChild(pTextView);
	m_pTextview = pTextView;
	SAFE_RELEASE(pTextView);

	cUIImageButton* pButton = new cUIImageButton(m_pSprite);
	pButton->SetDelegate(this);
	pButton->SetTag(1);
	pButton->Setup(
		std::string("../Resources/UI/UI_AcceptButton.png"),
		std::string("../Resources/UI/UI_AcceptButtonOn.png"),
		std::string("../Resources/UI/UI_AcceptButtonClick.png"));
	pButton->SetPosition(21, 271);
	m_pUIRoot->AddChild(pButton);
	SAFE_RELEASE(pButton);

	pButton = new cUIImageButton(m_pSprite);
	pButton->SetDelegate(this);
	pButton->SetTag(2);
	pButton->Setup(
		std::string("../Resources/UI/UI_CancelButton.png"),
		std::string("../Resources/UI/UI_CancelButtonClick.png"),
		std::string("../Resources/UI/UI_CancelButtonOn.png"));
	//float x = m_pUIRoot->GetSize().fWidth - pButton->GetSize().fWidth - 50;
	pButton->SetPosition(184, 271);
	m_pUIRoot->AddChild(pButton);
	SAFE_RELEASE(pButton);
}

void cUIExample::Update(float fDelta){
	if (m_pUIRoot)
		m_pUIRoot->Update(fDelta);
}

void cUIExample::Render(){
	if (m_pSprite)
	{
		m_pUIRoot->Render();
	}
}

void cUIExample::OnClick(cUIImageButton* pSender){
	if (pSender->GetTag() == 1)
	{
		m_pTextview->SetText(std::string("수락 버튼 눌렸음"));
	}
	else
	{
		m_pTextview->SetText(std::string("취소 버튼 눌렸음"));
	}
}