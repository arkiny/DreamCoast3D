#pragma once
#include "cUIWindow.h"
#include "cUIPopupWindow.h"
#include "iUIPopupWindowDelegate.h"
#include "cUIIcon.h"
class cUIImageView;

class cUIInventory : public cUIPopupWindow, public iUIPopupWindowDelegate
{
protected:
	cUIImageView*	m_pImageViewMouseOver;	//���콺 ���� �Ǿ����� ������ �׵θ�
	bool			m_isMouseOverVisible;	//���콺 ���� �׵θ� ��� ���� ����
	cUIImageView*	m_pImageViewMouseDown;	//���콺 �ٿ� �Ǿ����� ������ �׵θ�
	bool			m_isMouseDownVisible;	//���콺 �ٿ� �׵θ� ��� ���� ����
	cUIImageView*	m_pFocusSlot;			//���� ���̶���Ʈ �� ����
	//bool m_isEnable;					//Ȱ��ȭ �Ǿ��°�
	//bool m_isVisible;					//���̴� ���ΰ�(cUIPopupWindow bShow�� ����Ѵ�)
	bool m_isDragging;					//�巡�� �Ǵ� ���ΰ�
	bool m_isKeyHold;					//Ű�� ������ ���ΰ�

	std::vector<cUIIcon> m_vecUIIcon;	//�κ��丮 �ȿ� ��� �ִ� ������ ���� �����̳�
	//SYNTHESIZE(iUIPopupWindowDelegate*, m_pUIPopupWindowDelegate, UIPopupWindowDelegate);
	SYNTHESIZE(iUIPopupWindowDelegate*, m_pUIPopupWindowDelegate, UIPopupWindowDelegate);
	SYNTHESIZE(D3DXVECTOR3, m_vBeforeDragPos, BeforeDragPos);
	SYNTHESIZE(char, m_chHotKeyCode, HotKey);//����Ű(�ƽ�Ű)
public:
	cUIInventory();
	~cUIInventory();

	void cUIInventory::Setup();
	virtual void cUIInventory::Update(float fDelta) override; 
	virtual void cUIInventory::Render();
	virtual void cUIInventory::OnMouseLBDown() override;
	virtual void cUIInventory::OnMouseLBUp() override;
	virtual void cUIInventory::Drag() override;
	
	cUIObject* cUIInventory::FindFocusSlot(cUIObject* UIRoot);

	void cUIInventory::AddItem(cUIIcon& UIIcon);
};

