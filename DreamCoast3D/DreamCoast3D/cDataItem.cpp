#include "stdafx.h"
#include "cDataItem.h"
#include "cUIIcon.h"


cDataItem::cDataItem()
	: m_pUIIcon(nullptr)
{
}
cDataItem::cDataItem(cUIIcon* pUIIcon)
	: m_pUIIcon(pUIIcon)
{

}

cDataItem::~cDataItem()
{
	SAFE_RELEASE(m_pUIIcon);
}
void cDataItem::SetupItemData(EITEMTYPE	eItemType, int nItemNum, bool isStackable, size_t nMaxStack)
{
	m_eItemType = eItemType;
	m_nItemNum = nItemNum;		
	m_isStackable = isStackable;	
	m_nMaxStack = nMaxStack;	
}
//void cDataItem::SetupItemIcon(cUIIcon* pUIIcon)
//{
//	m_pUIIcon = pUIIcon;
//}
