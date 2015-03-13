#include "stdafx.h"
#include "cUIInventory.h"
#include "cUIImageView.h"
#include "cDataItem.h"

//TODO:
//화면 밖으로 못벗어나게 하기
//화면 가장자리 근처에서는 자석같은 느낌으로
//열때 닫을때 애니메이션 + 애니메이션 끝나기 전에는 상호작용은 무시
//HACK: 드래그로 화면 밖으로 이동을 시도 했을때 마우스가 원위치로 돌아오기 전에 가장자리에서 떨어져야 더 반응성이 좋을 것 같다.

//아이콘에 스프라이트 이미지를 가지게 한다.
//이제 짜놓은 구조를 엮어야된다.

//삭제된 슬롯에 빈칸이 되게 해야한다
//AddItem Get/Set계열 내가 커스텀 해야되겠다
//SAFE_RELEASE 확실히 제어해야한다.

cUIInventory::cUIInventory()
	: m_isDragging(false)
	, m_pUIPopupWindowDelegate(nullptr)
	, m_isKeyHold(false)
	, m_pImageViewMouseOver(nullptr)
	, m_pImageViewMouseDown(nullptr)
	, m_isMouseOverVisible(false)
	, m_isMouseDownVisible(false)
	, m_pFocusSlot(nullptr)
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
}

void cUIInventory::Setup()
{
	//단축키 할당
	SetHotKey('I');
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
	m_pImageViewMouseOver->SetTextureFilename(std::string("../Resources/UI/UI_INVENTORY/Component_I352.tga"));
	drawArea = { 0, 0, 56, 56 };
	m_pImageViewMouseOver->SetPosition(D3DXVECTOR3(0, 0, 0));

	//마우스 클릭시 보일 테두리
	m_pImageViewMouseDown = new cUIImageView(m_pSprite);
	m_pImageViewMouseDown->SetTextureFilename(std::string("../Resources/UI/UI_INVENTORY/Component_I355.tga"));
	drawArea = { 0, 0, 56, 56 };
	m_pImageViewMouseDown->SetPosition(D3DXVECTOR3(56, 0, 0));

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

	}
	//창이 열려 있을 때
	if (GetisPopped())
	{
		//마우스 오버 슬롯 테두리
		m_pFocusSlot = (cUISlot*)FindFocusSlot(GetvecUISlot());
		if (m_pFocusSlot)
		{
			m_isMouseOverVisible = true;
			m_pImageViewMouseOver->SetPosition(m_pUIRoot->GetPosition() + m_pFocusSlot->GetPosition());
		}
		else
		{
			m_isMouseOverVisible = false;
			m_isMouseDownVisible = false;
		}
		//임시 효과
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
		//왼버튼Down시
		if (g_pControlManager->GetInputInfo(VK_LBUTTON)){ OnMouseLBDown(); }
		//왼버튼Up시
		else if (!g_pControlManager->GetInputInfo(VK_LBUTTON)){ OnMouseLBUp(); }
		//오른쪽버튼Down시
		if (g_pControlManager->GetInputInfo(VK_RBUTTON)){ OnMouseRBDown(); }
		//오른쪽버튼Up시
		else if (!g_pControlManager->GetInputInfo(VK_RBUTTON)){ OnMouseRBUp(); }
		
		//드래그 중이면 드래그 행동
		if (m_isDragging == true) { Drag(); }

		if (m_pUIRoot)
		{
			m_pUIRoot->Update(fDelta);
		}
		for (size_t i = 0; i < m_vecUISlot.size(); ++i)
		{
			m_vecUISlot[i]->Update(fDelta);
			
			/*cUIIcon* pIcon = m_vecUISlot[i]->GetIcon();
			if (pIcon)
			{
				pIcon->Update(fDelta);
			}*/
		}
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
	}
}

void cUIInventory::AddItem(cDataItem* pDataItem)
{
	for (size_t i = 0; i < m_vecUISlot.size(); ++i)
	{
		if (!m_vecUISlot[i]->GetItem())
		{
			m_vecUISlot[i]->SetItem(nullptr);
			m_vecUISlot[i]->SetIcon(nullptr);
			m_vecUISlot[i]->SetItem(pDataItem);
			m_vecUISlot[i]->SetIcon(pDataItem->GetUIIcon());
			//m_vecUISlot[i]->AddChild(pDataItem->GetUIIcon());
			return;
		}
	}
}
//슬롯 번호로 삭제
void cUIInventory::DeleteItemInSlot(size_t nSlotNum)
{
	DeleteItemInSlot(m_vecUISlot[nSlotNum]);
}
//슬롯 포인터로
void cUIInventory::DeleteItemInSlot(cUISlot* pUISlot)
{
	pUISlot->SetIcon(nullptr);
	pUISlot->SetItem(nullptr);
	/*for (size_t i = 0; i < pUISlot->GetChild().size(); ++i)
	{
		pUISlot->DeleteChild(pUISlot->GetChild()[i]);
	}*/
}
void cUIInventory::OnMouseLBDown()
{
	//슬롯에 포커스가 있을때
	if (m_pFocusSlot)
	{
		m_isMouseOverVisible = false;
		m_isMouseDownVisible = true;
		m_pImageViewMouseDown->SetPosition(m_pUIRoot->GetPosition() + m_pFocusSlot->GetPosition());
	}
	//이건 몸체를 클릭 했을때 이야기
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
		}
	}
}
void cUIInventory::OnMouseLBUp()
{
	if (m_pFocusSlot)
	{
		m_isMouseOverVisible = true;
		m_isMouseDownVisible = false;
	}

	m_isDragging = false;
	SetBeforeDragPos(m_pUIRoot->GetPosition());
}
void cUIInventory::OnMouseRBDown()
{
	//슬롯에 포커스가 있을때
	if (m_pFocusSlot)
	{
		m_isMouseOverVisible = false;
		m_isMouseDownVisible = true;
		m_pImageViewMouseDown->SetPosition(m_pUIRoot->GetPosition() + m_pFocusSlot->GetPosition());
		UseItem(m_pFocusSlot);
	}
}
void cUIInventory::OnMouseRBUp()
{
	if (m_pFocusSlot)
	{
		m_isMouseOverVisible = true;
		m_isMouseDownVisible = false;
	}
}
void cUIInventory::Drag()
{
	if (m_isDragging == false) { return; }

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

cUIObject* cUIInventory::FindFocusSlot(std::vector<cUISlot*>& vecUISlot)
{
	RECT rtArea;
	for (size_t i = 0; i < vecUISlot.size(); ++i)
	{
		//HACK: 이거 꼭 Root의 좌표를 구해와야하나.. 더 좋은 방법이 있을 것 같다. : 민우
		rtArea = {
			(LONG)m_pUIRoot->GetPosition().x + vecUISlot[i]->GetPosition().x,
			(LONG)m_pUIRoot->GetPosition().y + vecUISlot[i]->GetPosition().y,
			(LONG)m_pUIRoot->GetPosition().x + vecUISlot[i]->GetPosition().x + vecUISlot[i]->GetDrawArea().right,
			(LONG)m_pUIRoot->GetPosition().y + vecUISlot[i]->GetPosition().y + vecUISlot[i]->GetDrawArea().bottom
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
	for (int row = 0; row < nRowQnt; ++row)
	{
		for (int col = 0; col < nColQnt; ++col)
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
	pUISlot->DeleteChild(pUsedItem->GetUIIcon());
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
void cUIInventory::SetupTest()
{
#pragma region 아이템 테스트 세팅
	
	LPD3DXSPRITE pSprite = nullptr;
	D3DXCreateSprite(g_pD3DDevice, &pSprite);
	//D3DXCreateSprite(g_pD3DDevice, &m_pSprite);

	cUIIcon* pUIIcon = new cUIIcon(pSprite);
	pUIIcon->SetTextureFilename(std::string("../Resources/ICON/ICON_Item/HP_Potion_Level_33_Tex.tga"));
	RECT rtDrawArea = { 0, 0, 56, 56 };
	pUIIcon->SetDrawArea(rtDrawArea);
	//pUIIcon->SetPosition(D3DXVECTOR3(m_vecUISlot[0]->GetPosition().x+4, m_vecUISlot[0]->GetPosition().y+4, 0));//FIX: 일단 하드코딩. 비율로 수정해야 한다.
	pUIIcon->SetPosition(D3DXVECTOR3(4, 4, 0));//FIX: 일단 하드코딩. 비율로 수정해야 한다.
	pUIIcon->SetScale(D3DXVECTOR3(0.8f, 0.8f, 0.8f));

	cDataItem* pDataItem = nullptr;
	pDataItem = new cDataItem(pUIIcon);
	m_vecOwnItem[0] = pDataItem;
	m_vecUISlot[0]->AddChild(pDataItem->GetUIIcon());
	m_vecUISlot[0]->SetItem(m_vecOwnItem[0]);
	m_vecUISlot[0]->SetIcon(m_vecOwnItem[0]->GetUIIcon());
	

	SAFE_RELEASE(pSprite);
	
#pragma endregion

	//작동 테스트용 0번칸 할당
	//m_vecUISlot[0]->SetItemNum(0);//0번 슬롯에 0번 아이템
	//cDataItem* pItem = m_vecOwnItem[m_vecUISlot[0]->GetItemNum()];
	//cDataItem* pItem = m_vecOwnItem[0];
	/*m_vecUISlot[0]->SetItem(pItem);
	m_vecUISlot[0]->SetIcon(pItem->GetUIIcon());*/
	//AddItem(m_vecOwnItem[0]);

}