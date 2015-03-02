#include "stdafx.h"
#include "cUIObject.h"


cUIObject::cUIObject()
	: m_pParent(NULL)
	, m_vPosition(0, 0, 0)
	, m_vScale(1, 1, 1)
	, m_pSprite(NULL)
	, m_pGameObjDelgate(NULL)
{
}

cUIObject::cUIObject(LPD3DXSPRITE pSprite)
	: m_pParent(NULL)
	, m_vPosition(0, 0, 0)
	, m_vScale(1, 1, 1)
	, m_pSprite(pSprite)
	, m_pGameObjDelgate(NULL)
	, m_pSceneDeligate(NULL)
{
	m_pSprite->AddRef();
}


cUIObject::~cUIObject()
{
	SAFE_RELEASE(m_pSprite);
}

void cUIObject::AddChild(cUIObject* pChild){
	pChild->AddRef();
	pChild->m_pParent = this;
	m_vecChild.push_back(pChild);
}

void cUIObject::Update(float fDelta){
	D3DXMATRIXA16 matS, matT;
	D3DXMatrixScaling(&matS, m_vScale.x, m_vScale.y, m_vScale.z);
	D3DXMatrixTranslation(&matT, m_vPosition.x, m_vPosition.y, m_vPosition.z);
	m_matWorld = matS * matT;
	if (m_pParent)
	{
		m_matWorld = m_matWorld * m_pParent->m_matWorld;
	}

	for each(cUIObject* pChild in m_vecChild)
	{
		pChild->Update(fDelta);
	}
}

void cUIObject::Render(){
	for each(cUIObject* pChild in m_vecChild)
	{
		pChild->Render();
	}
}

void cUIObject::Destroy(){
	for each(cUIObject* pChild in m_vecChild)
	{
		pChild->Destroy();
	}

	Release();
}

void cUIObject::SetGameObjDeligate(iGameObjectDelegate* pGameDeligate){
	for (auto p : m_vecChild){
		p->SetGameObjDeligate(pGameDeligate);
	}
	m_pGameObjDelgate = pGameDeligate;
}

void cUIObject::SetSceneDeligate(iSceneDelegate* pSceneDeligate){
	for (auto p : m_vecChild){
		p->SetSceneDeligate(pSceneDeligate);
	}
	m_pSceneDeligate = pSceneDeligate;
}