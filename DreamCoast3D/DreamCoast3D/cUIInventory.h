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
	cUIImageView*	m_pImageViewMouseOver;	//���콺 ���� �Ǿ����� ������ �׵θ�
	bool			m_isMouseOverVisible;	//���콺 ���� �׵θ� ��� ���� ����
	cUIImageView*	m_pImageViewMouseDown;	//���콺 �ٿ� �Ǿ����� ������ �׵θ�
	bool			m_isMouseDownVisible;	//���콺 �ٿ� �׵θ� ��� ���� ����

	//std::vector<cUIIcon> m_vecUIIcon;		//�κ��丮 �ȿ� ��� �ִ� ������ ���� �����̳�(������ ��ü�� �������� ���� ����)
	std::vector<cDataItem*> m_vecOwnItem;	//�κ��丮 �ȿ� ��� �ִ� ������ ���� �����̳�
	//std::vector<cUISlot*> m_vecUISlot;		
	SYNTHESIZE(bool, m_isDragging, IsDragging);	//�巡�� �Ǵ� ���ΰ�
	SYNTHESIZE(bool, m_isKeyHold, IsKeyHold);	//Ű�� ������ ���ΰ�
	SYNTHESIZE(iUIPopupWindowDelegate*, m_pUIPopupWindowDelegate, UIPopupWindowDelegate);
	SYNTHESIZE(D3DXVECTOR3, m_vBeforeDragPos, BeforeDragPos); //�巡�� �� �� ���콺�� ���� �� ó�� ���۵� ��ġ
	SYNTHESIZE(char, m_chHotKeyCode, HotKey);		//����Ű(�ƽ�Ű)
	SYNTHESIZE(cUISlot*, m_pFocusSlot, FocusSlot);	//���� ���̶���Ʈ �� ����
	SYNTHESIZE(std::vector<cUISlot*>, m_vecUISlot, vecUISlot);//�κ��丮�� �� ĭ
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

	virtual void cUIInventory::Drag() override;
	
	virtual cUIObject* cUIInventory::FindFocusSlot(std::vector<cUISlot*>& vecUISlot);		//���콺 Ŀ�� �Ʒ��� �ִ� ������ ã�´�.

	void cUIInventory::SetupTest();	//��� �׽�Ʈ��
	virtual void cUIInventory::AddItem(cDataItem* pDataItem) override;
	virtual void cUIInventory::DeleteItemInSlot(size_t nSlotNum) override;	//���� ��ȣ��
	virtual void cUIInventory::DeleteItemInSlot(cUISlot* pUISlot) override;	//���� �����ͷ�
	virtual cDataItem* cUIInventory::UseItem(cUISlot* pUISlot);
	virtual void cUIInventory::SetupSlot(size_t nRowQnt, size_t nColQnt);	//������ ���ڸ�ŭ �����ϰ� ��ġ�Ѵ�.
};

