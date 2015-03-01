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
};