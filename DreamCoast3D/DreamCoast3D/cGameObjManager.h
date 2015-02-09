#pragma once

class cFrustum;

class cGameObject;
class cGameMapObject;

class cGameObjManager : public cObject
{
private:
	// 프러스텀 컬링용
	cFrustum*						m_pFrustum;
	std::set<cGameObject*>			m_setGameObjects;

public:
	cGameObjManager();
	~cGameObjManager();

	void Setup();
	void Update(float fDelta);
	void Render();

	void AddGameObj(cGameObject* pGameObj);
	void RemoveGameObj(cGameObject* pGameObj);

	void AdjustYPositionByHeightMap(cGameMapObject* pMap);

	void Destroy();
};

