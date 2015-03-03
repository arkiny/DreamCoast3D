#include "stdafx.h"
#include "cGameObjManager.h"
#include "cFrustum.h"
#include "cTransform.h"
#include "cGameMapObject.h"
#include "cGameAIObject.h"
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
			//p->SetTargetObject(m_pPlayable);
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
	bool ret = false;
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

					if (isCollided(from, fFrom, scale, to, fTo, scale2)){
						std::map<std::string, ST_BOUNDING_SPHERE>* pMap = p->GetUpdatedDetailedSphere();
						for (auto pSphere : *pMap){

							D3DXVECTOR3 from = pFrom->GetCollisionSphere()->m_vCenter;
							D3DXVECTOR3 to = pSphere.second.m_vCenter;
							D3DXVECTOR3 dist = from - to;
							float fFrom = pFrom->GetCollisionSphere()->m_fRadius;
							float fTo = pSphere.second.m_fRadius;
							float scale = pFrom->GetScale().x;
							float scale2 = p->GetScale().x;
					
							if (isCollided(from, fFrom, scale, to, fTo, scale2)){
								if (pFrom->GetState() < 1){
									p->ForcedMoving(-dist, pFrom->GetMoveSpeed() * .1f);
								}
								else {
									pFrom->ForcedMoving(-dist, pFrom->GetMoveSpeed() * .1f);
									p->ForcedMoving(-dist, pFrom->GetMoveSpeed() * .3f);
								}
								ret = true;
							}
						}
					}
				}
				else{
					continue;
				}
			}
		}
	}
	return ret;
}

bool cGameObjManager::isCollided(D3DXVECTOR3 vFromCenter, float fFromRad, float fFromScale, D3DXVECTOR3 vToCenter, float fCenterRad, float fToScale){
	D3DXVECTOR3 dist = vFromCenter - vToCenter;
	float fIntersect = fFromRad*fFromScale + fCenterRad*fToScale;
	float fDist = D3DXVec3Length(&dist);
	return fIntersect > fDist;
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

				if (isCollided(from, fFrom, scale, to, fTo, scale2)){
					std::map<std::string, ST_BOUNDING_SPHERE>* pMap = p->GetUpdatedDetailedSphere();
					for (auto pSphere : *pMap){

						D3DXVECTOR3 from = pFrom->GetCollisionSphere()->m_vCenter;
						D3DXVECTOR3 to = pSphere.second.m_vCenter;
						D3DXVECTOR3 dist = from - to;
						float fFrom = pFrom->GetCollisionSphere()->m_fRadius;
						float fTo = pSphere.second.m_fRadius;
						/*float scale = pFrom->GetScale().x;
						float scale2 = p->GetScale().x;*/

						float scale = 1.0f;
						float scale2 = 1.0f;

						if (isCollided(from, fFrom, scale, to, fTo, scale2)){
							p->OnHitTarget(pFrom);
							m_pCameraDeligate->AttackCameraMoving();
						}
					}
				}
			}
			else{
				continue;
			}
		}
	}
	return false;
}

D3DXVECTOR3 cGameObjManager::PushingForce(D3DXVECTOR3* vFrom, float fFromLength, D3DXVECTOR3* vTo, float fFromTo)
{
	D3DXVECTOR3 vForceFrom(0.f, 0.f, 0.f);
	D3DXVECTOR3 vForceTo(0.f, 0.f, 0.f);
	D3DXVECTOR3 vDist(0.f, 0.f, 0.f);

	float fX = 0.f;
	float fY = 0.f;
	float fDist = 0.f;
	float fAngle = 0.f;
	float fRadius = 0.f;

	fRadius = fFromLength;

	vDist = vForceFrom - vForceTo;
	fDist = D3DXVec3Length(&vDist);

	// from -> to
	fX = vFrom->x - vTo->x;
	fY = vFrom->z - vTo->z;

	fAngle = atan2(fY, fX);

	vForceFrom.x = cos(fAngle);
	vForceFrom.z = sin(fAngle);
	D3DXVec3Normalize(&vForceFrom, &vForceFrom);

	vForceFrom *= fRadius;

	// to -> from
	fX = vTo->x - vFrom->x;
	fY = vTo->z - vFrom->z;

	fAngle = atan2(fY, fX);
	
	vForceTo.x = cos(fAngle);
	vForceTo.z = sin(fAngle);
	D3DXVec3Normalize(&vForceTo, &vForceTo);

	vForceTo *= fDist - fRadius;

	D3DXVECTOR3 vForce = vForceFrom - vForceTo;
	D3DXVec3Normalize(&vForce, &vForce);

	return vForce;
}

// MS
std::vector<cGameObject*> cGameObjManager::GetInSightObject(ST_BOUNDING_SPHERE stSphere)
{
	float fRadius = stSphere.m_fRadius;
	D3DXVECTOR3 vCenter = stSphere.m_vCenter;
	std::vector<cGameObject*> vecGameObject;

	for (auto p : m_setGameObjects)
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

void cGameObjManager::AttackMobToPlayer(cGameAIObject* pFrom){
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
				D3DXVECTOR3 from = pFrom->GetAttackSphere()->m_vCenter;
				D3DXVECTOR3 to = p->GetCollisionSphere()->m_vCenter;
				D3DXVECTOR3 dist = from - to;

				float fFrom = pFrom->GetAttackSphere()->m_fRadius;
				float fTo = p->GetCollisionSphere()->m_fRadius;
				/*float scale = pFrom->GetScale().x;
				float scale2 = p->GetScale().x;*/
				float scale = 1.0f;
				float scale2 = 1.0f;
				if (isCollided(from, fFrom, scale, to, fTo, scale2)){
					std::map<std::string, ST_BOUNDING_SPHERE>* pMap = p->GetUpdatedDetailedSphere();
					for (auto pSphere : *pMap){

						D3DXVECTOR3 from = pFrom->GetCollisionSphere()->m_vCenter;
						D3DXVECTOR3 to = pSphere.second.m_vCenter;
						D3DXVECTOR3 dist = from - to;
						float fFrom = pFrom->GetCollisionSphere()->m_fRadius;
						float fTo = pSphere.second.m_fRadius;
						/*float scale = pFrom->GetScale().x;
						float scale2 = p->GetScale().x;*/

						float scale = 1.0f;
						float scale2 = 1.0f;

						if (isCollided(from, fFrom, scale, to, fTo, scale2)){
							if (p->GetGameObjectType() == p->E_PLAYABLE){
								p->OnHitTarget(pFrom);
							}
						}
					}
				}
			}
			else{
				continue;
			}
		}
	}
	//return false;
}

ST_STAT_INFO* cGameObjManager::GetPlayerStatInfo(){
	ST_STAT_INFO* ret = m_pPlayable->GetStatInfo();
	return ret;
}

std::set<cGameObject*> cGameObjManager::GetObjectData()
{
	std::set<cGameObject*> setGameObject;
	setGameObject = m_setGameObjects;
	return setGameObject;
}

cGameObject* cGameObjManager::GetPlayerData()
{
	return m_pPlayable;
}

bool cGameObjManager::isCollidedStaticObject(cGameObject* pFrom)
{
	bool isColliedStaticObj = false;

	std::set<cGameObject*> setGameObject = m_setStaticGameObjects;

	int nSearchRange = 1;
	D3DXVECTOR3 vFrom;
	vFrom = pFrom->GetPosition();
	std::vector<cGameObject*> vecGameObject;
	vecGameObject = m_pGridTileSystem->GetAdjObjectCustomer(vFrom.x, vFrom.z, nSearchRange);

	for (auto p : vecGameObject)
	{
		if (p == pFrom)
		{
			continue;
		}
		else
		{
			if (setGameObject.count(p) > 0)
			{
				isColliedStaticObj = true;

				D3DXVECTOR3 vPlayer(0.f, 0.f, 0.f);
				D3DXVECTOR3 vObject(0.f, 0.f, 0.f);
				D3DXVECTOR3 vForce(0.f, 0.f, 0.f);
				D3DXVECTOR3 vRepulsiveForce(0.f, 0.f, 0.f);

				vPlayer = pFrom->GetPosition();
				vPlayer.y = 0.f;
				vObject = p->GetPosition();
				vObject.y = 0.f;

				vRepulsiveForce = vPlayer - vObject;

				float fDist = D3DXVec3Length(&vRepulsiveForce);
				vForce = vRepulsiveForce / fDist;
				pFrom->ForcedMoving(vForce, pFrom->GetMoveSpeed());
			}

			else
			{
				continue;
			}

		}
	}

	return isColliedStaticObj;
}