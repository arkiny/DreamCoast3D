#pragma once
#include "cUIWindow.h"
#include "cUIPopupWindow.h"
#include "iUIPopupWindowDelegate.h"
#include "cUIIcon.h"

class cUIInventory : public cUIPopupWindow, public iUIPopupWindowDelegate
{
protected:
	
	bool m_isEnable;					//Ȱ��ȭ �Ǿ��°�
	//bool m_isVisible;					//���̴� ���ΰ�(cUIPopupWindow bShow�� ����Ѵ�)
	bool m_isDragging;					//�巡�� �Ǵ� ���ΰ�
	std::vector<cUIIcon> m_vecUIIcon;	//�κ��丮 �ȿ� ��� �ִ� ������ ���� �����̳�
	//SYNTHESIZE(iUIPopupWindowDelegate*, m_pUIPopupWindowDelegate, UIPopupWindowDelegate);
	SYNTHESIZE(iUIPopupWindowDelegate*, m_pUIPopupWindowDelegate, UIPopupWindowDelegate);
	SYNTHESIZE(D3DXVECTOR3, m_vBeforeDragPos, BeforeDragPos);
public:
	cUIInventory();
	~cUIInventory();

	void cUIInventory::Setup();
	virtual void cUIInventory::Update(float fDelta) override; 
	
	virtual void cUIInventory::OnMouseLBDown() override;
	virtual void cUIInventory::OnMouseLBUp() override;
	virtual void cUIInventory::Drag() override;
	void cUIInventory::AddItem(cUIIcon& UIIcon);
};

