#pragma once
#include "iGameObjectDelegate.h"
#include "iCameraDeligate.h"
class cFrustum;
class cGameObject;
class cGameMapObject;

class cGameObjManager : public cObject, public iGameObjectDelegate
{
private:
	// 프러스텀 컬링용
	cFrustum*						m_pFrustum;

	cGameObject*					m_pPlayable;

	iGridTileSystem*				m_pGridTileSystem;
	iCameraDeligate*				m_pCameraDeligate;
	

	// HACK : Playable과 몬스터와 사물을 나눠놓을까?
	std::set<cGameObject*>			m_setGameObjects;
	std::set<cGameObject*>			m_setStaticGameObjects;

public:
	cGameObjManager();
	virtual ~cGameObjManager();

	void Setup();
	void Update(float fDelta);
	void Render();

	void SetCurrentTileSystem(iGridTileSystem* pGridSystem);
	void SetCameraDeligate(iCameraDeligate* pCamera){ m_pCameraDeligate = pCamera; }

	void AddGameObj(cGameObject* pGameObj);
	void RemoveGameObj(cGameObject* pGameObj);

	void AddStaticGameObjects(cGameObject* pGameObj);
	void RemoveStaticGameObjects(cGameObject* pGameObj); 

	void AdjustYPositionByHeightMap(iMap* pMap);
	
	void SetPlayableGameObject(cGameObject* pPlayer);
	cGameObject* GetPlayerableGameObject() { return m_pPlayable; }

	void Destroy();

	// iGameObjectDelegate override
	bool isGameObjectCollided(cGameObject* pFrom) override;
	bool isGameAttackSphereCollided(cGameObject* pFrom, ST_BOUNDING_SPHERE stAttackSphere) override;

protected:
	bool isCollided(D3DXVECTOR3 vFromCenter,
		float fFromRad, float fFromScale,
		D3DXVECTOR3 vToCenter,
		float fCenterRad, float fToScale);

	// Calculate by Conan
private:
	D3DXVECTOR3 PushingForce(D3DXVECTOR3* vFrom, float fFromLength, D3DXVECTOR3* vTo, float fFromTo);
};

