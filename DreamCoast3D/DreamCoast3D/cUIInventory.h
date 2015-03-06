#pragma once
#include "cUIWindow.h"
#include "cUIPopupWindow.h"
#include "iUIPopupWindowDelegate.h"
#include "cUIIcon.h"
#include "cUISlot.h"

class cUIImageView;
class cDataItem;

class cUIInventory : public cUIPopupWindow, public iUIPopupWindowDelegate
{
protected:
	cUIImageView*	m_pImageViewMouseOver;	//���콺 ���� �Ǿ����� ������ �׵θ�
	bool			m_isMouseOverVisible;	//���콺 ���� �׵θ� ��� ���� ����
	cUIImageView*	m_pImageViewMouseDown;	//���콺 �ٿ� �Ǿ����� ������ �׵θ�
	bool			m_isMouseDownVisible;	//���콺 �ٿ� �׵θ� ��� ���� ����
	cUISlot*		m_pFocusSlot;			//���� ���̶���Ʈ �� ����

	//std::vector<cUIIcon> m_vecUIIcon;		//�κ��丮 �ȿ� ��� �ִ� ������ ���� �����̳�(������ ��ü�� �������� ���� ����)
	std::vector<cDataItem*> m_vecItem;		//�κ��丮 �ȿ� ��� �ִ� ������ ���� �����̳�
	SYNTHESIZE(bool, m_isDragging, IsDragging);	//�巡�� �Ǵ� ���ΰ�
	SYNTHESIZE(bool, m_isKeyHold, IsKeyHold);	//Ű�� ������ ���ΰ�
	SYNTHESIZE(iUIPopupWindowDelegate*, m_pUIPopupWindowDelegate, UIPopupWindowDelegate);
	SYNTHESIZE(D3DXVECTOR3, m_vBeforeDragPos, BeforeDragPos); //�巡�� �� �� ���콺�� ���� �� ó�� ���۵� ��ġ
	SYNTHESIZE(char, m_chHotKeyCode, HotKey);	//����Ű(�ƽ�Ű)
public:
	cUIInventory();
	~cUIInventory();

	void cUIInventory::Setup();
	virtual void cUIInventory::Update(float fDelta) override; 
	virtual void cUIInventory::Render() override;
	virtual void cUIInventory::OnMouseLBDown() override;
	virtual void cUIInventory::OnMouseLBUp() override;
	virtual void cUIInventory::Drag() override;
	
	virtual cUIObject* cUIInventory::FindFocusSlot(cUIObject* UIRoot);		//���콺 Ŀ�� �Ʒ��� �ִ� ������ ã�´�.

	virtual void cUIInventory::AddItem(cDataItem* pDataItem);
	virtual void cUIInventory::SetSlot(size_t nRowQnt, size_t nColQnt);		//������ ���ڸ�ŭ �����ϰ� ��ġ�Ѵ�.
};

