#pragma once
#include "iGameObjectDelegate.h"
#include "iCameraDeligate.h"

// MS
#include "iGameEventDelegate.h"

class cFrustum;
class cGameMapObject;
class cGamePlayableObject;

class cGameObjManager : public cObject, public iGameObjectDelegate
{
public:
	int m_nScore;

private:
	// 프러스텀 컬링용

	cFrustum*						m_pFrustum;

	cGameObject*					m_pPlayable;

	iGridTileSystem*				m_pGridTileSystem;
	iCameraDeligate*				m_pCameraDeligate;
	iEffectManagerDelegate*			m_pEffectDelegate;

	// HACK : Playable과 몬스터와 사물을 나눠놓을까?
	std::set<cGameObject*>			m_setGameObjects;
	std::set<cGameObject*>			m_setStaticGameObjects;

	// 삭제해야할 오브젝트
	std::queue<cGameObject*>		m_queueGameObjectTobeErase;
	// 죽은 오브젝트
	std::queue<cGameObject*>		m_queueDeadMonster;

    // MS
    float m_fAccumTime;
	
    //std::set<cGameObject*>          m_setDeadMonster;

	iGameEventDelegate*				m_pEventDeligate;
public:
	cGameObjManager();
	virtual ~cGameObjManager();

	void Setup();
	void Update(float fDelta);
	void Render();

	virtual void Render(LPD3DXEFFECT pEffect);


	void SetCurrentTileSystem(iGridTileSystem* pGridSystem);
	void SetEffectDeligate(iEffectManagerDelegate* pEffectDeligate);

	void SetCameraDeligate(iCameraDeligate* pCamera);


	// MS
	void SetEventDeligate(iGameEventDelegate* pEventDeligate){ m_pEventDeligate = pEventDeligate; }

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
	std::vector<cGameObject*> GetInSightObject(ST_BOUNDING_SPHERE stSphere) override;
	void AttackMobToPlayer(cGameAIObject* pFrom) override;
	ST_STAT_INFO* GetPlayerStatInfo() override;
	void EraseFromGameObjectSet(cGameObject* pFrom) override;
	int	GetScore() override {
		return m_nScore;
	}
	void SetScore(int nNewScore) override {
		m_nScore = nNewScore;
	}

	//MS
	cGameObject* GetPlayerData() override;
	std::set<cGameObject*> GetObjectData() override;
	D3DXVECTOR3 isCollidedStaticObject(cGameObject* pFrom) override;
	void SetTileSystemForEvent();


protected:
	bool isCollided(D3DXVECTOR3 vFromCenter,
		float fFromRad, float fFromScale,
		D3DXVECTOR3 vToCenter,
		float fCenterRad, float fToScale);


private:
	// Calculated by Conan
	D3DXVECTOR3 PushingForce(D3DXVECTOR3* vFrom, float fFromLength, D3DXVECTOR3* vTo, float fFromTo);
    void DeadObjectUpdate();

	bool CalGradient(cGameObject* pFrom);
	bool CalGradientMinus(cGameObject* pFrom);
	iMap* m_iMap;
	void SetNextPosition(D3DXVECTOR3 vNextPos);
	void SetCurrentPosition(D3DXVECTOR3 vCurrentPos);
	D3DXVECTOR3 m_vNextPos;
	D3DXVECTOR3 m_vCurrentPos;

	D3DXVECTOR3 GravityForce();

	std::vector<POINT*> RangeSkill(cGameObject* pFrom);
};

