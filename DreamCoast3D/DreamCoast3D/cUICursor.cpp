#include "stdafx.h"
#include "cUICursor.h"


cUICursor::cUICursor() : m_bIsShow(true)
{
	m_rectDrawArea = { 0, 0, 0, 0 };
	LPD3DXSPRITE sprite;
	D3DXCreateSprite(g_pD3DDevice, &sprite);
	m_pSprite = sprite;
}


cUICursor::~cUICursor() 
{
	SAFE_RELEASE(m_pSprite);
}

cUICursor::cUICursor(LPD3DXSPRITE pSprite)
	:cUIImageView(pSprite), m_bIsShow(true)
{
}

void cUICursor::Update(float fDelta) {
	POINT p;
	GetCursorPos(&p);
	ScreenToClient(g_hWnd, &p);
	m_vPosition = D3DXVECTOR3((float)p.x, (float)p.y, 0.0f);

	cUIObject::Update(fDelta);
}

void cUICursor::Render(){
	if (m_bIsShow){
		cUIImageView::Render();
	}
}
