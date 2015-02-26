#pragma once

class cGameObject;
class cGameActionSkinnedMeshObj;

__interface iGameObjectDelegate{
	bool isGameObjectCollided(cGameObject* pFrom);
	bool isGameAttackSphereCollided(cGameActionSkinnedMeshObj* pFrom, ST_BOUNDING_SPHERE stAttackSphere);
};