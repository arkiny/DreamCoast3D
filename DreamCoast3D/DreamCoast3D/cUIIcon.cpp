#include "stdafx.h"
#include "cUIIcon.h"


cUIIcon::cUIIcon()
{
}
cUIIcon::cUIIcon(LPD3DXSPRITE pSprite)
	: cUIImageView(pSprite)
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