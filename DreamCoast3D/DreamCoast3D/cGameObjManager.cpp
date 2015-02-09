#include "stdafx.h"
#include "cGameObjManager.h"
#include "cFrustum.h"
#include "cTransform.h"
#include "cGameMapObject.h"

cGameObjManager::cGameObjManager()
	:m_pFrustum(NULL)
{
}


cGameObjManager::~cGameObjManager()
{
	SAFE_RELEASE(m_pFrustum);
}

void cGameObjManager::Setup(){
	// HACK : Scene매니저에서 하나만 만들어서 돌려가면서 쓸수 있게 생각해보자.
	m_pFrustum = new cFrustum;
	m_pFrustum->Setup();
	m_pFrustum->SetDesc("Frustom For Example1");
}

void cGameObjManager::Update(float fDelta){
	if (m_pFrustum){
		m_pFrustum->Update(fDelta); 
	}
	if (m_setGameObjects.size() > 0){
		for (auto p : m_setGameObjects){
			p->Update(fDelta);
		}
	}
}

void cGameObjManager::Render(){
	for (auto p : m_setGameObjects){
		if (m_pFrustum->IsIn(p->GetBoundingSphere())){
			SAFE_RENDER(p);
		}
	}
}

void cGameObjManager::AddGameObj(cGameObject* pGameObj){
	if (pGameObj){
		if (m_setGameObjects.find(pGameObj) == m_setGameObjects.end()){
			SAFE_ADD_REF(pGameObj);
			m_setGameObjects.insert(pGameObj);
		}
	}
}

void cGameObjManager::RemoveGameObj(cGameObject* pGameObj){
	if (pGameObj){
		if (m_setGameObjects.find(pGameObj) != m_setGameObjects.end()){
			m_setGameObjects.erase(pGameObj);
			SAFE_RELEASE(pGameObj);
		}
	}
}

void cGameObjManager::Destroy(){
	for (auto p : m_setGameObjects){
		SAFE_RELEASE(p);
	}
	this->Release(); 
}

void cGameObjManager::AdjustYPositionByHeightMap(cGameMapObject* pMap){
	// 높이맵의 높이에 따라 Y좌표를 수정해준다.
	for (auto p : m_setGameObjects){
		bool bIsLand = false;
		float yPos = pMap->GetHeight(bIsLand, &p->GetTransform()->GetPosition());
		if (bIsLand){
			p->SetYPosition(yPos);
		}
	}
}