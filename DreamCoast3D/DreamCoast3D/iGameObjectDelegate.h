#pragma once

class cGameObject;
class cGameActionSkinnedMeshObj;
class cGameAIObject;

__interface iGameObjectDelegate{
	bool isGameObjectCollided(cGameObject* pFrom);
	bool isGameAttackSphereCollided(cGameObject* pFrom, ST_BOUNDING_SPHERE stAttackSphere);;
	void AttackMobToPlayer(cGameAIObject* pFrom);
	std::vector<cGameObject*> GetInSightObject(ST_BOUNDING_SPHERE stSphere);
	ST_STAT_INFO* GetPlayerStatInfo();
	void EraseFromGameObjectSet(cGameObject* pFrom);
	int	GetScore();
	void SetScore(int nNewScore);

	// MS
	cGameObject* GetPlayerData();
	std::set<cGameObject*> GetObjectData();
	D3DXVECTOR3 isCollidedStaticObject(cGameObject* pFrom);

	bool CalGradient(cGameObject* pFrom);
	bool CalGradientMinus(cGameObject* pFrom);
	void SetNextPosition(D3DXVECTOR3 vNextPos);
	void SetCurrentPosition(D3DXVECTOR3 vCurrentPos);
	D3DXVECTOR3 GravityForce();

	std::vector<POINT*> RangeSkill(cGameObject* pFrom);
};