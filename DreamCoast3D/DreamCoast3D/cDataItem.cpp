#include "stdafx.h"
#include "cDataItem.h"
#include "cUIIcon.h"


cDataItem::cDataItem()
	: m_pUIIcon(nullptr)
	, m_eItemType(EITEMTYPE_MAX)
	, m_nItemNum(-1)
	, m_isStackable(false)
	, m_nMaxStack(0)
	, m_nCurrStack(0)
{
	
}
cDataItem::cDataItem(cUIIcon* pUIIcon)
	: m_pUIIcon(pUIIcon)
	, m_eItemType(EITEMTYPE_MAX)
	, m_nItemNum(-1)
	, m_isStackable(false)
	, m_nMaxStack(0)
	, m_nCurrStack(0)
{

}

cDataItem::~cDataItem()
{
	//아이템의 아이콘 해제
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
