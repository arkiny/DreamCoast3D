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
	// HeigthMap 위에 Object추가하면서 해줄 것
	void AddObjectOnGrid(cGameObject* pGameObejct, int nX, int nZ, int nObjectWidth, int nObjectHeight);
	// Grid 좌표 입력하면 오브젝트가 튀어나옴
	std::set<cGameObject*> GetObjectOnGrid(int nX, int nZ);


	// 아직 구현안됨
	void RemoveObejctOnTile(cGameObject* pGameObejct, int nX, int nZ);
};

