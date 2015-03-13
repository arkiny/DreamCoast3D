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
cUIIcon* cUIIcon::CloneIcon(cUIIcon* pSrcIcon)
{
	//LPD3DXSPRITE pSprite = nullptr;
	//D3DXCreateSprite(g_pD3DDevice, &pSprite);
	cUIIcon* pUIIcon = new cUIIcon;
	if (pSrcIcon)
	{
		(*pUIIcon) = (*pSrcIcon);
	}

	//SAFE_RELEASE(pSprite);
	return pUIIcon;
}