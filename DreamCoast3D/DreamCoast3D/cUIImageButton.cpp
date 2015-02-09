#include "stdafx.h"
#include "cUIImageButton.h"


cUIImageButton::cUIImageButton(LPD3DXSPRITE pSprite)
	: cUIObject(pSprite)
	, m_pDelegate(NULL)
	, m_eButtonState(E_NORMAL)
{
}

cUIImageButton::~cUIImageButton(void)
{
}

void cUIImageButton::Setup(std::string sNormal, std::string sMouseOver, std::string sSelected)
{
	m_sNormal = sNormal;
	m_sMouseOver = sMouseOver;
	m_sSelected = sSelected;

	D3DXIMAGE_INFO stImageInfo;
	g_pTextureManager->GetTexture(sNormal, &stImageInfo);
	m_stSize.fWidth = (float)stImageInfo.Width;
	m_stSize.fHeight = (float)stImageInfo.Height;
}

void cUIImageButton::Update(float fDelta)
{
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
				if (m_pDelegate)
					m_pDelegate->OnClick(this);
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

void cUIImageButton::Render()
{
	if (!m_pSprite)
		return;

	m_pSprite->Begin(D3DXSPRITE_ALPHABLEND | D3DXSPRITE_SORT_TEXTURE);


	LPDIRECT3DTEXTURE9 pTexture = NULL;
	if (E_NORMAL == m_eButtonState)
	{
		pTexture = g_pTextureManager->GetTexture(m_sNormal);
	}
	else if (E_MOUSE_OVER == m_eButtonState)
	{
		pTexture = g_pTextureManager->GetTexture(m_sMouseOver);
	}
	else
	{
		pTexture = g_pTextureManager->GetTexture(m_sSelected);
	}
	RECT rc;
	SetRect(&rc, 0, 0, (int)m_stSize.fWidth, (int)m_stSize.fHeight);
	m_pSprite->SetTransform(&m_matWorld);
	m_pSprite->Draw(pTexture,
		&rc,
		&D3DXVECTOR3(0, 0, 0),
		&D3DXVECTOR3(0, 0, 0),
		D3DCOLOR_XRGB(255, 255, 255));

	m_pSprite->End();

	cUIObject::Render();
}
