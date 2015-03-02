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
	SAFE_RELEASE(m_pSprite);
	if (m_pUIRoot)
		m_pUIRoot->Destroy();
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

	ParticleSetup();
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

	ParticleRender();
}

void cUIMinimap::SetObject(std::vector<cGameObject*> m_setGameObject)
{
	m_setGameObject = m_setGameObject;
}

void cUIMinimap::SetPlayerPosition(D3DXVECTOR3* vPlayerPosition)
{
	m_stPlayerSightSphere.m_vCenter = *vPlayerPosition;
}

void cUIMinimap::SetSight(float fSightRange)
{
	m_stPlayerSightSphere.m_fRadius = fSightRange;
}

std::vector<cGameObject*> cUIMinimap::UpdateInSightObject(ST_BOUNDING_SPHERE stSphere)
{
	float fRadius = stSphere.m_fRadius;
	D3DXVECTOR3 vCenter = stSphere.m_vCenter;
	std::vector<cGameObject*> vecGameObject;

	for (auto p : m_setGameObject)
	{
		float fDist = 0.f;
		D3DXVECTOR3 vDist(0.f, 0.f, 0.f);
		D3DXVECTOR3 vObjectCenter(0.f, 0.f, 0.f);
		vObjectCenter = p->GetPosition();
		vDist = vObjectCenter - vCenter;
		fDist = D3DXVec3Length(&vDist);

		if (fDist <= fRadius)
		{
			vecGameObject.push_back(p);
		}
	}
	return vecGameObject;
}

void cUIMinimap::UpdateMinimap()
{
	UpdateInSightObject(m_stPlayerSightSphere);

	m_vecPositionGameObjectInSight.resize(m_vecGameObjectInSight.size());

	for (int i = 0; i < m_vecGameObjectInSight.size(); i++)
	{
		D3DXVECTOR3 vObject = m_vecGameObjectInSight[i]->GetPosition();
		D3DXVECTOR3 vPosition;
		vPosition = m_stPlayerSightSphere.m_vCenter - vObject;

		m_vecPositionGameObjectInSight[i] = vPosition;
	}
}

DWORD FtoDw(float f)
{
	return *((DWORD*)&f);
}

void cUIMinimap::ParticleSetup()
{
	g_pTextureManager->GetTexture("Resources/UI/UI_Particle/Particle.tga");

	// 포인트를 확대 축소 할 수 있게 해줌
	g_pD3DDevice->SetRenderState(D3DRS_POINTSCALEENABLE, true);

	// 포인트 스케일링 Factor값 설정
	g_pD3DDevice->SetRenderState(D3DRS_POINTSCALE_A, FtoDw(0.0f));
	g_pD3DDevice->SetRenderState(D3DRS_POINTSCALE_B, FtoDw(0.0f));
	g_pD3DDevice->SetRenderState(D3DRS_POINTSCALE_C, FtoDw(1.0f));

	// 포인트에 텍스쳐를 입힐 수 있게 해줌
	g_pD3DDevice->SetRenderState(D3DRS_POINTSPRITEENABLE, true);

	// 텍스쳐 알파 옵션 설정
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);

	// 알파블랜딩 방식 결정.
	g_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	g_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	//D3DXVECTOR3 vec(0.f, 0.f, 0.f);
	//vec = { 125, 10, 125 };

	//ST_PC_VERTEX v;
	//v.p = vec;
	//v.c = D3DCOLOR_XRGB(255, 100, 0);
	//m_vecParticle.push_back(v);

	D3DXMatrixIdentity(&m_mat);
}

void cUIMinimap::ParticleRender()
{
	g_pD3DDevice->SetRenderState(D3DRS_LIGHTING, false);

	// 포인트 사이즈 설정
	g_pD3DDevice->SetRenderState(D3DRS_POINTSIZE, FtoDw(1.0f));

	// 알파블렌딩 사용 가능하게 설정.
	g_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, true);

	g_pD3DDevice->SetRenderState(D3DRS_ZWRITEENABLE, false);

	LPDIRECT3DTEXTURE9 pTexture = g_pTextureManager->GetTexture("Particle.tga");
	g_pD3DDevice->SetTexture(0, pTexture);

	g_pD3DDevice->SetFVF(ST_PC_VERTEX::FVF);

	g_pD3DDevice->SetTransform(D3DTS_WORLD, &m_mat);

	//////


	//g_pD3DDevice->DrawPrimitiveUP(D3DPT_POINTLIST,
	//	m_vecParticle.size(),
	//	&m_vecParticle[0],
	//	sizeof(ST_PC_VERTEX));


	//////

	g_pD3DDevice->SetRenderState(D3DRS_ZWRITEENABLE, true);

	g_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
}

