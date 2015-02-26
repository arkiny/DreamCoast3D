#include "stdafx.h"
#include "cGameObjManager.h"
#include "cFrustum.h"
#include "cTransform.h"
#include "cGameMapObject.h"
#include "cGameActionSkinnedMeshObj.h"

cGameObjManager::cGameObjManager()
	:m_pFrustum(NULL)
	, m_pPlayable(NULL)
{
}


cGameObjManager::~cGameObjManager()
{
	SAFE_RELEASE(m_pFrustum);
	SAFE_RELEASE(m_pPlayable);
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
	for (auto p : m_setStaticGameObjects){
		if (m_pFrustum->IsIn(p->GetBoundingSphere())){
			SAFE_RENDER(p);
		}
	}

	for (auto p : m_setGameObjects){
		if (m_pFrustum->IsIn(p->GetBoundingSphere())){
			SAFE_RENDER(p);
		}
	}
}

void cGameObjManager::SetCurrentTileSystem(iGridTileSystem* pGridSystem){
	m_pGridTileSystem = pGridSystem;

	for (auto p : m_setStaticGameObjects){
		p->SetGridTileSystem(pGridSystem);
	}

	for (auto p : m_setGameObjects){
		p->SetGridTileSystem(pGridSystem);
	}
}

void cGameObjManager::AddGameObj(cGameObject* pGameObj){
	if (pGameObj){
		if (m_setGameObjects.find(pGameObj) == m_setGameObjects.end()){
			SAFE_ADD_REF(pGameObj);
			pGameObj->SetGameObjDeligate(this);
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

void cGameObjManager::AddStaticGameObjects(cGameObject* pGameObj){
	if (pGameObj){
		if (m_setStaticGameObjects.find(pGameObj) == m_setStaticGameObjects.end()){
			SAFE_ADD_REF(pGameObj);
			pGameObj->SetGameObjDeligate(this);
			m_setStaticGameObjects.insert(pGameObj);
		}
	}
}

void cGameObjManager::RemoveStaticGameObjects(cGameObject* pGameObj){
	if (pGameObj){
		if (m_setStaticGameObjects.find(pGameObj) != m_setStaticGameObjects.end()){
			m_setStaticGameObjects.erase(pGameObj);
			SAFE_RELEASE(pGameObj);
		}
	}
}

void cGameObjManager::Destroy(){
	for (auto p : m_setStaticGameObjects){
		SAFE_RELEASE(p);
	}
	for (auto p : m_setGameObjects){
		SAFE_RELEASE(p);
	}	
	this->Release(); 
}

void cGameObjManager::AdjustYPositionByHeightMap(iMap* pMap){
	// 높이맵의 높이에 따라 Y좌표를 수정해준다.
	for (auto p : m_setGameObjects){
		bool bIsLand = false;
		float yPos = pMap->GetHeight(bIsLand, &p->GetTransform()->GetPosition());
		if (bIsLand){
			p->SetYPosition(yPos);
		}
	}

	for (auto p : m_setStaticGameObjects){
		bool bIsLand = false;
		float yPos = pMap->GetHeight(bIsLand, &p->GetTransform()->GetPosition());
		if (bIsLand){
			p->SetYPosition(yPos);
		}
	}
}

void cGameObjManager::SetPlayableGameObject(cGameObject* pPlayer){
	if (pPlayer && pPlayer != m_pPlayable){
		pPlayer->AddRef();
		m_pPlayable = pPlayer;
	}	
}

bool cGameObjManager::isGameObjectCollided(cGameObject* pFrom){
	if (pFrom->GetCollisionSphere()){
		for (auto p : m_setGameObjects){
			if (p == pFrom){
				continue;
			}
			else{
				if (p->GetCollisionSphere()){
					D3DXVECTOR3 from = pFrom->GetCollisionSphere()->m_vCenter;
					D3DXVECTOR3 to = p->GetCollisionSphere()->m_vCenter;
					D3DXVECTOR3 dist = from - to;
					float fFrom = pFrom->GetCollisionSphere()->m_fRadius;
					float fTo = p->GetCollisionSphere()->m_fRadius;
					float scale = pFrom->GetScale().x;
					float scale2 = p->GetScale().x;
					float fIntersect = fFrom*scale + fTo*scale2;
					//pFrom->GetCollisionSphere()->m_vCenter;
					//pFrom->GetCollisionSphere()->m_fRadius;
					//p->GetCollisionSphere()->m_vCenter;
					//p->GetCollisionSphere()->m_fRadius;
					float fDist = D3DXVec3Length(&dist);
					if (fIntersect > fDist){
						return true;
					}
				}
				else{
					continue;
				}
			}
		}
	}
	return false;
}

bool cGameObjManager::isGameAttackSphereCollided(
	cGameObject* pFrom,
	ST_BOUNDING_SPHERE stAttackSphere)
{
	int nAttackRange = 10;
	D3DXVECTOR3 vFrom;
	vFrom = pFrom->GetPosition();
	std::vector<cGameObject*> vecGameObject;
	vecGameObject = m_pGridTileSystem->GetAdjObjectCustomer(vFrom.x, vFrom.z, nAttackRange);

	for (auto p : vecGameObject){
		if (p == pFrom){
			continue;
		}
		else{
			if (p->GetCollisionSphere()){
				D3DXVECTOR3 from = stAttackSphere.m_vCenter;
				D3DXVECTOR3 to = p->GetCollisionSphere()->m_vCenter;
				D3DXVECTOR3 dist = from - to;

				float fFrom = stAttackSphere.m_fRadius;
				float fTo = p->GetCollisionSphere()->m_fRadius;
				float scale = pFrom->GetScale().x;
				float scale2 = p->GetScale().x;
				float fIntersect = fFrom*scale + fTo*scale2;

				float fDist = D3DXVec3Length(&dist);
				if (fIntersect > fDist){
					p->ChangeState(4);
					// 공격 스피어와 충돌
					// p->ChangeState(OnHit);
					// int a = 0;
					//pFrom->ChangeState(4);
				}
			}
			else{
				continue;
			}
		}
	}

	return false;
}