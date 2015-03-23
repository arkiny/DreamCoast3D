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
    , m_fAccumTime(0.f)
	, m_nScore(1000)
{
	// MS
	m_iMap = nullptr;

}


cGameObjManager::~cGameObjManager()
{
	SAFE_RELEASE(m_pFrustum);
	SAFE_RELEASE(m_pPlayable);

	while (!m_queueDeadMonster.empty()){
		SAFE_RELEASE(m_queueDeadMonster.front());
		m_queueDeadMonster.pop();
	}
	while (!m_queueGameObjectTobeErase.empty()){
		SAFE_RELEASE(m_queueGameObjectTobeErase.front());
		m_queueGameObjectTobeErase.pop();
	}
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
    // MS
	while (!m_queueGameObjectTobeErase.empty()){
		m_setGameObjects.erase(m_queueGameObjectTobeErase.front());
		m_queueDeadMonster.push(m_queueGameObjectTobeErase.front());
		if (m_queueGameObjectTobeErase.front()->GetGameObjectType() == cGameObject::E_MOP){
			cGameAIObject* p = (cGameAIObject*)m_queueGameObjectTobeErase.front();
			if (p->GetAItype() == p->E_AI_BOSS){
				m_nScore += 500;
			}
			else {
				m_nScore += 100;
			}
		}

		m_queueGameObjectTobeErase.pop();
	}
    DeadObjectUpdate();
}

void cGameObjManager::Render(){
	g_pD3DDevice->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER), 0x00000000, 1.0f, 0);
	LPDIRECT3DSURFACE9 pHWBackBuffer = NULL;
	LPDIRECT3DSURFACE9 pHWDepthStencilBuffer = NULL;
	g_pD3DDevice->GetRenderTarget(0, &pHWBackBuffer);
	g_pD3DDevice->GetDepthStencilSurface(&pHWDepthStencilBuffer);
	LPDIRECT3DSURFACE9 pShadowSurface = NULL;
	g_pShaderManager->GetShadowRenderTarget()->GetSurfaceLevel(0, &pShadowSurface);
	{
		g_pD3DDevice->SetRenderTarget(0, pShadowSurface);
		SAFE_RELEASE(pShadowSurface);
	}
	g_pD3DDevice->SetDepthStencilSurface(g_pShaderManager->GetShadowDepthStencil());
	g_pD3DDevice->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER), 0xFFFFFFFF, 1.0f, 0);

	for (auto p : m_setStaticGameObjects){
		if (m_pFrustum->IsIn(p->GetBoundingSphere())){
			if (p->GetEventType() != p->E_BLOCK){
				p->RenderShadow();
			}
		}
	}

	for (auto p : m_setGameObjects){
		if (m_pFrustum->IsIn(p->GetBoundingSphere())){
			p->RenderShadow();
		}
	}

	g_pD3DDevice->SetRenderTarget(0, pHWBackBuffer);
	g_pD3DDevice->SetDepthStencilSurface(pHWDepthStencilBuffer);
	SAFE_RELEASE(pHWBackBuffer);
	SAFE_RELEASE(pHWDepthStencilBuffer);

	for (auto p : m_setStaticGameObjects){
		if (m_pFrustum->IsIn(p->GetBoundingSphere())){
			if (p->GetEventType() != p->E_BLOCK){
				SAFE_RENDER(p);
			}
		}
	}

	for (auto p : m_setGameObjects){
		// 차후 Reasonable 하게 처리 가능할때 사용
		if (m_pFrustum->IsIn(p->GetBoundingSphere())){
			SAFE_RENDER(p);
		}
	}
}

void cGameObjManager::Render(LPD3DXEFFECT pEffect){
	for (auto p : m_setStaticGameObjects){
		if (m_pFrustum->IsIn(p->GetBoundingSphere())){
			pEffect->SetMatrix("gWorldMatrix", p->GetTransformMatrix());
			pEffect->CommitChanges();
			SAFE_RENDER(p);
		}
	}

	for (auto p : m_setGameObjects){
		if (m_pFrustum->IsIn(p->GetBoundingSphere())){
			pEffect->SetMatrix("gWorldMatrix", p->GetTransformMatrix());
			pEffect->CommitChanges();
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

void cGameObjManager::SetTileSystemForEvent()
{
	// Event Manager에서 참조
	for (auto p : m_setGameObjects){
		p->SetEventDelegate(m_pEventDeligate);
	}
	m_pEventDeligate->SetCurrentTileSystem(m_pGridTileSystem);
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

	m_iMap = pMap;
}

void cGameObjManager::SetCameraDeligate(iCameraDeligate* pCamera)
{
	m_pCameraDeligate = pCamera;
	m_pCameraDeligate->SetPlayerForCamera(m_pPlayable);
}


void cGameObjManager::SetEffectDeligate(iEffectManagerDelegate* pEffectDeligate){
	for (auto p : m_setGameObjects){
		p->SetEffectDelegate(pEffectDeligate);
	}
	m_pEffectDelegate = pEffectDeligate;
}


void cGameObjManager::SetPlayableGameObject(cGameObject* pPlayer){
	if (pPlayer && pPlayer != m_pPlayable){
		pPlayer->AddRef();
		m_pPlayable = pPlayer;
	}	
}

bool cGameObjManager::isGameObjectCollided(cGameObject* pFrom){
	float fDelta = g_pTimer->DeltaTime();
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
									p->ForcedMoving(-dist, pFrom->GetMoveSpeed() * fDelta);
								}
								else {
									pFrom->ForcedMoving(-dist, pFrom->GetMoveSpeed() * fDelta);
									p->ForcedMoving(-dist, pFrom->GetMoveSpeed() * fDelta);
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
	/*int nAttackRange = 3;
	D3DXVECTOR3 vFrom;
	vFrom = pFrom->GetPosition();
	std::vector<cGameObject*> vecGameObject;
	vecGameObject = m_pGridTileSystem->GetAdjObjectCustomer((int)vFrom.x, (int)vFrom.z, nAttackRange);*/

	for (auto p : m_setGameObjects){
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

				//if (isCollided(from, fFrom, scale, to, fTo, scale2)){
					std::map<std::string, ST_BOUNDING_SPHERE>* pMap = p->GetUpdatedDetailedSphere();
					for (auto pSphere : *pMap){

						D3DXVECTOR3 from = pFrom->GetCollisionSphere()->m_vCenter;
						D3DXVECTOR3 to = pSphere.second.m_vCenter;
						D3DXVECTOR3 dist = from - to;
						float fFrom = pFrom->GetCollisionSphere()->m_fRadius;
						float fTo = pSphere.second.m_fRadius;
						/*float scale = pFrom->GetScale().x;
						float scale2 = p->GetScale().x;*/

						float scale = pFrom->GetScale().x;
						float scale2 = p->GetTransform()->GetScaling().x;


						if (isCollided(from, fFrom, scale, to, fTo, scale2)){
							D3DXVECTOR3 vDir, vHit;
							D3DXVec3Normalize(&vDir, &dist);
							float scale2 = p->GetScale().x;
							vHit = to + vDir*scale2;
							p->OnHitTarget(pFrom, 10.0f, vHit);
							m_pCameraDeligate->AttackCameraMoving();
						}
					}
				//}
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
	int nAttackRange = 4;
	D3DXVECTOR3 vFrom;
	vFrom = pFrom->GetPosition();
	std::vector<cGameObject*> vecGameObject;
	vecGameObject = m_pGridTileSystem->GetAdjObjectCustomer((int)vFrom.x, (int)vFrom.z, nAttackRange);

	for (auto p : vecGameObject){
		if (p == pFrom){
			continue;
		}
		else{
			if (p->GetCollisionSphere()){
				for (auto p2 : *pFrom->GetAttackSpheres()){
					
					D3DXVECTOR3 from = p2.second.m_vCenter;
					D3DXVECTOR3 to = p->GetCollisionSphere()->m_vCenter;
					D3DXVECTOR3 dist = from - to;

					float fFrom = p2.second.m_fRadius;
					float fTo = p->GetCollisionSphere()->m_fRadius;

					float scale = 1.0f;
					float scale2 = 1.0f;

					if (isCollided(from, fFrom, scale, to, fTo, scale2)){
						std::map<std::string, ST_BOUNDING_SPHERE>* pMap = p->GetUpdatedDetailedSphere();

						for (auto pSphere : *pMap){
							D3DXVECTOR3 from = p2.second.m_vCenter;
							D3DXVECTOR3 to = pSphere.second.m_vCenter;
							D3DXVECTOR3 dist = from - to;
							float fFrom = p2.second.m_fRadius;
							float fTo = pSphere.second.m_fRadius;

							float scale = pFrom->GetScale().x;
							float scale2 = p->GetTransform()->GetScaling().x;

							if (isCollided(from, fFrom, scale, to, fTo, scale2)){
								if (p->GetGameObjectType() == p->E_PLAYABLE){

									D3DXVECTOR3 vDir, vHit;
									D3DXVec3Normalize(&vDir, &dist);
									vHit = to + vDir;

									p->OnHitTarget(pFrom, 10.0f, vHit);
								}
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

D3DXVECTOR3 cGameObjManager::isCollidedStaticObject(cGameObject* pFrom)
{
	D3DXVECTOR3 vFinal(0.f, 0.f, 0.f);

	std::set<cGameObject*> setGameObject = m_setStaticGameObjects;

	int nSearchRange = 1;
	D3DXVECTOR3 vFrom;
	vFrom = pFrom->GetPosition();
	std::vector<cGameObject*> vecGameObject;
	vecGameObject = m_pGridTileSystem->GetAdjObjectCustomer((int)vFrom.x, (int)vFrom.z, nSearchRange);

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

				return vFinal = vForce;
			}

		}
	}
	return vFinal;
}

void cGameObjManager::EraseFromGameObjectSet(cGameObject* pFrom)
{
    if (pFrom)
    {
		m_queueGameObjectTobeErase.push(pFrom);
    }
}

void cGameObjManager::DeadObjectUpdate()
{
    if (m_queueDeadMonster.size() > 0)
    {
        m_fAccumTime += g_pTimer->DeltaTime();
    }

    if (m_fAccumTime >= 5.f)
    {
        m_fAccumTime = 0.f;
        m_queueDeadMonster.front()->Start();
        m_setGameObjects.insert(m_queueDeadMonster.front());
        m_queueDeadMonster.pop();
    }
}

bool cGameObjManager::CalGradient(cGameObject* pFrom)
{
	bool isTest = false;

	D3DXVECTOR3 vNext(0.f, 0.f, 0.f);
	D3DXVECTOR3 vCurrent(0.f, 0.f, 0.f);

	vCurrent = m_vCurrentPos;
	vNext = m_vNextPos;

	bool isGarbage = false;

	float fNextHeight = m_iMap->GetHeight(isGarbage, &vNext);
	float fCurrentHeight = m_iMap->GetHeight(isGarbage, &vCurrent);

	float fDist = 0.f;

	float fX = vCurrent.x - vNext.x;
	float fZ = vCurrent.z - vNext.z;
	
	fDist = sqrt((fX*fX) + (fZ*fZ));

	float fAngle = atan2(fNextHeight - fCurrentHeight, fDist);
	fAngle = D3DXToDegree(fAngle);

	if (fAngle > 35.f)
	{
		isTest = true;
	}

	return isTest;
}

bool cGameObjManager::CalGradientMinus(cGameObject* pFrom)
{
	bool isTest = false;

	D3DXVECTOR3 vNext(0.f, 0.f, 0.f);
	D3DXVECTOR3 vCurrent(0.f, 0.f, 0.f);

	vCurrent = m_vCurrentPos;
	vNext = m_vNextPos;

	D3DXVECTOR3 vFront(0.f, 0.f, 0.f);
	vFront = vCurrent - vNext;
	vNext = vCurrent - vFront;

	bool isGarbage = false;

	float fNextHeight = m_iMap->GetHeight(isGarbage, &vNext);
	float fCurrentHeight = m_iMap->GetHeight(isGarbage, &vCurrent);

	float fDist = 0.f;

	float fX = vCurrent.x - vNext.x;
	float fZ = vCurrent.z - vNext.z;

	fDist = sqrt((fX*fX) + (fZ*fZ));

	float fAngle = atan2(fCurrentHeight - fNextHeight, fDist);
	fAngle = D3DXToDegree(fAngle);

	if (fAngle > 35.f)
	{
		isTest = true;
	}

	return isTest;
}

D3DXVECTOR3 cGameObjManager::GravityForce()
{
	D3DXVECTOR3 vFinal(0.f, 0.f, 0.f);

	D3DXVECTOR3 vNext(0.f, 0.f, 0.f);
	D3DXVECTOR3 vCurrent(0.f, 0.f, 0.f);

	vCurrent = m_vCurrentPos;
	vNext = m_vNextPos;

	bool isGarbage = false;

	float fNextHeight = m_iMap->GetHeight(isGarbage, &vNext);
	float fCurrentHeight = m_iMap->GetHeight(isGarbage, &vCurrent);

	float fDist = 0.f;

	float fX = vCurrent.x - vNext.x;
	float fZ = vCurrent.z - vNext.z;

	fDist = sqrt((fX*fX) + (fZ*fZ));

	float fAngle = atan2(fNextHeight - fCurrentHeight, fDist);
	fAngle = D3DXToDegree(fAngle);

	if (fAngle > 35.f)
	{
		vFinal.y = -9.8f;
	}

	return vFinal;
}

void cGameObjManager::SetNextPosition(D3DXVECTOR3 vNextPos)
{
	m_vNextPos = vNextPos;
}

void cGameObjManager::SetCurrentPosition(D3DXVECTOR3 vCurrentPos)
{
	m_vCurrentPos = vCurrentPos;
}

std::vector<POINT*> cGameObjManager::RangeSkill(cGameObject* pFrom)
{
	int nAttackRange = 3;
	D3DXVECTOR3 vFrom;
	vFrom = pFrom->GetPosition();
	std::vector<cGameObject*> vecGameObject;
	vecGameObject = m_pGridTileSystem->GetAdjObjectCircle((int)vFrom.x, (int)vFrom.z, nAttackRange);

	for (auto p : vecGameObject)
	{
		if (p == pFrom)
		{
			continue;
		}
		else
		{
			p->OnHitTarget(pFrom, 100.f, p->GetPosition());
		}
	}

	std::vector<POINT*> vec = m_pGridTileSystem->GetAdjCircle(vFrom.x, vFrom.z, nAttackRange);
	
	/*for (size_t i = 0; i < vec.size(); i++){
		D3DXVECTOR3 effPoint = { vec[i]->x + 0.5f, pFrom->GetPosition().y, vec[i]->y + 0.5f };
		m_pEffectDelegate->AddEffect(5, effPoint);
	}*/
	D3DXVECTOR3 effPoint = pFrom->GetPosition();
	effPoint.y += 1.0f;
	m_pEffectDelegate->AddEffect(5, effPoint);
	g_pSoundManager->executeSkill(0);
	
	return m_pGridTileSystem->GetAdjCircle(vFrom.x, vFrom.z, nAttackRange);
}