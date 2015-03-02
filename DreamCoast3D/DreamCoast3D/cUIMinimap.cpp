#include "stdafx.h"
#include "cUIMinimap.h"
#include "cUIImageView.h"
#include "cUIImageButton.h"
#include "cUITextView.h"

cUIMinimap::cUIMinimap()
	: m_fMiniMapSize(93.0f)
	, m_pSpriteMiniMap(NULL)
	, m_pTexture(NULL)
{
}

cUIMinimap::~cUIMinimap()
{
	SAFE_RELEASE(m_pSprite);
	if (m_pUIRoot)
		m_pUIRoot->Destroy();

	SAFE_RELEASE(m_pSpriteMiniMap);
	SAFE_RELEASE(m_pTexture);
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

	D3DXCreateSprite(g_pD3DDevice, &m_pSpriteMiniMap);
	D3DXCreateTextureFromFileEx(
		g_pD3DDevice,
		"../Resources/UI/UI_Particle/point.png",
		D3DX_DEFAULT_NONPOW2,
		D3DX_DEFAULT_NONPOW2,
		D3DX_DEFAULT,
		0,
		D3DFMT_UNKNOWN,
		D3DPOOL_MANAGED,
		D3DX_FILTER_NONE,
		D3DX_DEFAULT,
		0,
		&m_stImageInfo,
		NULL,
		&m_pTexture);
}

void cUIMinimap::Start()
{
	m_pGameObjDelgate->GetPlayerStatInfo();
	std::set<cGameObject*> setGameObject = m_pGameObjDelgate->GetObjectData();
	SetObject(setGameObject);
	cGameObject* pGameObject = m_pGameObjDelgate->GetPlayerData();
	D3DXVECTOR3 vec = m_pGameObjDelgate->GetPlayerData()->GetPosition();
	SetPlayerPosition(&vec);
	SetSight(30.f);

	////cGameObject* pGameObject = 
	//D3DXVECTOR3 vec = m_pGameObjDelgate->GetPlayerData()->GetPosition();
	//SetPlayerPosition(&vec);
	//SetSight(30.f);
}

void cUIMinimap::Update(float fDelta){
	if (m_pUIRoot)
		m_pUIRoot->Update(fDelta);
	std::set<cGameObject*> setGameObject = m_pGameObjDelgate->GetObjectData();
	SetObject(setGameObject);
	cGameObject* pGameObject = m_pGameObjDelgate->GetPlayerData();
	D3DXVECTOR3 vec = pGameObject->GetPosition();
	SetPlayerPosition(&vec);
	SetSight(30.f);
	UpdateInSightObject(m_stPlayerSightSphere);
	UpdateMinimap();
	ObjectPositionUpdate();
	//float x = m_pGameObjDelgate->GetPlayerStatInfo()->fCurrentHp;
	//D3DXVECTOR3 vec = m_pGameObjDelgate->GetPlayerData()->GetPosition();
	//cGameObject* pGameObject = m_pGameObjDelgate->GetPlayerData();

	//std::set<cGameObject*> setGameObject = m_pGameObjDelgate->GetObjectData();
	//SetObject(setGameObject);
	//cGameObject* pGameObject = m_pGameObjDelgate->GetPlayerData();
	//D3DXVECTOR3 vec = pGameObject->GetPosition();
	//SetPlayerPosition(&vec);
	//SetSight(30.f);
	//UpdateInSightObject(m_stPlayerSightSphere);
	//UpdateMinimap();
	//ObjectPositionUpdate();

}

void cUIMinimap::Render(){
	if (m_pSpriteMiniMap)
	{
		for (int i = 0; i < m_vecPositionGameObjectInSight.size(); i++)
		{
			m_pSpriteMiniMap->Begin(D3DXSPRITE_ALPHABLEND | D3DXSPRITE_SORT_TEXTURE);
			RECT rc;
			SetRect(&rc, 0, 0, m_stImageInfo.Width, m_stImageInfo.Height);
			D3DXMATRIXA16 matWorld;
			D3DXMATRIXA16 matS;
			D3DXMATRIXA16 matR;
			D3DXMatrixRotationX(&matR, D3DX_PI / 2);
			D3DXMatrixTranslation(&matWorld, m_vecPositionGameObjectInSight[i].x + 1148,
				m_vecPositionGameObjectInSight[i].y + 93, 0.5);
			D3DXMatrixScaling(&matS, 0.5f, 0.5f, 0.f);
			matWorld = matS*matWorld;
			m_pSpriteMiniMap->SetTransform(&matWorld);
			m_pSpriteMiniMap->Draw(m_pTexture,
				&rc,
				&D3DXVECTOR3(0, 0, 0),
				&D3DXVECTOR3(0, 0, 0),
				D3DCOLOR_XRGB(255, 255, 255));
			m_pSpriteMiniMap->End();
		}

	}

	if (m_pSprite)
	{
		m_pUIRoot->Render();
	}


}

void cUIMinimap::SetObject(std::set<cGameObject*> setGameObject)
{
	m_setGameObject = setGameObject;
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
	m_vecGameObjectInSight = UpdateInSightObject(m_stPlayerSightSphere);

	m_vecPositionGameObjectInSight.resize(m_vecGameObjectInSight.size());

	for (int i = 0; i < m_vecGameObjectInSight.size(); i++)
	{
		D3DXVECTOR3 vObject = m_vecGameObjectInSight[i]->GetPosition();
		D3DXVECTOR3 vPosition(0.f, 0.f, 0.f);
		vPosition.x = m_stPlayerSightSphere.m_vCenter.x - vObject.x;
		vPosition.y = m_stPlayerSightSphere.m_vCenter.z - vObject.z;

		m_vecPositionGameObjectInSight[i] = vPosition;
	}
}

void cUIMinimap::ObjectPositionUpdate()
{
	//for (int i = 0; i < m_vecGameObjectInSight.size(); i++)
	//{
	//	D3DXVECTOR3* vObjectPosition = new D3DXVECTOR3;
	//	vObjectPosition = &m_vecPositionGameObjectInSight[i];
	//	vObjectPosition->x = (vObjectPosition->x / m_stPlayerSightSphere.m_fRadius) 
	//		* (m_fMiniMapSize) + m_fMiniMapSize;
	//	vObjectPosition->y = (vObjectPosition->y / m_stPlayerSightSphere.m_fRadius)
	//		* (m_fMiniMapSize) + m_fMiniMapSize;

	//	cUIImageView* pImageView = new cUIImageView(m_pSprite);
	//	pImageView->SetTextureFilename(std::string("../Resources/UI/UI_Particle/Particle.tga"));
	//	pImageView->SetPosition(*vObjectPosition);
	//	pImageView->SetScale(D3DXVECTOR3(0.01f, 0.01f, 0.01f));

	//	m_pUIRoot->AddChild(pImageView);
	//	SAFE_RELEASE(pImageView);
	//}
}