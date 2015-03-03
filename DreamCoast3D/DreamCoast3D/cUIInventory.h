#pragma once
#include "cUIWindow.h"
//#include "cUITextView.h"
#include "cUIIcon.h"
#include "cUIPopupWindow.h"

class cUIInventory : public cUIPopupWindow
{
protected:
	
	bool m_isEnable;					//활성화 되었는가
	bool m_isVisible;					//보이는 중인가
	std::vector<cUIIcon> m_vecUIIcon;	//인벤토리 안에 들어 있는 아이콘 벡터 컨테이너
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

