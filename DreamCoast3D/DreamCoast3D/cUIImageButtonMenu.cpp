#include "stdafx.h"
#include "cUIImageButtonMenu.h"


cUIImageButtonMenu::cUIImageButtonMenu(LPD3DXSPRITE pSprite)
	:cUIImageButton(pSprite)
{

}


cUIImageButtonMenu::~cUIImageButtonMenu()
{
	SAFE_RELEASE(m_pSprite);
}

void cUIImageButtonMenu::Setup(
	std::string sNormal,
	std::string sMouseOver,
	std::string sSelected){
	cUIImageButton::Setup(sNormal, sMouseOver, sSelected);
}

void cUIImageButtonMenu::Update(float fDelta){
	RECT rc;
	rc.left = (LONG)m_matWorld._41;
	rc.top = (LONG)m_matWorld._42;

	D3DXVECTOR3 v = D3DXVECTOR3(m_stSize.fWidth, m_stSize.fHeight, 1);
	D3DXMATRIXA16 matS;
	D3DXMatrixScaling(&matS, m_matWorld._11, m_matWorld._22, m_matWorld._33);
	D3DXVec3TransformCoord(&v, &v, &matS);
	rc.right = (LONG)m_matWorld._41 + (LONG)v.x;
	rc.bottom = (LONG)m_matWorld._42 + (LONG)v.y;

	POINT ptMouse;
	GetCursorPos(&ptMouse);
	ScreenToClient(g_hWnd, &ptMouse);

	if (PtInRect(&rc, ptMouse))
	{
		if (GetKeyState(VK_LBUTTON) & 0x8000)
		{
			if (m_eButtonState == E_MOUSE_OVER)
			{
				m_eButtonState = E_SELECTED;
			}
		}
		else
		{
			if (m_eButtonState == E_SELECTED)
			{
				if (m_pMenuDelegate)
					m_pMenuDelegate->MenuControl(this, m_nTarget);
			}
			m_eButtonState = E_MOUSE_OVER;
		}
	}
	else
	{
		if (GetKeyState(VK_LBUTTON) & 0x8000)
		{
			if (m_eButtonState != E_SELECTED)
				m_eButtonState = E_NORMAL;
		}
		else
		{
			m_eButtonState = E_NORMAL;
		}
	}

	cUIObject::Update(fDelta);
}

void cUIImageButtonMenu::Render(){
	cUIImageButton::Render();
}