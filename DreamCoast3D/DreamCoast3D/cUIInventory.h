#pragma once
#include "cUIWindow.h"
//#include "cUITextView.h"
#include "cUIIcon.h"
#include "cUIPopupWindow.h"

class cUIInventory : public cUIPopupWindow
{
protected:
	
	bool m_isEnable;					//Ȱ��ȭ �Ǿ��°�
	bool m_isVisible;					//���̴� ���ΰ�
	std::vector<cUIIcon> m_vecUIIcon;	//�κ��丮 �ȿ� ��� �ִ� ������ ���� �����̳�
public:
	cUIInventory();
	~cUIInventory();

	void cUIInventory::Setup();
	void cUIInventory::Update(float fDelta);
	void cUIInventory::Render();
	void cUIInventory::AddItem(cUIIcon& UIIcon);
	void cUIInventory::OnMouseLBDown();
	void cUIInventory::OnMouseLBUp();
	void cUIInventory::OnMouseRBDown();
	void cUIInventory::OnMouseRBUp();
	void cUIInventory::OnMouseMove();

};

