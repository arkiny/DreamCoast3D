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
	enum EDRAGSTATE
	{
		E_NODRAG,
		E_EMPTY_DRAG,
		E_ICON_DRAG,
		E_WINDOW_DRAG,
		E_DRAG_MAX
	};
	cUIImageView*	m_pImageViewMouseOver;	//마우스 오버 되었을때 씌워질 테두리
	cUIImageView*	m_pImageViewMouseDown;	//마우스 다운 되었을때 씌워질 테두리

	std::vector<cDataItem*> m_vecOwnItem;	//인벤토리 안에 들어 있는 아이템 벡터 컨테이너

	SYNTHESIZE(bool, m_isDragging, IsDragging);	//드래그 되는 중인가
	SYNTHESIZE(bool, m_isKeyHold, IsKeyHold);	//키가 눌러진 중인가
	SYNTHESIZE(iUIPopupWindowDelegate*, m_pUIPopupWindowDelegate, UIPopupWindowDelegate);
	SYNTHESIZE(D3DXVECTOR3, m_vBeforeDragPos, BeforeDragPos); //드래그 할 때 마우스를 떼기 전 처음 시작된 위치
	SYNTHESIZE(char, m_chHotKeyCode, HotKey);		//단축키(아스키)
	SYNTHESIZE(cUISlot*, m_pFocusSlot, FocusSlot);	//현재 하이라이트 된 슬롯
	SYNTHESIZE(std::vector<cUISlot*>, m_vecUISlot, vecUISlot);//인벤토리의 각 칸
	SYNTHESIZE(EDRAGSTATE, m_eDragState, DragState);	//현재 드래그 상태
	SYNTHESIZE(bool, m_isMouseOverVisible, IsMouseOverVisible);//마우스 오버 테두리 출력 여부 결정
	SYNTHESIZE(bool, m_isMouseDownVisible, IsMouseDownVisible);//마우스 다운 테두리 출력 여부 결정
	SYNTHESIZE(cUIIcon*, m_pIconDragSrc, IconDragSrc);	//드래그가 시작된 대상 아이콘 저장용
	SYNTHESIZE(cUIIcon*, m_pBindingIcon, BindingIcon);	//드래그중 마우스에 붙어 있는 아이콘
	SYNTHESIZE(cUISlot*, m_pSlotDragSrc, SlotDragSrc);	//슬롯 드래그가 시작된 슬롯
	SYNTHESIZE(cUISlot*, m_pSlotDragDest, SlotDragDest);//슬롯 드래그가 완료될 슬롯

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

	virtual void cUIInventory::Drag() override {}
	virtual void cUIInventory::Drag(EDRAGSTATE eDragState);
	virtual cUIObject* cUIInventory::FindFocusSlot(std::vector<cUISlot*>& vecUISlot);		//마우스 커서 아래에 있는 슬롯을 찾는다.

	void cUIInventory::SetupTest();	//기능 테스트용
	virtual void cUIInventory::AddItem(cDataItem* pDataItem) override;		//아이템 추가
	virtual void cUIInventory::DeleteItemInSlot(size_t nSlotNum) override;	//슬롯 번호로 아이템 삭제
	virtual void cUIInventory::DeleteItemInSlot(cUISlot* pUISlot) override;	//슬롯 포인터로 아이템 삭제
	virtual cDataItem* cUIInventory::UseItem(cUISlot* pUISlot);				//아이템 사용시
	virtual void cUIInventory::SetupSlot(size_t nRowQnt, size_t nColQnt);	//슬롯을 인자만큼 생성하고 배치한다.
	virtual void cUIInventory::SwapItemInSlot(cUISlot* pSrcSlot, cUISlot* pDestSlot);	//슬롯간 아이템 교환
	virtual void cUIInventory::HighLightFocusSlot();						//마우스 오버/다운 된 슬롯 하이라이트 시키기
};

