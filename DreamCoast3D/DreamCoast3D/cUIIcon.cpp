#include "stdafx.h"
#include "cUIIcon.h"


cUIIcon::cUIIcon()
	: m_eIconType(EICONTYPE_MAX)
{
}
cUIIcon::cUIIcon(LPD3DXSPRITE pSprite)
	: cUIImageView(pSprite)
	, m_eIconType(EICONTYPE_MAX)
{
}


cUIIcon::~cUIIcon()
{
	//SAFE_RELEASE(m_pSprite);
}

void cUIIcon::SetSprite(LPD3DXSPRITE pSprite)
{
	m_pSprite = pSprite;
}

LPD3DXSPRITE cUIIcon::GetSprite()
{
	return m_pSprite;
}
void cUIIcon::Render()
{
	//cUIImageView::Render();
	if (!m_pSprite)
		return;

	m_pSprite->Begin(D3DXSPRITE_ALPHABLEND | D3DXSPRITE_SORT_TEXTURE);

	LPDIRECT3DTEXTURE9 pTexture = g_pTextureManager->GetTexture(m_sTextureFilename);

	m_pSprite->SetTransform(&m_matWorld);
	m_pSprite->Draw(pTexture,
		&m_rectDrawArea,
		&D3DXVECTOR3(0, 0, 0),
		&D3DXVECTOR3(0, 0, 0),
		D3DXCOLOR(1.f, 1.f, 1.f, GetAlpha()));

	m_pSprite->End();

	
}