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
	
	bool m_isEnable;					//활성화 되었는가
	//bool m_isVisible;					//보이는 중인가(cUIPopupWindow bShow가 대신한다)
	bool m_isDragging;					//드래그 되는 중인가
	std::vector<cUIIcon> m_vecUIIcon;	//인벤토리 안에 들어 있는 아이콘 벡터 컨테이너
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

