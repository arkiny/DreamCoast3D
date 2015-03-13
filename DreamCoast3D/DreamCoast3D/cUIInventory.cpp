#include "stdafx.h"
#include "cUIInventory.h"
#include "cUIImageView.h"
#include "cDataItem.h"

//TODO:
//ȭ�� ������ ������� �ϱ�
//ȭ�� �����ڸ� ��ó������ �ڼ����� ��������
//���� ������ �ִϸ��̼� + �ִϸ��̼� ������ ������ ��ȣ�ۿ��� ����
//HACK: �巡�׷� ȭ�� ������ �̵��� �õ� ������ ���콺�� ����ġ�� ���ƿ��� ���� �����ڸ����� �������� �� �������� ���� �� ����.

//�����ܿ� ��������Ʈ �̹����� ������ �Ѵ�.
//���� ¥���� ������ ����ߵȴ�.

//������ ���Կ� ��ĭ�� �ǰ� �ؾ��Ѵ�
//AddItem Get/Set�迭 ���� Ŀ���� �ؾߵǰڴ�
//SAFE_RELEASE Ȯ���� �����ؾ��Ѵ�.

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

	//������ ��� ����
	for (size_t i = 0; i < m_vecOwnItem.size(); ++i)
	{
		SAFE_RELEASE(m_vecOwnItem[i]);
	}
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
	m_pImageViewMouseOver->SetTextureFilename(std::string("../Resources/UI/UI_INVENTORY/Component_I352.tga"));
	drawArea = { 0, 0, 56, 56 };
	m_pImageViewMouseOver->SetPosition(D3DXVECTOR3(0, 0, 0));

	//���콺 Ŭ���� ���� �׵θ�
	m_pImageViewMouseDown = new cUIImageView(m_pSprite);
	m_pImageViewMouseDown->SetTextureFilename(std::string("../Resources/UI/UI_INVENTORY/Component_I355.tga"));
	drawArea = { 0, 0, 56, 56 };
	m_pImageViewMouseDown->SetPosition(D3DXVECTOR3(56, 0, 0));

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

	}
	//â�� ���� ���� ��
	if (GetisPopped())
	{
		//���콺 ���� ���� �׵θ�
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
		//�ӽ� ȿ��
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
		//�޹�ưDown��
		if (g_pControlManager->GetInputInfo(VK_LBUTTON)){ OnMouseLBDown(); }
		//�޹�ưUp��
		else if (!g_pControlManager->GetInputInfo(VK_LBUTTON)){ OnMouseLBUp(); }
		//�����ʹ�ưDown��
		if (g_pControlManager->GetInputInfo(VK_RBUTTON)){ OnMouseRBDown(); }
		//�����ʹ�ưUp��
		else if (!g_pControlManager->GetInputInfo(VK_RBUTTON)){ OnMouseRBUp(); }
		
		//�巡�� ���̸� �巡�� �ൿ
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
//���� ��ȣ�� ����
void cUIInventory::DeleteItemInSlot(size_t nSlotNum)
{
	DeleteItemInSlot(m_vecUISlot[nSlotNum]);
}
//���� �����ͷ�
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
void cUIInventory::OnMouseRBDown()
{
	//���Կ� ��Ŀ���� ������
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

cUIObject* cUIInventory::FindFocusSlot(std::vector<cUISlot*>& vecUISlot)
{
	RECT rtArea;
	for (size_t i = 0; i < vecUISlot.size(); ++i)
	{
		//HACK: �̰� �� Root�� ��ǥ�� ���ؿ;��ϳ�.. �� ���� ����� ���� �� ����. : �ο�
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
	//���Ե�
	//IntersectRect();//�簢�浹üũ �Լ�
	//PtInRect(&rc, ptMouse);//�簢������ �� Ȯ��
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
	//�ϴ� ���õ��� �������� �������ְ�
	cDataItem* pUsedItem = pUISlot->GetItem();
	pUISlot->DeleteChild(pUsedItem->GetUIIcon());
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
void cUIInventory::SetupTest()
{
#pragma region ������ �׽�Ʈ ����
	
	LPD3DXSPRITE pSprite = nullptr;
	D3DXCreateSprite(g_pD3DDevice, &pSprite);
	//D3DXCreateSprite(g_pD3DDevice, &m_pSprite);

	cUIIcon* pUIIcon = new cUIIcon(pSprite);
	pUIIcon->SetTextureFilename(std::string("../Resources/ICON/ICON_Item/HP_Potion_Level_33_Tex.tga"));
	RECT rtDrawArea = { 0, 0, 56, 56 };
	pUIIcon->SetDrawArea(rtDrawArea);
	//pUIIcon->SetPosition(D3DXVECTOR3(m_vecUISlot[0]->GetPosition().x+4, m_vecUISlot[0]->GetPosition().y+4, 0));//FIX: �ϴ� �ϵ��ڵ�. ������ �����ؾ� �Ѵ�.
	pUIIcon->SetPosition(D3DXVECTOR3(4, 4, 0));//FIX: �ϴ� �ϵ��ڵ�. ������ �����ؾ� �Ѵ�.
	pUIIcon->SetScale(D3DXVECTOR3(0.8f, 0.8f, 0.8f));

	cDataItem* pDataItem = nullptr;
	pDataItem = new cDataItem(pUIIcon);
	m_vecOwnItem[0] = pDataItem;
	m_vecUISlot[0]->AddChild(pDataItem->GetUIIcon());
	m_vecUISlot[0]->SetItem(m_vecOwnItem[0]);
	m_vecUISlot[0]->SetIcon(m_vecOwnItem[0]->GetUIIcon());
	

	SAFE_RELEASE(pSprite);
	
#pragma endregion

	//�۵� �׽�Ʈ�� 0��ĭ �Ҵ�
	//m_vecUISlot[0]->SetItemNum(0);//0�� ���Կ� 0�� ������
	//cDataItem* pItem = m_vecOwnItem[m_vecUISlot[0]->GetItemNum()];
	//cDataItem* pItem = m_vecOwnItem[0];
	/*m_vecUISlot[0]->SetItem(pItem);
	m_vecUISlot[0]->SetIcon(pItem->GetUIIcon());*/
	//AddItem(m_vecOwnItem[0]);

}