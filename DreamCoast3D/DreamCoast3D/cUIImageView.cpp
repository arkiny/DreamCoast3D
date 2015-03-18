#include "stdafx.h"
#include "cUIImageView.h"

cUIImageView::cUIImageView(LPD3DXSPRITE pSprite)
	: cUIObject(pSprite)
{
	m_rectDrawArea = { 0, 0, 0, 0 };
}


cUIImageView::~cUIImageView(void)
{
}
std::string& cUIImageView::GetTextureFilename()
{
	return m_sTextureFilename;
}

void cUIImageView::SetTextureFilename(std::string& sTextureFilename)
{
	m_sTextureFilename = sTextureFilename;
	g_pTextureManager->GetTexture(m_sTextureFilename, &m_stImageInfo);
	SetRect(&m_rectDrawArea, 0, 0, m_stImageInfo.Width, m_stImageInfo.Height);
}

void cUIImageView::Setup(std::string sTexturePath, int nDrawRectRight, int nDrawRectBottom, D3DXVECTOR3& vPos, D3DXVECTOR3& vScale)
{
	SetTextureFilename(sTexturePath);
	RECT rtDrawArea = { 0, 0, nDrawRectRight, nDrawRectBottom };
	SetDrawArea(rtDrawArea);
	SetPosition(vPos);	//부모에 의한 상대좌표
	SetScale(vScale);
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

	if (m_bShowChild){
		cUIObject::Render();
	}
}

ST_SIZE cUIImageView::GetSize() const
{
	return ST_SIZE((float)m_stImageInfo.Width, (float)m_stImageInfo.Height);
}

