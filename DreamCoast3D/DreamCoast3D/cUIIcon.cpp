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
}
