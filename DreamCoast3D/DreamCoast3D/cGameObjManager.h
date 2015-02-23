#pragma once

class cFrustum;
class cGameObject;
class cGameMapObject;

class cGameObjManager : public cObject
{
private:
	// �������� �ø���
	cFrustum*						m_pFrustum;

	cGameObject*					m_pPlayable;

	// HACK : Playable�� ���Ϳ� �繰�� ����������?
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

