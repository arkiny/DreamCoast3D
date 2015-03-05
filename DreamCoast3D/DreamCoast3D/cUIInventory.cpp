#include "stdafx.h"
#include "cUIInventory.h"
#include "cUIImageView.h"

//TODO:
//화면 밖으로 못벗어나게 하기
//화면 가장자리 근처에서는 자석같은 느낌으로
//열때 닫을때 애니메이션 + 애니메이션 끝나기 전에는 상호작용은 무시
//HACK: 드래그로 화면 밖으로 이동을 시도 했을때 마우스가 원위치로 돌아오기 전에 가장자리에서 떨어져야 더 반응성이 좋을 것 같다.

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
	
	//인벤토리 바탕
	cUIImageView* pImageView = new cUIImageView(m_pSprite);
	pImageView->SetTextureFilename(std::string("../Resources/UI/UI_INVENTORY/BG_Inventory.tga"));
	pImageView->SetScale(D3DXVECTOR3(1.f, 1.f, 1.0f));
	
	//실제 그려질 크기. 이미지 파일의 크기와 실제 이미지인 부분이 다르기 때문에 직접 지정해준다.
	RECT drawArea = { 0, 0, 472, 436 };
	pImageView->SetDrawArea(drawArea);
	//위치(x, y)
	pImageView->SetPosition(D3DXVECTOR3(50, 50, 0));
	m_pUIRoot = pImageView;

	//드래그되기 전 좌표를 현재 좌표로 초기화
	SetBeforeDragPos(m_pUIRoot->GetPosition());

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

	//pImageView->SetScale(D3DXVECTOR3(1.f, 1.f, 1.0f));
	//
	////실제 그려질 크기. 이미지 파일의 크기와 실제 이미지인 부분이 다르기 때문에 직접 지정해준다.
	//RECT drawArea = { 0, 0, 472, 436 };
	//pImageView->SetDrawArea(drawArea);
	////위치(x, y)
	//pImageView->SetPosition(D3DXVECTOR3(50, 50, 0));
	//m_pUIRoot = pImageView;


	////찬칸
	//pImageView = new cUIImageView(m_pSprite);
	//pImageView->SetTextureFilename(std::string("../Resources/ICON/ICON_Item/HP_Potion_Level_33_Tex.tga"));
	//drawArea = { 0, 0, 64, 64};
	//pImageView->SetDrawArea(drawArea);
	//pImageView->SetScale(D3DXVECTOR3(0.85f, 0.85f, 0.85f));
	//pImageView->SetPosition(D3DXVECTOR3(16, 85, 0));

	//m_pUIRoot->AddChild(pImageView);
	//SAFE_RELEASE(pImageView);

	//빈칸들
	//IntersectRect();//사각충돌체크 함수
	//PtInRect(&rc, ptMouse);//사각영역안 점 확인
	for (int row = 0; row < 8; ++row)
	{
		for (int col = 0; col < 5; ++col)
		{
			pImageView = new cUIImageView(m_pSprite);
			pImageView->SetTextureFilename(std::string("../Resources/UI/UI_INVENTORY/Component_I341.tga"));
			//pImageView->SetTextureFilename(std::string("../Resources/UI/UI_INVENTORY/Component_I35B.tga"));
			drawArea = { 0, 0, 56, 56 };
			pImageView->SetDrawArea(drawArea);

			pImageView->SetPosition(D3DXVECTOR3(
				16 + ((float)row*pImageView->GetDrawArea().right)/*+(i*3)*/,
				112 + ((float)col*pImageView->GetDrawArea().bottom),
				0));
			
			m_pUIRoot->AddChild(pImageView);
			SAFE_RELEASE(pImageView);
		}
	}	
}
void cUIInventory::Update(float fDelta)
{
	//단축키가 최초로 눌러지는 입력인지 검사한다. 키 반복입력 방지용
	if (!g_pControlManager->GetInputInfo(GetHotKey()))
	{
		m_isKeyHold = false;
	}
	//창이 열려 있을 때
	if (GetisPopped())
	{
		if (m_isKeyHold == false && g_pControlManager->GetInputInfo(GetHotKey()))
		{
			SetisPopped(false);
			m_isKeyHold = true;
		}
		//마우스 오버 슬롯 테두리
		m_pFocusSlot = (cUIImageView*)FindFocusSlot(m_pUIRoot);
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

		//왼클릭Down시
		if (g_pControlManager->GetInputInfo(VK_LBUTTON))
		{
			OnMouseLBDown();
		}
		//왼클릭Up시
		else if (!g_pControlManager->GetInputInfo(VK_LBUTTON))
		{
			OnMouseLBUp();
		}
		//드래그 중이면 드래그 행동
		if (m_isDragging == true) { Drag(); }

		if (m_pUIRoot)
		{
			m_pUIRoot->Update(fDelta);
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

void cUIInventory::AddItem(cUIIcon& UIIcon)
{
	//TODO: 여기서 아이콘 타입을 검사하고 아이템이면 인벤토리에 추가한다.
	m_vecUIIcon.push_back(UIIcon);
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

cUIObject* cUIInventory::FindFocusSlot(cUIObject* pUIRoot)
{
	RECT rtArea;
	for (size_t i = 0; i < pUIRoot->GetChild().size(); ++i)
	{
		rtArea	= {
		(LONG)pUIRoot->GetPosition().x +pUIRoot->GetChild()[i]->GetPosition().x,
		(LONG)pUIRoot->GetPosition().y +pUIRoot->GetChild()[i]->GetPosition().y,
		(LONG)pUIRoot->GetPosition().x +pUIRoot->GetChild()[i]->GetPosition().x + pUIRoot->GetChild()[i]->GetDrawArea().right,
		(LONG)pUIRoot->GetPosition().y +pUIRoot->GetChild()[i]->GetPosition().y + pUIRoot->GetChild()[i]->GetDrawArea().bottom
		};
		if (PtInRect(&rtArea, g_pControlManager->GetCurrentCursorPosition()))
		{
			return pUIRoot->GetChild()[i];
		}
	}
	return nullptr;
}