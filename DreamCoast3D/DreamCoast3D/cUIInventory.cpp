#include "stdafx.h"
#include "cUIInventory.h"
#include "cUIImageView.h"


cUIInventory::cUIInventory()
{
}


cUIInventory::~cUIInventory()
{
	SAFE_RELEASE(m_pSprite);
	if (m_pUIRoot)
		m_pUIRoot->Destroy();
}

void cUIInventory::Setup()
{
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

	//��ĭ
	pImageView = new cUIImageView(m_pSprite);
	pImageView->SetTextureFilename(std::string("../Resources/ICON/ICON_Item/HP_Potion_Level_33_Tex.tga"));
	drawArea = { 0, 0, 64, 64};
	pImageView->SetDrawArea(drawArea);
	pImageView->SetScale(D3DXVECTOR3(0.85f, 0.85f, 0.85f));
	pImageView->SetPosition(D3DXVECTOR3(16, 85, 0));

	m_pUIRoot->AddChild(pImageView);
	SAFE_RELEASE(pImageView);

	//��ĭ��
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
	if (m_pUIRoot)
		m_pUIRoot->Update(fDelta);
}

void cUIInventory::Render()
{
	if (m_pSprite)
	{
		m_pUIRoot->Render();
	}
}
void cUIInventory::AddItem(cUIIcon& UIIcon)
{
	//TODO: ���⼭ ������ Ÿ���� �˻��ϰ� �������̸� �κ��丮�� �߰��Ѵ�.
	m_vecUIIcon.push_back(UIIcon);
}