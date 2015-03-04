#include "stdafx.h"
#include "cUIInventory.h"
#include "cUIImageView.h"


cUIInventory::cUIInventory()
	: m_isDragging(false)
	, m_pInventoryDelegate(nullptr)
{
	
}


cUIInventory::~cUIInventory()
{
	SAFE_RELEASE(m_pSprite);
}

void cUIInventory::Setup()
{
	
	//델리게이트 설정
	SetInventoryDelegate(this);
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
		//if (!m_pInventoryDelegate) { return; }
		m_pInventoryDelegate->OnMouseLBDown(this);
	}
	else if (!g_pControlManager->GetInputInfo(VK_LBUTTON))
	{
		m_pInventoryDelegate->OnMouseLBUp(this);
	}
	if (m_isDragging == true) { Drag(); }
	
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
void cUIInventory::OnMouseLBDown(cUIInventory* pSender)
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
			m_isDragging = true;
		}
	}
}
void cUIInventory::OnMouseLBUp(cUIInventory* pSender)
{
	m_isDragging = false;
}
void cUIInventory::OnMouseMove(cUIInventory* pSender)
{
	
}
void cUIInventory::Drag()
{
	if (m_isDragging == false) { return; }
	POINT ptClicked = g_pControlManager->GetLClickedCursorPosition();
	POINT ptCurrent = g_pControlManager->GetCurrentCursorPosition();
	m_pUIRoot->SetPosition(D3DXVECTOR3(ptCurrent.x - ptClicked.x, ptCurrent.y - ptClicked.y, 0));
	//SetPosition(D3DXVECTOR3(ptCurrent.x - ptClicked.x, ptCurrent.y - ptClicked.y, 0));
}