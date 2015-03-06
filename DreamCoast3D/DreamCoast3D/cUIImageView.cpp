#include "stdafx.h"
#include "cUIImageView.h"

cUIImageView::cUIImageView(LPD3DXSPRITE pSprite)
	: cUIObject(pSprite)
	, fAlpha(1.f)
{
	m_rectDrawArea = { 0, 0, 0, 0 };
}


cUIImageView::~cUIImageView(void)
{
}

void cUIImageView::SetTextureFilename(std::string& sTextureFilename)
{
	m_sTextureFilename = sTextureFilename;
	g_pTextureManager->GetTexture(m_sTextureFilename, &m_stImageInfo);
	SetRect(&m_rectDrawArea, 0, 0, m_stImageInfo.Width, m_stImageInfo.Height);
}

void cUIImageView::Render()
{
	if (!m_pSprite)
		return;

	m_pSprite->Begin(D3DXSPRITE_ALPHABLEND | D3DXSPRITE_SORT_TEXTURE);

	LPDIRECT3DTEXTURE9 pTexture = g_pTextureManager->GetTexture(m_sTextureFilename);
	
	m_pSprite->SetTransform(&m_matWorld);
	m_pSprite->Draw(pTexture,
		&m_rectDrawArea,
		&D3DXVECTOR3(0, 0, 0),
		&D3DXVECTOR3(0, 0, 0),
		D3DXCOLOR(1.f, 1.f, 1.f,GetAlpha()));

	m_pSprite->End();

	cUIObject::Render();
}

ST_SIZE cUIImageView::GetSize() const
{
	return ST_SIZE((float)m_stImageInfo.Width, (float)m_stImageInfo.Height);
}

