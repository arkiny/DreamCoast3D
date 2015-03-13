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
//아이템을 세팅하면서 아이템에 있는 텍스쳐 경로로 아이콘까지 할당해서 세팅한다.
void cUISlot::SetItem(cDataItem* pDataItem)
{
	m_pItem = pDataItem;
	if (pDataItem)
	{
		LPD3DXSPRITE pSprite = nullptr;
		D3DXCreateSprite(g_pD3DDevice, &pSprite);

		cUIIcon* pUIIcon = new cUIIcon(pSprite);
		pUIIcon->Setup(
			pDataItem->GetUIIconImagePath(),
			56, 56,
			D3DXVECTOR3(4, 4, 0),
			D3DXVECTOR3(.8f, .8f, .8f));

		SetIcon(pUIIcon);
		AddChild(GetIcon());//AddRef
		SAFE_RELEASE(pUIIcon);
		SAFE_RELEASE(pSprite);
	}
}
cDataItem* cUISlot::GetItem()
{
	return m_pItem;
}
void cUISlot::DeleteItem()
{
	//슬롯에 등록된 아이콘을 ImageView 트리구조에서 제외한다.
	SubChild(GetIcon());
	//아이템을 공백으로
	SetItem(nullptr);
	//아이콘을 공백으로
	SetIcon(nullptr);
}