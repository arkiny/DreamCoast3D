#include "stdafx.h"
#include "cUIObject.h"


cUIObject::cUIObject()
	: m_pParent(NULL)
	, m_vPosition(0, 0, 0)
	, m_vScale(1, 1, 1)
	, m_pSprite(NULL)
	, m_pGameObjDelgate(NULL)
	, m_pSceneDeligate(NULL)
	, m_pUIManagerDeligate(NULL)
	, m_fAlpha(1.f)
	, m_pEventDeligate(NULL)
	, m_pEffectManagerDelegate(NULL)
{
}

cUIObject::cUIObject(LPD3DXSPRITE pSprite)
	: m_pParent(NULL)
	, m_vPosition(0, 0, 0)
	, m_vScale(1, 1, 1)
	, m_pSprite(pSprite)
	, m_pGameObjDelgate(NULL)
	, m_pSceneDeligate(NULL)
	, m_pUIManagerDeligate(NULL)
	, m_fAlpha(1.f)
	, m_pEventDeligate(nullptr)
	, m_pEffectManagerDelegate(NULL)
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
	//m_vecChild.find
}
void cUIObject::SubChild(cUIObject* pChild){
	
	if (!pChild) { return; }
	pChild->m_pParent = nullptr;
	//Child 컨테이너에서 인수로 들어온 것과 같은 객체를 찾아서 컨테이너 내에서 삭제하고 Release한다.(자식의 자식들까지 모두)
	for (size_t i = 0; i < m_vecChild.size(); ++i)
	{
		if (m_vecChild[i] == pChild)
		{
			m_vecChild.erase(m_vecChild.begin() + i);
			pChild->Destroy();//(대대손손 다지움)
			//SAFE_RELEASE(pChild);(대상 자식 하나만 지울 경우)
		}
	}
}

// : 민우
std::vector<cUIObject*>& cUIObject::GetChild()
{
	return m_vecChild;
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

void cUIObject::SetEventDeligate(iGameEventDelegate* pEventDeligate)
{
	for (auto p : m_vecChild)
	{
		p->SetEventDeligate(pEventDeligate);
	}
	m_pEventDeligate = pEventDeligate;
}

void cUIObject::SetSceneDeligate(iSceneDelegate* pSceneDeligate){
	for (auto p : m_vecChild){
		p->SetSceneDeligate(pSceneDeligate);
	}
	m_pSceneDeligate = pSceneDeligate;
}

void cUIObject::SetUIManagerDeligate(iUIManagerDeligate* pUImanagerDeligate){
	for (auto p : m_vecChild){
		p->SetUIManagerDeligate(pUImanagerDeligate);
	}
	m_pUIManagerDeligate = pUImanagerDeligate;
}

void cUIObject::SetEffectDelegate(iEffectManagerDelegate* pEffectManager){
	for (auto p : m_vecChild){
		p->SetEffectDelegate(pEffectManager);
	}
	m_pEffectManagerDelegate = pEffectManager;
}