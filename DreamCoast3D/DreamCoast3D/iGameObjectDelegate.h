#pragma once

class cGameObject;
class cGameActionSkinnedMeshObj;

__interface iGameObjectDelegate{
	bool isGameObjectCollided(cGameObject* pFrom);
	bool isGameAttackSphereCollided(cGameObject* pFrom, ST_BOUNDING_SPHERE stAttackSphere);
	std::vector<cGameObject*> GetInSightObject(ST_BOUNDING_SPHERE stSphere);
};