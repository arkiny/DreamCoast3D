#pragma once
#include "cUIWindow.h"
#include "cUIPopupWindow.h"
#include "iUIPopupWindowDelegate.h"
#include "cUIIcon.h"
class cUIImageView;

class cUIInventory : public cUIPopupWindow, public iUIPopupWindowDelegate
{
protected:
	cUIImageView*	m_pImageViewMouseOver;	//마우스 오버 되었을때 씌워질 테두리
	bool			m_isMouseOverVisible;	//마우스 오버 테두리 출력 여부 결정
	cUIImageView*	m_pImageViewMouseDown;	//마우스 다운 되었을때 씌워질 테두리
	bool			m_isMouseDownVisible;	//마우스 다운 테두리 출력 여부 결정
	cUIImageView*	m_pFocusSlot;			//현재 하이라이트 된 슬롯
	//bool m_isEnable;					//활성화 되었는가
	//bool m_isVisible;					//보이는 중인가(cUIPopupWindow bShow가 대신한다)
	bool m_isDragging;					//드래그 되는 중인가
	bool m_isKeyHold;					//키가 눌러진 중인가

	std::vector<cUIIcon> m_vecUIIcon;	//인벤토리 안에 들어 있는 아이콘 벡터 컨테이너
	//SYNTHESIZE(iUIPopupWindowDelegate*, m_pUIPopupWindowDelegate, UIPopupWindowDelegate);
	SYNTHESIZE(iUIPopupWindowDelegate*, m_pUIPopupWindowDelegate, UIPopupWindowDelegate);
	SYNTHESIZE(D3DXVECTOR3, m_vBeforeDragPos, BeforeDragPos);
	SYNTHESIZE(char, m_chHotKeyCode, HotKey);//단축키(아스키)
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

