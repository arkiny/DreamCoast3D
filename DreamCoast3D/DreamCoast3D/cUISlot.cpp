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
	m_rectDrawArea = { 0, 0, 0, 0 };
}

cUISlot::~cUISlot()
{
	//자기 칸에 가지고 있는 아이템 해제
	//SAFE_RELEASE(m_pItem);
}
//void cUISlot::Update(float fDelta)
//{
//
//}
//void cUISlot::Render()
//{
//
//}
