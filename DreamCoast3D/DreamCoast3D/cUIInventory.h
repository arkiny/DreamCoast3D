#pragma once
#include "cUIWindow.h"
#include "cUIPopupWindow.h"
#include "iUIPopupWindowDelegate.h"
#include "cUIIcon.h"

class cUIInventory : public cUIPopupWindow, public iUIPopupWindowDelegate
{
protected:
	
	bool m_isEnable;					//활성화 되었는가
	//bool m_isVisible;					//보이는 중인가(cUIPopupWindow bShow가 대신한다)
	bool m_isDragging;					//드래그 되는 중인가
	std::vector<cUIIcon> m_vecUIIcon;	//인벤토리 안에 들어 있는 아이콘 벡터 컨테이너
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

