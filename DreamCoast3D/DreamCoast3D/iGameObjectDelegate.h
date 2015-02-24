#pragma once

class cGameObject;

__interface iGameObjectDelegate{
	bool isGameObjectCollided(cGameObject* pFrom);
};