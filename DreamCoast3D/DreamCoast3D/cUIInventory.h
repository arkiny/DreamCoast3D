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
	cUIImageView*	m_pImageViewMouseOver;	//마우스 오버 되었을때 씌워질 테두리
	bool			m_isMouseOverVisible;	//마우스 오버 테두리 출력 여부 결정
	cUIImageView*	m_pImageViewMouseDown;	//마우스 다운 되었을때 씌워질 테두리
	bool			m_isMouseDownVisible;	//마우스 다운 테두리 출력 여부 결정

	//std::vector<cUIIcon> m_vecUIIcon;		//인벤토리 안에 들어 있는 아이콘 벡터 컨테이너(아이템 객체가 아이콘을 가질 예정)
	std::vector<cDataItem*> m_vecOwnItem;	//인벤토리 안에 들어 있는 아이템 벡터 컨테이너
	//std::vector<cUISlot*> m_vecUISlot;		
	SYNTHESIZE(bool, m_isDragging, IsDragging);	//드래그 되는 중인가
	SYNTHESIZE(bool, m_isKeyHold, IsKeyHold);	//키가 눌러진 중인가
	SYNTHESIZE(iUIPopupWindowDelegate*, m_pUIPopupWindowDelegate, UIPopupWindowDelegate);
	SYNTHESIZE(D3DXVECTOR3, m_vBeforeDragPos, BeforeDragPos); //드래그 할 때 마우스를 떼기 전 처음 시작된 위치
	SYNTHESIZE(char, m_chHotKeyCode, HotKey);		//단축키(아스키)
	SYNTHESIZE(cUISlot*, m_pFocusSlot, FocusSlot);	//현재 하이라이트 된 슬롯
	SYNTHESIZE(std::vector<cUISlot*>, m_vecUISlot, vecUISlot);//인벤토리의 각 칸
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

	virtual void cUIInventory::Drag() override;
	
	virtual cUIObject* cUIInventory::FindFocusSlot(std::vector<cUISlot*>& vecUISlot);		//마우스 커서 아래에 있는 슬롯을 찾는다.

	void cUIInventory::SetupTest();	//기능 테스트용
	virtual void cUIInventory::AddItem(cDataItem* pDataItem) override;
	virtual void cUIInventory::DeleteItemInSlot(size_t nSlotNum) override;	//슬롯 번호로
	virtual void cUIInventory::DeleteItemInSlot(cUISlot* pUISlot) override;	//슬롯 포인터로
	virtual cDataItem* cUIInventory::UseItem(cUISlot* pUISlot);
	virtual void cUIInventory::SetupSlot(size_t nRowQnt, size_t nColQnt);	//슬롯을 인자만큼 생성하고 배치한다.
};

