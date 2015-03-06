#include "stdafx.h"
#include "cUISlot.h"
#include "cUIObject.h"

cUISlot::cUISlot()
	: m_pIcon(nullptr)
{
	m_rectDrawArea = { 0, 0, 0, 0 };
}
cUISlot::cUISlot(LPD3DXSPRITE pSprite)
	: cUIImageView(pSprite)
	, m_pIcon(nullptr)
{

}

cUISlot::~cUISlot()
{
}
