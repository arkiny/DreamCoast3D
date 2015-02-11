#pragma once

class cFrustum;
class cGameObject;
class cGameMapObject;

class cGameObjManager : public cObject
{
private:
	// �������� �ø���
	cFrustum*						m_pFrustum;

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

	void Destroy();
};

