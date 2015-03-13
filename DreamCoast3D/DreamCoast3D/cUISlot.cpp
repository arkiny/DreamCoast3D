#include "stdafx.h"
#include "cUISlot.h"
#include "cUIObject.h"
#include "cDataItem.h"

cUISlot::cUISlot()
	: m_pIcon(nullptr)
	, m_pItem(nullptr)
{
	m_rectDrawArea = { 0, 0, 0, 0 };
}
cUISlot::cUISlot(LPD3DXSPRITE pSprite)
	: cUIImageView(pSprite)
	, m_pIcon(nullptr)
	, m_pItem(nullptr)
{
}

cUISlot::~cUISlot()
{

}
void cUISlot::Update(float fDelta)
{
	if (m_pIcon)
	{
		//m_pIcon->SetPosition(GetPosition().x + 4, GetPosition().y + 4);
		m_pIcon->Update(fDelta);
	}
	cUIImageView::Update(fDelta);
}
void cUISlot::Render()
{
	if (m_pIcon)
	{
		m_pIcon->Render();
	}
	cUIImageView::Render();
}
