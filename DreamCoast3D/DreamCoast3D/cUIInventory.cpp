#include "stdafx.h"
#include "cUIInventory.h"
#include "cUIImageView.h"

//TODO:
//ȭ�� ������ ������� �ϱ�
//ȭ�� �����ڸ� ��ó������ �ڼ����� ��������
//���� ������ �ִϸ��̼� + �ִϸ��̼� ������ ������ ��ȣ�ۿ��� ����
//HACK: �巡�׷� ȭ�� ������ �̵��� �õ� ������ ���콺�� ����ġ�� ���ƿ��� ���� �����ڸ����� �������� �� �������� ���� �� ����.

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
	//����Ű �Ҵ�
	SetHotKey('I');
	//��������Ʈ ����
	SetUIPopupWindowDelegate(this);
	//�θ� cUIPopupWindow�� ���. �˾� �Ǿ����� �ƴ��� �Ǵ��ϰ� �׸��⸦ �����Ѵ�.
	SetisPopped(false);
	
	//ȭ���� Ŭ���̾�Ʈ ������ ���Ѵ�
	RECT clientRect;
	GetClientRect(g_hWnd, &clientRect);
	//��������Ʈ ����
	D3DXCreateSprite(g_pD3DDevice, &m_pSprite);
	
	//�κ��丮 ����
	cUIImageView* pImageView = new cUIImageView(m_pSprite);
	pImageView->SetTextureFilename(std::string("../Resources/UI/UI_INVENTORY/BG_Inventory.tga"));
	pImageView->SetScale(D3DXVECTOR3(1.f, 1.f, 1.0f));
	
	//���� �׷��� ũ��. �̹��� ������ ũ��� ���� �̹����� �κ��� �ٸ��� ������ ���� �������ش�.
	RECT drawArea = { 0, 0, 472, 436 };
	pImageView->SetDrawArea(drawArea);
	//��ġ(x, y)
	pImageView->SetPosition(D3DXVECTOR3(50, 50, 0));
	m_pUIRoot = pImageView;

	//�巡�׵Ǳ� �� ��ǥ�� ���� ��ǥ�� �ʱ�ȭ
	SetBeforeDragPos(m_pUIRoot->GetPosition());

	//���콺 ������ ���� �׵θ�
	m_pImageViewMouseOver = new cUIImageView(m_pSprite);
	m_pImageViewMouseOver->SetTextureFilename(std::string("../Resources/UI/UI_INVENTORY/Component_I352.tga"));
	drawArea = { 0, 0, 56, 56 };
	m_pImageViewMouseOver->SetPosition(D3DXVECTOR3(0, 0, 0));

	//���콺 Ŭ���� ���� �׵θ�
	
	m_pImageViewMouseDown = new cUIImageView(m_pSprite);
	m_pImageViewMouseDown->SetTextureFilename(std::string("../Resources/UI/UI_INVENTORY/Component_I355.tga"));
	drawArea = { 0, 0, 56, 56 };
	m_pImageViewMouseDown->SetPosition(D3DXVECTOR3(56, 0, 0));

	//pImageView->SetScale(D3DXVECTOR3(1.f, 1.f, 1.0f));
	//
	////���� �׷��� ũ��. �̹��� ������ ũ��� ���� �̹����� �κ��� �ٸ��� ������ ���� �������ش�.
	//RECT drawArea = { 0, 0, 472, 436 };
	//pImageView->SetDrawArea(drawArea);
	////��ġ(x, y)
	//pImageView->SetPosition(D3DXVECTOR3(50, 50, 0));
	//m_pUIRoot = pImageView;


	////��ĭ
	//pImageView = new cUIImageView(m_pSprite);
	//pImageView->SetTextureFilename(std::string("../Resources/ICON/ICON_Item/HP_Potion_Level_33_Tex.tga"));
	//drawArea = { 0, 0, 64, 64};
	//pImageView->SetDrawArea(drawArea);
	//pImageView->SetScale(D3DXVECTOR3(0.85f, 0.85f, 0.85f));
	//pImageView->SetPosition(D3DXVECTOR3(16, 85, 0));

	//m_pUIRoot->AddChild(pImageView);
	//SAFE_RELEASE(pImageView);

	//��ĭ��
	//IntersectRect();//�簢�浹üũ �Լ�
	//PtInRect(&rc, ptMouse);//�簢������ �� Ȯ��
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
	//����Ű�� ���ʷ� �������� �Է����� �˻��Ѵ�. Ű �ݺ��Է� ������
	if (!g_pControlManager->GetInputInfo(GetHotKey()))
	{
		m_isKeyHold = false;
	}
	//â�� ���� ���� ��
	if (GetisPopped())
	{
		if (m_isKeyHold == false && g_pControlManager->GetInputInfo(GetHotKey()))
		{
			SetisPopped(false);
			m_isKeyHold = true;
		}
		//���콺 ���� ���� �׵θ�
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

		//��Ŭ��Down��
		if (g_pControlManager->GetInputInfo(VK_LBUTTON))
		{
			OnMouseLBDown();
		}
		//��Ŭ��Up��
		else if (!g_pControlManager->GetInputInfo(VK_LBUTTON))
		{
			OnMouseLBUp();
		}
		//�巡�� ���̸� �巡�� �ൿ
		if (m_isDragging == true) { Drag(); }

		if (m_pUIRoot)
		{
			m_pUIRoot->Update(fDelta);
		}
		m_pImageViewMouseOver->Update(fDelta);
		m_pImageViewMouseDown->Update(fDelta);
	}
	//â�� ���� ���� ���� ��
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
	//TODO: ���⼭ ������ Ÿ���� �˻��ϰ� �������̸� �κ��丮�� �߰��Ѵ�.
	m_vecUIIcon.push_back(UIIcon);
}
void cUIInventory::OnMouseLBDown()
{
	//���Կ� ��Ŀ���� ������
	if (m_pFocusSlot)
	{
		m_isMouseOverVisible = false;
		m_isMouseDownVisible = true;
		m_pImageViewMouseDown->SetPosition(m_pUIRoot->GetPosition() + m_pFocusSlot->GetPosition());
	}
	//�̰� ��ü�� Ŭ�� ������ �̾߱�
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
		
		//�簢������ �� Ȯ��
		if (PtInRect(&rc, ptMouse))
		{
			//�巡�� ����
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

	//Ŭ���� ������ ���� ���콺 ��ǥ���� ���̰�
	POINT ptCursorOffset;
	ptCursorOffset.x = ptCurrent.x - ptClicked.x;
	ptCursorOffset.y = ptCurrent.y - ptClicked.y;

	//UI�� ��ǥ ��ǥ(���� ��ġ�� �� ��ǥ)
	POINT ptUIDestPos;
	ptUIDestPos.x = (int)vBeforeDragPos.x + (int)ptCursorOffset.x;
	ptUIDestPos.y = (int)vBeforeDragPos.y + (int)ptCursorOffset.y;

	//�����¿� ȭ����� ����� ���ϰ� ó�����ش�
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

	//��ǥ ��ǥ�� ���� ���ش�
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