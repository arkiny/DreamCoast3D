#include "stdafx.h"
#include "cUIInventory.h"
#include "cUIImageView.h"
#include "cDataItem.h"

//TODO:
//ȭ�� �����ڸ� ��ó������ �ڼ����� ��������
//���� ������ �ִϸ��̼� + �ִϸ��̼� ������ ������ ��ȣ�ۿ��� ����
//HACK: �巡�׷� ȭ�� ������ �̵��� �õ� ������ ���콺�� ����ġ�� ���ƿ��� ���� �����ڸ����� �������� �� �������� ���� �� ����.

//���ε� �����ܿ� ���ؼ� �Ϸ� �ؾ��Ѵ�.

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

	//������ ��� ����
	for (size_t i = 0; i < m_vecOwnItem.size(); ++i)
	{
		SAFE_RELEASE(m_vecOwnItem[i]);
	}
	//������ ��� ��������? Parent->Child ������ ���ԵǾ� �ֳ�����
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
	//����Ű �Ҵ�
	//SetHotKey('I');
	//��������Ʈ ����
	SetUIPopupWindowDelegate(this);
	//�θ� cUIPopupWindow�� ���. �˾� �Ǿ����� �ƴ��� �Ǵ��ϰ� �׸��⸦ �����Ѵ�.
	SetisPopped(false);
	
	//ȭ���� Ŭ���̾�Ʈ ������ ���Ѵ�
	RECT clientRect;
	GetClientRect(g_hWnd, &clientRect);
	//��������Ʈ ����
	D3DXCreateSprite(g_pD3DDevice, &m_pSprite);
	
	m_vecOwnItem.resize(10);//�����ϰ� �ִ� ������ ����(������ �Ǿ����)


#pragma region �κ��丮 ����(UIRoot) �ʱ�ȭ
	//�κ��丮 ����(UIRoot)
	cUIImageView* pImageView = new cUIImageView(m_pSprite);
	pImageView->SetTextureFilename(std::string("../Resources/UI/UI_INVENTORY/BG_Inventory.tga"));

	//���� �׷��� ũ��. �̹��� ������ ũ��� ���� �̹����� �κ��� �ٸ��� ������ ���� �������ش�.
	RECT drawArea = { 0, 0, 472, 436 };
	pImageView->SetDrawArea(drawArea);
	//��ġ(x, y)
	pImageView->SetPosition(D3DXVECTOR3(50, 50, 0));
	pImageView->SetAlpha(0);
	pImageView->SetScale(D3DXVECTOR3(1.f, 1.f, 1.f));
	SetUIRoot(pImageView);
	//m_pUIRoot = pImageView;

	//�巡�׵Ǳ� �� ��ǥ�� ���� ��ǥ�� �ʱ�ȭ
	SetBeforeDragPos(m_pUIRoot->GetPosition());
#pragma endregion

	//HACK: ��׸� �κ��丮�� Child�� �ٲ����
	//���콺 ������ ���� �׵θ�
	m_pImageViewMouseOver = new cUIImageView(m_pSprite);
	m_pImageViewMouseOver->Setup(
		std::string("../Resources/UI/UI_INVENTORY/Component_I352.tga"),
		56, 56,
		D3DXVECTOR3(0, 0, 0),
		D3DXVECTOR3(1.f, 1.f, 1.f));
	
	//���콺 Ŭ���� ���� �׵θ�
	m_pImageViewMouseDown = new cUIImageView(m_pSprite);
	m_pImageViewMouseDown->Setup(
		std::string("../Resources/UI/UI_INVENTORY/Component_I355.tga"),
		56, 56,
		D3DXVECTOR3(0, 0, 0),
		D3DXVECTOR3(1.f, 1.f, 1.f));

	//���Ե� ��ġ
	SetupSlot(5, 8);

	//��� �׽�Ʈ�� �ʱ�ȭ
	SetupTest();
	
}
void cUIInventory::Update(float fDelta)
{
	//����Ű�� ���ʷ� �������� �Է����� �˻��Ѵ�. Ű �ݺ��Է� ������
	if (!g_pControlManager->GetInputInfo(GetHotKey()))
	{
		m_isKeyHold = false;
	}
	//������ �߰�
	if (g_pControlManager->GetInputInfo('C'))
	{
		AddItem(m_vecOwnItem[1]);
	}
	//������ �̵� ����
	if (g_pControlManager->GetInputInfo('V'))
	{
		SwapItemInSlot(m_vecUISlot[0], m_vecUISlot[39]);
	}
	//â�� ���� ���� ��
	if (GetisPopped())
	{
		m_pFocusSlot = (cUISlot*)FindFocusSlot(m_vecUISlot);
		//���콺 ����/�ٿ�� �� ���̶���Ʈ
		HighLightFocusSlot();

#pragma region ���ĺ��� ȿ��
		//�ӽ� ȿ��(����� �� �����ϰ� ����� �����)
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

		//�޹�ưDown��
		if (g_pControlManager->GetInputInfo(VK_LBUTTON)){ OnMouseLBDown(); }
		//�޹�ưUp��
		else if (!g_pControlManager->GetInputInfo(VK_LBUTTON)){ OnMouseLBUp(); }
		//�����ʹ�ưDown��
		if (g_pControlManager->GetInputInfo(VK_RBUTTON)){ OnMouseRBDown(); }
		//�����ʹ�ưUp��
		else if (!g_pControlManager->GetInputInfo(VK_RBUTTON)){ OnMouseRBUp(); }
		
		//�巡�� ���̸� �巡�� �ൿ
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
		if (m_pBindingIcon){ m_pBindingIcon->Render(); }
	}
}

//��ĭ(nullptr)�� ã�Ƽ� �������� �߰��ϰ� ���Կ� �����ۿ� �ش�Ǵ� �������� ����Ѵ�.
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
//���� ��ȣ�� ����
void cUIInventory::DeleteItemInSlot(size_t nSlotNum)
{
	DeleteItemInSlot(m_vecUISlot[nSlotNum]);
}
//���� �����ͷ�
void cUIInventory::DeleteItemInSlot(cUISlot* pUISlot)
{
	pUISlot->DeleteItem();
}
void cUIInventory::OnMouseLBDown()
{
	//�޸��콺�� �ٿ�Ǵ°� �Ǻ��ؼ� �巡�׷� ��ȯ�Ϸ��� �����Ǹ޼��� �̹Ƿ� �巡�����̸� Ż��
	if (m_isDragging) { return; }

	////��ü�� Ŭ�� ������ WindowDrag�� ��ȯ�Ǵ� Ʈ����
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
			SetDragState(E_WINDOW_DRAG);
		}
	}
	
	//������ Ŭ�� ������ IconDrag�� ��ȯ�Ǵ� Ʈ����
	if (m_pFocusSlot && m_isDragging == false)
	{
		//���Կ� ��Ŀ���� ������ ���콺 �ٿ� ���̶���Ʈ
		SetIsMouseOverVisible(false);
		SetIsMouseDownVisible(true);
		m_pImageViewMouseDown->SetPosition(m_pUIRoot->GetPosition() + m_pFocusSlot->GetPosition());

		//���� Ŭ���� ���Կ� �������� �����ϴ��� Ȯ��
		bool isItemExist;
		if (m_pFocusSlot->GetItem()){ isItemExist = true; }
		else { isItemExist = false; }

		//(�������� ������) ���콺 �ٿ�� ���Կ� �������� �־�� �巡�׷� ��ȯ
		if (isItemExist)
		{
			m_isDragging = true;
			SetDragState(E_ICON_DRAG);
			//�巡�װ� ���۵� ������ ���
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

			//�巡�� ���� �������� ���
			//m_pIconDragSrc = m_pFocusSlot->GetIcon();
		}
		//�������� ������
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
		//�޹�ư�� Up������ ������ư�� Down�ϰ�� ������ �̹����� �����ؾ� �ϹǷ� �˻�
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
	//���Կ� ��Ŀ���� ������
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
		//������ư�� Up������ �޹�ư�� Down�ϰ�� ������ �̹����� �����ؾ� �ϹǷ� �˻�
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
	//�󽽷Կ� ������ �巡�װ� ������ �̰��� ��� ó���� ���ΰ�

	if (m_eDragState == E_WINDOW_DRAG) { return nullptr; }	//â�� �巡�� �ϴ� ���̸� ������ �˻��� ���� �����Ƿ� Ż��
	else if (m_eDragState == E_EMPTY_DRAG) { return nullptr; }	//�󽽷��� �巡�� �ϴ� ���̸� ������ �˻��� ���� �����Ƿ� Ż��
	RECT rtArea;
	for (size_t i = 0; i < vecUISlot.size(); ++i)
	{
		//HACK: �̰� �� Root�� ��ǥ�� ���ؿ;��ϳ�.. �� ���� ����� ���� �� ����. : �ο�
		//Invectory Root�κ��� ������ ������ǥ�� ���س��� ������
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
	//���Ե�
	//IntersectRect();//�簢�浹üũ �Լ�
	//PtInRect(&rc, ptMouse);//�簢������ �� Ȯ��
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
	//�ϴ� ���õ��� �������� �������ְ�
	cDataItem* pUsedItem = pUISlot->GetItem();
	//TODO: ��밡���� ���������� üũ�ϰ�
	//�����ϸ�
	if (true)
	{
		//������ ����
		DeleteItemInSlot(pUISlot);
		//���� �������� ��ȯ�Ѵ�
		return pUsedItem;
	}
	//�Ұ����ϸ� nullptr ����
	return nullptr;
}
void cUIInventory::SwapItemInSlot(cUISlot* pSrcSlot, cUISlot* pDestSlot)
{
	//nullptr�̸� nullptr�δ�� ���� ��ȯ�Ѵ�. ������ ��������� �̴�� �Ѵ�.

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
	//���콺 ���� ���� �׵θ� ��ġ�� ��Ŀ�̵� ���Կ� �����(�̵��Ѵ�)
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
#pragma region ������ �׽�Ʈ ����

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