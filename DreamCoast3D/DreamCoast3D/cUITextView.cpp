#include "stdafx.h"
#include "cUITextView.h"


cUITextView::cUITextView(LPD3DXSPRITE pSprite)
	: cUIObject(pSprite)
	, m_eFontType(cFontManager::FONT_DEFAULT)
	, m_dwDrawTextFormat(DT_LEFT | DT_TOP)
	, m_dwTextColor(D3DCOLOR_XRGB(255, 255, 255))
{
}


cUITextView::~cUITextView(void)
{
}

void cUITextView::Render()
{
	RECT rc;
	SetRect(&rc, 0, 0, (int)(m_stSize.fWidth), (int)(m_stSize.fHeight));

	LPD3DXFONT pFont = g_pFontManager->GetFont(m_eFontType);

	m_pSprite->Begin(D3DXSPRITE_ALPHABLEND | D3DXSPRITE_SORT_TEXTURE);

	m_pSprite->SetTransform(&m_matWorld);

	pFont->DrawTextA(m_pSprite,
		m_sText.c_str(),
		m_sText.length(),
		&rc,
		m_dwDrawTextFormat,
		m_dwTextColor);

	m_pSprite->End();

	cUIObject::Render();
}
