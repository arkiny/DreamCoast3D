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
	//�ڱ� ĭ�� ������ �ִ� ������ ����
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
