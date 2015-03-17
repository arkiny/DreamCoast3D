#pragma once
#include "cUIWindow.h"
#include "cUIPopupWindow.h"
#include "iUIPopupWindowDelegate.h"
#include "iUIInventoryDelegate.h"
#include "cUIIcon.h"
#include "cUISlot.h"

class cUIImageView;
class cDataItem;

class cUIInventory : public cUIPopupWindow, public iUIPopupWindowDelegate, public iUIInventoryDelegate
{
protected:
	enum EDRAGSTATE
	{
		E_NODRAG,
		E_EMPTY_DRAG,
		E_ICON_DRAG,
		E_WINDOW_DRAG,
		E_DRAG_MAX
	};
	cUIImageView*	m_pImageViewMouseOver;	//���콺 ���� �Ǿ����� ������ �׵θ�
	cUIImageView*	m_pImageViewMouseDown;	//���콺 �ٿ� �Ǿ����� ������ �׵θ�

	std::vector<cDataItem*> m_vecOwnItem;	//�κ��丮 �ȿ� ��� �ִ� ������ ���� �����̳�

	SYNTHESIZE(bool, m_isDragging, IsDragging);	//�巡�� �Ǵ� ���ΰ�
	SYNTHESIZE(bool, m_isKeyHold, IsKeyHold);	//Ű�� ������ ���ΰ�
	SYNTHESIZE(iUIPopupWindowDelegate*, m_pUIPopupWindowDelegate, UIPopupWindowDelegate);
	SYNTHESIZE(D3DXVECTOR3, m_vBeforeDragPos, BeforeDragPos); //�巡�� �� �� ���콺�� ���� �� ó�� ���۵� ��ġ
	SYNTHESIZE(char, m_chHotKeyCode, HotKey);		//����Ű(�ƽ�Ű)
	SYNTHESIZE(cUISlot*, m_pFocusSlot, FocusSlot);	//���� ���̶���Ʈ �� ����
	SYNTHESIZE(std::vector<cUISlot*>, m_vecUISlot, vecUISlot);//�κ��丮�� �� ĭ
	SYNTHESIZE(EDRAGSTATE, m_eDragState, DragState);	//���� �巡�� ����
	SYNTHESIZE(bool, m_isMouseOverVisible, IsMouseOverVisible);//���콺 ���� �׵θ� ��� ���� ����
	SYNTHESIZE(bool, m_isMouseDownVisible, IsMouseDownVisible);//���콺 �ٿ� �׵θ� ��� ���� ����
	SYNTHESIZE(cUIIcon*, m_pIconDragSrc, IconDragSrc);	//�巡�װ� ���۵� ��� ������ �����
	SYNTHESIZE(cUIIcon*, m_pBindingIcon, BindingIcon);	//�巡���� ���콺�� �پ� �ִ� ������
	SYNTHESIZE(cUISlot*, m_pSlotDragSrc, SlotDragSrc);	//���� �巡�װ� ���۵� ����
	SYNTHESIZE(cUISlot*, m_pSlotDragDest, SlotDragDest);//���� �巡�װ� �Ϸ�� ����

public:
	cUIInventory();
	~cUIInventory();

	virtual void cUIInventory::Setup() override;
	virtual void cUIInventory::Update(float fDelta) override; 
	virtual void cUIInventory::Render() override;
	virtual void cUIInventory::OnMouseLBDown() override;
	virtual void cUIInventory::OnMouseLBUp() override;
	virtual void cUIInventory::OnMouseRBDown() override;
	virtual void cUIInventory::OnMouseRBUp() override;

	virtual void cUIInventory::Drag() override {}
	virtual void cUIInventory::Drag(EDRAGSTATE eDragState);
	virtual cUIObject* cUIInventory::FindFocusSlot(std::vector<cUISlot*>& vecUISlot);		//���콺 Ŀ�� �Ʒ��� �ִ� ������ ã�´�.

	void cUIInventory::SetupTest();	//��� �׽�Ʈ��
	virtual void cUIInventory::AddItem(cDataItem* pDataItem) override;		//������ �߰�
	virtual void cUIInventory::DeleteItemInSlot(size_t nSlotNum) override;	//���� ��ȣ�� ������ ����
	virtual void cUIInventory::DeleteItemInSlot(cUISlot* pUISlot) override;	//���� �����ͷ� ������ ����
	virtual cDataItem* cUIInventory::UseItem(cUISlot* pUISlot);				//������ ����
	virtual void cUIInventory::SetupSlot(size_t nRowQnt, size_t nColQnt);	//������ ���ڸ�ŭ �����ϰ� ��ġ�Ѵ�.
	virtual void cUIInventory::SwapItemInSlot(cUISlot* pSrcSlot, cUISlot* pDestSlot);	//���԰� ������ ��ȯ
	virtual void cUIInventory::HighLightFocusSlot();						//���콺 ����/�ٿ� �� ���� ���̶���Ʈ ��Ű��
};

