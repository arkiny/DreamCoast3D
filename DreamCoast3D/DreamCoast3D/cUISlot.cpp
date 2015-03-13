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
//�������� �����ϸ鼭 �����ۿ� �ִ� �ؽ��� ��η� �����ܱ��� �Ҵ��ؼ� �����Ѵ�.
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
	//���Կ� ��ϵ� �������� ImageView Ʈ���������� �����Ѵ�.
	SubChild(GetIcon());
	//�������� ��������
	SetItem(nullptr);
	//�������� ��������
	SetIcon(nullptr);
}