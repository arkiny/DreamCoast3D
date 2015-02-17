#pragma once
#include "cGameObject.h"

class cGridSystem : public cGameObject
{
private:
	std::vector<std::set<cGameObject*>> m_vecTileData;
	int m_nMapSize;

public:
	cGridSystem();
	~cGridSystem();

	// HeightMap Setup
	void Setup(int nMapSize);
	// HeigthMap ���� Object�߰��ϸ鼭 ���� ��
	void AddObjectOnGrid(cGameObject* pGameObejct, int nX, int nZ, int nObjectWidth, int nObjectHeight);
	// Grid ��ǥ �Է��ϸ� ������Ʈ�� Ƣ���
	std::set<cGameObject*> GetObjectOnGrid(int nX, int nZ);


	// ���� �����ȵ�
	void RemoveObejctOnTile(cGameObject* pGameObejct, int nX, int nZ);
};

