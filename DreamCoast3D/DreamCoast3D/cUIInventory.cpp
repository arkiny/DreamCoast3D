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
{

}


cUIInventory::~cUIInventory()
{
	SAFE_RELEASE(m_pSprite);
}

void cUIInventory::Setup()
{
	
	//델리게이트 설정
	SetUIPopupWindowDelegate(this);
	//부모 cUIPopupWindow의 멤버. 팝업 되었는지 아닌지 판단하고 그리기를 결정한다.
	SetisPopped(true);
	
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

	//찬칸
	pImageView = new cUIImageView(m_pSprite);
	pImageView->SetTextureFilename(std::string("../Resources/ICON/ICON_Item/HP_Potion_Level_33_Tex.tga"));
	drawArea = { 0, 0, 64, 64};
	pImageView->SetDrawArea(drawArea);
	pImageView->SetScale(D3DXVECTOR3(0.85f, 0.85f, 0.85f));
	pImageView->SetPosition(D3DXVECTOR3(16, 85, 0));

	m_pUIRoot->AddChild(pImageView);
	SAFE_RELEASE(pImageView);

	//빈칸들
	//IntersectRect();//사각충돌체크 함수
	//PtInRect(&rc, ptMouse);//사각영역안 점 확인
	for (int row = 0; row < 8; ++row)
	{
		for (int col = 0; col < 6; ++col)
		{
			pImageView = new cUIImageView(m_pSprite);
			pImageView->SetTextureFilename(std::string("../Resources/UI/UI_INVENTORY/Component_I35B.tga"));
			drawArea = { 0, 0, 55, 55 };
			pImageView->SetDrawArea(drawArea);

			pImageView->SetPosition(D3DXVECTOR3(16 + (row * 55)/*+(i*3)*/, 85+(col*55), 0));
			
			m_pUIRoot->AddChild(pImageView);
			SAFE_RELEASE(pImageView);
		}
	}	
}
void cUIInventory::Update(float fDelta)
{
	if (g_pControlManager->GetInputInfo(VK_LBUTTON))
	{
		//if (!m_pUIPopupWindowDelegate) { return; }
		m_pUIPopupWindowDelegate->OnMouseLBDown();
	}
	else if (!g_pControlManager->GetInputInfo(VK_LBUTTON))
	{
		m_pUIPopupWindowDelegate->OnMouseLBUp();
	}
	if (m_isDragging == true) { m_pUIPopupWindowDelegate->Drag(); }
	
	if (m_bShow){
		if (m_pUIRoot)
			m_pUIRoot->Update(fDelta);
	}
}
/*
void cUIInventory::Render()
{
	if (m_bShow){
		if (m_pSprite)
		{
			m_pUIRoot->Render();
		}
	}
}
*/
void cUIInventory::AddItem(cUIIcon& UIIcon)
{
	//TODO: 여기서 아이콘 타입을 검사하고 아이템이면 인벤토리에 추가한다.
	m_vecUIIcon.push_back(UIIcon);
}
void cUIInventory::OnMouseLBDown()
{
	if (m_isDragging == false)
	{
		RECT rc;
		cUIImageView* pUIImageView = (cUIImageView*)m_pUIRoot;
		SetRect(&rc,
			m_pUIRoot->GetPosition().x,
			m_pUIRoot->GetPosition().y,
			m_pUIRoot->GetPosition().x + pUIImageView->GetDrawArea().right,
			m_pUIRoot->GetPosition().y + pUIImageView->GetDrawArea().bottom);
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
	ptUIDestPos.x = vBeforeDragPos.x + ptCursorOffset.x;
	ptUIDestPos.y = vBeforeDragPos.y + ptCursorOffset.y;

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
		ptUIDestPos.x,
		ptUIDestPos.y,
		0));
}