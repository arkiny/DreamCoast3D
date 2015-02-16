#include "stdafx.h"
#include "cUIMinimap.h"
#include "cUIImageView.h"
#include "cUIImageButton.h"
#include "cUITextView.h"

cUIMinimap::cUIMinimap()
{
}

cUIMinimap::~cUIMinimap()
{
}

void cUIMinimap::Setup(){
	RECT clientRect;
	GetClientRect(g_hWnd, &clientRect);

	D3DXCreateSprite(g_pD3DDevice, &m_pSprite);
	// MiniMap_I1BF_2
	// TODO LATER

	cUIImageView* pImageView = new cUIImageView(m_pSprite);
	pImageView->SetTextureFilename(std::string("../Resources/UI/UI_MiniMap/MiniMap_I1BF_2.tga"));
	pImageView->SetScale(D3DXVECTOR3(0.8f, 0.8f, 1.0f));
	ST_SIZE stSizeRoot = pImageView->GetSize();

	float right = clientRect.right - stSizeRoot.fWidth * .8f;

	pImageView->SetPosition(D3DXVECTOR3(right, 20.0f, 0.0f));
	
	m_pUIRoot = pImageView;
}

void cUIMinimap::Update(float fDelta){
	if (m_pUIRoot)
		m_pUIRoot->Update(fDelta);
}

void cUIMinimap::Render(){
	if (m_pSprite)
	{
		m_pUIRoot->Render();
	}
}