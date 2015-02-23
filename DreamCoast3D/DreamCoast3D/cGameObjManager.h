#pragma once

class cFrustum;
class cGameObject;
class cGameMapObject;

class cGameObjManager : public cObject
{
private:
	// 프러스텀 컬링용
	cFrustum*						m_pFrustum;

	cGameObject*					m_pPlayable;

	// HACK : Playable과 몬스터와 사물을 나눠놓을까?
	std::set<cGameObject*>			m_setGameObjects;

public:
	cGameObjManager();
	virtual ~cGameObjManager();

	void Setup();
	void Update(float fDelta);
	void Render();

	void AddGameObj(cGameObject* pGameObj);
	void RemoveGameObj(cGameObject* pGameObj);

	void AdjustYPositionByHeightMap(cGameMapObject* pMap);
	void SetPlayableGameObject(cGameObject* pPlayer);
	cGameObject* GetPlayerableGameObject() { return m_pPlayable; }

	void Destroy();
};

