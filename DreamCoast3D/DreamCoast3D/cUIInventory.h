#pragma once
#include "cUIWindow.h"
//#include "cUITextView.h"
#include "cUIIcon.h"
#include "cUIPopupWindow.h"

class cUIInventory;
__interface iInventoryDelegate
{
	virtual void OnMouseLBDown(cUIInventory* pSender);
	virtual void OnMouseLBUp(cUIInventory* pSender);
	virtual void OnMouseMove(cUIInventory* pSender);
};

class cUIInventory : public cUIPopupWindow, public iInventoryDelegate
{
protected:
	
	bool m_isEnable;					//Ȱ��ȭ �Ǿ��°�
	//bool m_isVisible;					//���̴� ���ΰ�(cUIPopupWindow bShow�� ����Ѵ�)
	bool m_isDragging;					//�巡�� �Ǵ� ���ΰ�
	std::vector<cUIIcon> m_vecUIIcon;	//�κ��丮 �ȿ� ��� �ִ� ������ ���� �����̳�
	SYNTHESIZE(iInventoryDelegate*, m_pInventoryDelegate, InventoryDelegate);
public:
	cUIInventory();
	~cUIInventory();

	void cUIInventory::Setup();
	virtual void cUIInventory::Update(float fDelta) override; 
	//virtual void cUIInventory::Render();
	void cUIInventory::AddItem(cUIIcon& UIIcon);
	virtual void cUIInventory::OnMouseLBDown(cUIInventory* pSender);
	virtual void cUIInventory::OnMouseLBUp(cUIInventory* pSender);
	virtual void cUIInventory::OnMouseMove(cUIInventory* pSender);
	void cUIInventory::Drag();
	//void cUIInventory::OnMouseLBDown();
	//void cUIInventory::OnMouseLBUp();
	void cUIInventory::OnMouseRBDown();
	void cUIInventory::OnMouseRBUp();
	void cUIInventory::OnMouseMove();


};

