#include "stdafx.h"
#include "cUIInventory.h"
#include "cUIImageView.h"
#include "cDataItem.h"

//TODO:
//화면 가장자리 근처에서는 자석같은 느낌으로
//열때 닫을때 애니메이션 + 애니메이션 끝나기 전에는 상호작용은 무시
//HACK: 드래그로 화면 밖으로 이동을 시도 했을때 마우스가 원위치로 돌아오기 전에 가장자리에서 떨어져야 더 반응성이 좋을 것 같다.

//바인딩 아이콘에 대해서 완료 해야한다.

cUIInventory::cUIInventory()
	: m_isDragging(false)
	, m_pUIPopupWindowDelegate(nullptr)
	, m_isKeyHold(false)
	, m_pImageViewMouseOver(nullptr)
	, m_pImageViewMouseDown(nullptr)
	, m_isMouseOverVisible(false)
	, m_isMouseDownVisible(false)
	, m_pFocusSlot(nullptr)
	, m_eDragState(E_NODRAG)
	, m_pBindingIcon(nullptr)
{

}

cUIInventory::~cUIInventory()
{
	SAFE_RELEASE(m_pSprite);
	
	SAFE_RELEASE(m_pImageViewMouseOver);
	SAFE_RELEASE(m_pImageViewMouseDown);

	//아이템 모두 해제
	for (size_t i = 0; i < m_vecOwnItem.size(); ++i)
	{
		SAFE_RELEASE(m_vecOwnItem[i]);
	}
	//슬롯은 어디서 해제하지? Parent->Child 구조에 포함되어 있나보다
	/*for (size_t i = 0; i < m_vecUISlot.size(); ++i)
	{
		SAFE_RELEASE(m_vecUISlot[i]);
	}*/
	//SAFE_RELEASE(m_pIconDragSrc);
	//SAFE_RELEASE(m_pBindingIcon);
	SAFE_RELEASE(m_pBindingIcon);
}

void cUIInventory::Setup()
{
	//단축키 할당
	//SetHotKey('I');
	//델리게이트 설정
	SetUIPopupWindowDelegate(this);
	//부모 cUIPopupWindow의 멤버. 팝업 되었는지 아닌지 판단하고 그리기를 결정한다.
	SetisPopped(false);
	
	//화면의 클라이언트 영역을 구한다
	RECT clientRect;
	GetClientRect(g_hWnd, &clientRect);
	//스프라이트 생성
	D3DXCreateSprite(g_pD3DDevice, &m_pSprite);
	
	m_vecOwnItem.resize(10);//소지하고 있는 아이템 갯수(종류가 되어야함)


#pragma region 인벤토리 바탕(UIRoot) 초기화
	//인벤토리 바탕(UIRoot)
	cUIImageView* pImageView = new cUIImageView(m_pSprite);
	pImageView->SetTextureFilename(std::string("../Resources/UI/UI_INVENTORY/BG_Inventory.tga"));

	//실제 그려질 크기. 이미지 파일의 크기와 실제 이미지인 부분이 다르기 때문에 직접 지정해준다.
	RECT drawArea = { 0, 0, 472, 436 };
	pImageView->SetDrawArea(drawArea);
	//위치(x, y)
	pImageView->SetPosition(D3DXVECTOR3(50, 50, 0));
	pImageView->SetAlpha(0);
	pImageView->SetScale(D3DXVECTOR3(1.f, 1.f, 1.f));
	SetUIRoot(pImageView);
	//m_pUIRoot = pImageView;

	//드래그되기 전 좌표를 현재 좌표로 초기화
	SetBeforeDragPos(m_pUIRoot->GetPosition());
#pragma endregion

	//HACK: 얘네를 인벤토리의 Child로 바꿔야함
	//마우스 오버시 보일 테두리
	m_pImageViewMouseOver = new cUIImageView(m_pSprite);
	m_pImageViewMouseOver->Setup(
		std::string("../Resources/UI/UI_INVENTORY/Component_I352.tga"),
		56, 56,
		D3DXVECTOR3(0, 0, 0),
		D3DXVECTOR3(1.f, 1.f, 1.f));
	
	//마우스 클릭시 보일 테두리
	m_pImageViewMouseDown = new cUIImageView(m_pSprite);
	m_pImageViewMouseDown->Setup(
		std::string("../Resources/UI/UI_INVENTORY/Component_I355.tga"),
		56, 56,
		D3DXVECTOR3(0, 0, 0),
		D3DXVECTOR3(1.f, 1.f, 1.f));

	//슬롯들 배치
	SetupSlot(5, 8);

	//기능 테스트용 초기화
	SetupTest();
	
}
void cUIInventory::Update(float fDelta)
{
	//단축키가 최초로 눌러지는 입력인지 검사한다. 키 반복입력 방지용
	if (!g_pControlManager->GetInputInfo(GetHotKey()))
	{
		m_isKeyHold = false;
	}
	//아이템 추가
	if (g_pControlManager->GetInputInfo('C'))
	{
		AddItem(m_vecOwnItem[1]);
	}
	//아이템 이동 시험
	if (g_pControlManager->GetInputInfo('V'))
	{
		SwapItemInSlot(m_vecUISlot[0], m_vecUISlot[39]);
	}
	//창이 열려 있을 때
	if (GetisPopped())
	{
		m_pFocusSlot = (cUISlot*)FindFocusSlot(m_vecUISlot);
		//마우스 오버/다운된 곳 하이라이트
		HighLightFocusSlot();

#pragma region 알파블렌딩 효과
		//임시 효과(기능을 다 구현하고 제대로 만든다)
		cUIImageView* pImageView = (cUIImageView*)m_pUIRoot;
		float fAlpha = pImageView->GetAlpha();
		if (fAlpha < 1.f)
		{
			//fAlpha += 0.05f;
			fAlpha += fDelta*5;
			pImageView->SetAlpha(fAlpha);
			pImageView->SetScale(D3DXVECTOR3(1.f, 1.f, 1.f));

			m_pImageViewMouseOver->SetAlpha(fAlpha);
			m_pImageViewMouseDown->SetAlpha(fAlpha);
		}

		if (m_isKeyHold == false && g_pControlManager->GetInputInfo(GetHotKey()))
		{
			SetisPopped(false);
			m_isKeyHold = true;
			fAlpha = 0.f;
			pImageView->SetAlpha(fAlpha);
			pImageView->SetScale(D3DXVECTOR3(fAlpha, fAlpha, 1.f));

			m_pImageViewMouseOver->SetAlpha(fAlpha);
			m_pImageViewMouseDown->SetAlpha(fAlpha);
			
		}
#pragma endregion

		//왼버튼Down시
		if (g_pControlManager->GetInputInfo(VK_LBUTTON)){ OnMouseLBDown(); }
		//왼버튼Up시
		else if (!g_pControlManager->GetInputInfo(VK_LBUTTON)){ OnMouseLBUp(); }
		//오른쪽버튼Down시
		if (g_pControlManager->GetInputInfo(VK_RBUTTON)){ OnMouseRBDown(); }
		//오른쪽버튼Up시
		else if (!g_pControlManager->GetInputInfo(VK_RBUTTON)){ OnMouseRBUp(); }
		
		//드래그 중이면 드래그 행동
		if (m_isDragging == true) { Drag(m_eDragState); }

		if (m_pUIRoot)
		{
			m_pUIRoot->Update(fDelta);
		}
		/*if (m_pBindingIcon)
		{
			m_pBindingIcon->SetPosition(g_pControlManager->GetCurrentCursorPosition().x, g_pControlManager->GetCurrentCursorPosition().y);
			m_pBindingIcon->Update(fDelta);
		}*/
		m_pImageViewMouseOver->Update(fDelta);
		m_pImageViewMouseDown->Update(fDelta);
	}
	//창이 열려 있지 않을 때
	else if (!GetisPopped())
	{
		if (m_isKeyHold == false && g_pControlManager->GetInputInfo(GetHotKey()))
		{
			SetisPopped(true);
			m_isKeyHold = true;
		}
	}
}

void cUIInventory::Render()
{
	if (m_bShow){
		if (m_pSprite)
		{
			m_pUIRoot->Render();
		}

		if (m_isMouseOverVisible) { m_pImageViewMouseOver->Render(); }
		if (m_isMouseDownVisible) { m_pImageViewMouseDown->Render(); }
		if (m_pBindingIcon){ m_pBindingIcon->Render(); }
	}
}

//빈칸(nullptr)을 찾아서 아이템을 추가하고 슬롯에 아이템에 해당되는 아이콘을 등록한다.
void cUIInventory::AddItem(cDataItem* pDataItem)
{
	LPD3DXSPRITE pSprite = nullptr;
	D3DXCreateSprite(g_pD3DDevice, &pSprite);

	for (size_t i = 0; i < m_vecUISlot.size(); ++i)
	{
		if (!m_vecUISlot[i]->GetItem())
		{			
			m_vecUISlot[i]->SetItem(pDataItem);
			break;
		}
	}
	SAFE_RELEASE(pSprite);
}
//슬롯 번호로 삭제
void cUIInventory::DeleteItemInSlot(size_t nSlotNum)
{
	DeleteItemInSlot(m_vecUISlot[nSlotNum]);
}
//슬롯 포인터로
void cUIInventory::DeleteItemInSlot(cUISlot* pUISlot)
{
	pUISlot->DeleteItem();
}
void cUIInventory::OnMouseLBDown()
{
	//왼마우스가 다운되는걸 판별해서 드래그로 전환하려는 목적의메서드 이므로 드래그중이면 탈출
	if (m_isDragging) { return; }

	////몸체를 클릭 했을때 WindowDrag로 전환되는 트리거
	if (m_pFocusSlot == nullptr && m_isDragging == false)
	{
		RECT rc;
		cUIImageView* pUIImageView = (cUIImageView*)m_pUIRoot;
		SetRect(&rc,
			(int)m_pUIRoot->GetPosition().x,
			(int)m_pUIRoot->GetPosition().y,
			(int)m_pUIRoot->GetPosition().x + pUIImageView->GetDrawArea().right,
			(int)m_pUIRoot->GetPosition().y + pUIImageView->GetDrawArea().bottom);
		POINT ptMouse = g_pControlManager->GetCurrentCursorPosition();
		
		//사각영역안 점 확인
		if (PtInRect(&rc, ptMouse))
		{
			//드래그 시작
			m_isDragging = true;
			SetDragState(E_WINDOW_DRAG);
		}
	}
	
	//슬롯을 클릭 했을때 IconDrag로 전환되는 트리거
	if (m_pFocusSlot && m_isDragging == false)
	{
		//슬롯에 포커스가 있을때 마우스 다운 하이라이트
		SetIsMouseOverVisible(false);
		SetIsMouseDownVisible(true);
		m_pImageViewMouseDown->SetPosition(m_pUIRoot->GetPosition() + m_pFocusSlot->GetPosition());

		//현재 클릭된 슬롯에 아이템이 존재하는지 확인
		bool isItemExist;
		if (m_pFocusSlot->GetItem()){ isItemExist = true; }
		else { isItemExist = false; }

		//(아이템이 있으면) 마우스 다운된 슬롯에 아이템이 있어야 드래그로 전환
		if (isItemExist)
		{
			m_isDragging = true;
			SetDragState(E_ICON_DRAG);
			//드래그가 시작된 슬롯을 기억
			m_pSlotDragSrc = m_pFocusSlot;


			//m_pBindingIcon = new cUIIcon;
			//LPD3DXSPRITE pSprite = nullptr;
			//D3DXCreateSprite(g_pD3DDevice, &pSprite);

			//cUIIcon* pUIIcon = new cUIIcon(pSprite);

			//pUIIcon->Setup(
			//	m_pSlotDragSrc->GetTextureFilename(),
			//	56, 56,
			//	D3DXVECTOR3(4, 4, 0),
			//	D3DXVECTOR3(.8f, .8f, .8f));

			////m_pBindingIcon = m_pSlotDragSrc->GetIcon();
			//m_pBindingIcon->SetPosition(D3DXVECTOR3(
			//	g_pControlManager->GetCurrentCursorPosition().x,
			//	g_pControlManager->GetCurrentCursorPosition().y,
			//	0));

			//드래그 시작 아이콘을 기억
			//m_pIconDragSrc = m_pFocusSlot->GetIcon();
		}
		//아이템이 없으면
		else if (!isItemExist)
		{
			m_isDragging = true;
			SetDragState(E_EMPTY_DRAG);
		}
	}
}
void cUIInventory::OnMouseLBUp()
{
	if (m_eDragState == E_ICON_DRAG)
	{
		if (m_pFocusSlot)
		{
			SwapItemInSlot(m_pSlotDragSrc, m_pFocusSlot);
		}
	}
	if (m_pFocusSlot)
	{
		//왼버튼은 Up이지만 오른버튼이 Down일경우 눌러진 이미지를 유지해야 하므로 검사
		if (!g_pControlManager->GetInputInfo(VK_RBUTTON))
		{
			m_isMouseOverVisible = true;
			m_isMouseDownVisible = false;
		}
	}
	m_isDragging = false;
	m_eDragState = E_NODRAG;
	SetBeforeDragPos(m_pUIRoot->GetPosition());//?
}
void cUIInventory::OnMouseRBDown()
{
	//슬롯에 포커스가 있을때
	if (m_pFocusSlot)
	{
		SetIsMouseOverVisible(false);
		SetIsMouseDownVisible(true);
		m_pImageViewMouseDown->SetPosition(m_pUIRoot->GetPosition() + m_pFocusSlot->GetPosition());

		if (m_pFocusSlot->GetItem())
		{
			UseItem(m_pFocusSlot);
		}
	}
}
void cUIInventory::OnMouseRBUp()
{
	if (m_pFocusSlot)
	{
		//오른버튼은 Up이지만 왼버튼이 Down일경우 눌러진 이미지를 유지해야 하므로 검사
		if (!g_pControlManager->GetInputInfo(VK_LBUTTON))
		{
			SetIsMouseOverVisible(true);
			SetIsMouseDownVisible(false);
		}
	}
}
void cUIInventory::Drag(EDRAGSTATE eDragState)
{
	if (m_isDragging == false) { return; }

	if (eDragState == E_WINDOW_DRAG)
	{
		POINT ptClicked = g_pControlManager->GetLClickedCursorPosition();
		POINT ptCurrent = g_pControlManager->GetCurrentCursorPosition();
		//D3DXVECTOR3 vCurrRootPos = m_pUIRoot->GetPosition();
		D3DXVECTOR3 vBeforeDragPos = GetBeforeDragPos();

		RECT rtClientRect;
		GetClientRect(g_hWnd, &rtClientRect);

		//클릭된 지점과 현재 마우스 좌표간의 차이값
		POINT ptCursorOffset;
		ptCursorOffset.x = ptCurrent.x - ptClicked.x;
		ptCursorOffset.y = ptCurrent.y - ptClicked.y;

		//UI의 목표 좌표(실제 위치가 될 좌표)
		POINT ptUIDestPos;
		ptUIDestPos.x = (int)vBeforeDragPos.x + (int)ptCursorOffset.x;
		ptUIDestPos.y = (int)vBeforeDragPos.y + (int)ptCursorOffset.y;

		//상하좌우 화면밖을 벗어나지 못하게 처리해준다
		if (ptUIDestPos.x < 0) { ptUIDestPos.x = 0; }
		else if (ptUIDestPos.x + m_pUIRoot->GetDrawArea().right > rtClientRect.right)
		{
			ptUIDestPos.x = rtClientRect.right - m_pUIRoot->GetDrawArea().right;
		}
		if (ptUIDestPos.y < 0) { ptUIDestPos.y = 0; }
		else if (ptUIDestPos.y + m_pUIRoot->GetDrawArea().right > rtClientRect.bottom)
		{
			ptUIDestPos.y = rtClientRect.bottom - m_pUIRoot->GetDrawArea().bottom;
		}

		//목표 좌표로 세팅 해준다
		m_pUIRoot->SetPosition(D3DXVECTOR3(
			(float)ptUIDestPos.x,
			(float)ptUIDestPos.y,
			0));
	}
	else if (eDragState == E_ICON_DRAG)
	{
	/*m_pBindingIcon->SetPosition(D3DXVECTOR3(
				g_pControlManager->GetCurrentCursorPosition().x,
				g_pControlManager->GetCurrentCursorPosition().y,
				0));	*/
	}
	else if (eDragState == E_EMPTY_DRAG)
	{

	}
}

cUIObject* cUIInventory::FindFocusSlot(std::vector<cUISlot*>& vecUISlot)
{
	//빈슬롯에 아이콘 드래그가 나오면 이것을 어떻게 처리할 것인가

	if (m_eDragState == E_WINDOW_DRAG) { return nullptr; }	//창을 드래그 하는 중이면 슬롯을 검색할 일이 없으므로 탈출
	else if (m_eDragState == E_EMPTY_DRAG) { return nullptr; }	//빈슬롯을 드래그 하는 중이면 슬롯을 검색할 일이 없으므로 탈출
	RECT rtArea;
	for (size_t i = 0; i < vecUISlot.size(); ++i)
	{
		//HACK: 이거 꼭 Root의 좌표를 구해와야하나.. 더 좋은 방법이 있을 것 같다. : 민우
		//Invectory Root로부터 슬롯의 절대좌표를 구해내는 과정임
		rtArea = {
			(LONG)m_pUIRoot->GetPosition().x + (LONG)vecUISlot[i]->GetPosition().x,
			(LONG)m_pUIRoot->GetPosition().y + (LONG)vecUISlot[i]->GetPosition().y,
			(LONG)m_pUIRoot->GetPosition().x + (LONG)vecUISlot[i]->GetPosition().x + vecUISlot[i]->GetDrawArea().right,
			(LONG)m_pUIRoot->GetPosition().y + (LONG)vecUISlot[i]->GetPosition().y + vecUISlot[i]->GetDrawArea().bottom
		};
		if (PtInRect(&rtArea, g_pControlManager->GetCurrentCursorPosition()))
		{
			return vecUISlot[i];
		}
	}
	return nullptr;
}
void cUIInventory::SetupSlot(size_t nRowQnt, size_t nColQnt)
{
	LPD3DXSPRITE pSprite = nullptr;
	D3DXCreateSprite(g_pD3DDevice, &pSprite);
	//슬롯들
	//IntersectRect();//사각충돌체크 함수
	//PtInRect(&rc, ptMouse);//사각영역안 점 확인
	for (size_t row = 0; row < nRowQnt; ++row)
	{
		for (size_t col = 0; col < nColQnt; ++col)
		{
			cUISlot* pUISlot;
			pUISlot = new cUISlot(pSprite);	//AddRef
			pUISlot->SetTextureFilename(std::string("../Resources/UI/UI_INVENTORY/Component_I341.tga"));
			RECT rtDrawArea;
			rtDrawArea = { 0, 0, 56, 56 };
			pUISlot->SetDrawArea(rtDrawArea);
			pUISlot->SetPosition(D3DXVECTOR3(
				16 + ((float)col * pUISlot->GetDrawArea().right)/*+(i*3)*/,
				112 + ((float)row * pUISlot->GetDrawArea().bottom),
				0));
			pUISlot->SetNum(nColQnt*row + col);

			m_pUIRoot->AddChild(pUISlot);	//AddRef
			m_vecUISlot.push_back(pUISlot);
			SAFE_RELEASE(pUISlot);
		}
	}
	SAFE_RELEASE(pSprite);
}
cDataItem* cUIInventory::UseItem(cUISlot* pUISlot)
{
	//일단 사용시도된 아이템을 가지고있고
	cDataItem* pUsedItem = pUISlot->GetItem();
	//TODO: 사용가능한 아이템인지 체크하고
	//가능하면
	if (true)
	{
		//슬롯을 비우고
		DeleteItemInSlot(pUISlot);
		//사용된 아이템을 반환한다
		return pUsedItem;
	}
	//불가능하면 nullptr 리턴
	return nullptr;
}
void cUIInventory::SwapItemInSlot(cUISlot* pSrcSlot, cUISlot* pDestSlot)
{
	//nullptr이면 nullptr인대로 서로 교환한다. 문제가 생기기전엔 이대로 한다.

	cDataItem* pDataItem = nullptr;
	pDataItem = pDestSlot->GetItem();
	DeleteItemInSlot(pDestSlot);

	pDestSlot->SetItem(pSrcSlot->GetItem());
	//pDestSlot->SetIcon(pDestSlot->GetItem()->GetUIIconImagePath());
	DeleteItemInSlot(pSrcSlot);

	pSrcSlot->SetItem(pDataItem);
}
void cUIInventory::HighLightFocusSlot()
{
	//마우스 오버 슬롯 테두리 위치를 포커싱된 슬롯에 씌운다(이동한다)
		if (m_pFocusSlot)
		{
			SetIsMouseOverVisible(true);
			m_pImageViewMouseOver->SetPosition(m_pUIRoot->GetPosition() + m_pFocusSlot->GetPosition());
		}
		else
		{
			SetIsMouseOverVisible(false);
			SetIsMouseDownVisible(false);
		}
}
void cUIInventory::SetupTest()
{
#pragma region 아이템 테스트 세팅

	cDataItem* pDataItem = pDataItem = new cDataItem;
	pDataItem->SetUIIconImagePath("../Resources/ICON/ICON_Item/HP_Potion_Level_33_Tex.tga");
	m_vecOwnItem[0] = pDataItem;

	pDataItem = new cDataItem;
	pDataItem->SetUIIconImagePath("../Resources/ICON/ICON_Item/MP_Potion_Level_33_Tex.tga");
	m_vecOwnItem[1] = pDataItem;

	m_vecUISlot[0]->SetItem(m_vecOwnItem[0]);
	m_vecUISlot[39]->SetItem(m_vecOwnItem[1]);
#pragma endregion

	
}