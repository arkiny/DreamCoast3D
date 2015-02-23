#pragma once

class cGameObject;

class cGridSystem : public cObject
{
private:
	std::vector<std::set<cGameObject*>> m_vecTileData;
	std::vector<std::vector<cGameObject*>> m_vecGameObject;

	int m_nMapSize;
	float m_nConstTileSize;

	// 해당 타일 자체가 들어갈수 있는 타일인가 들어갈수 없는 타일인가
	// 1차적으로 일정수준의 맵 컨트롤에 필요할득
	std::vector<bool>	m_vecMovable;

public:
	cGridSystem();
	~cGridSystem();

	// HeightMap Setup
	void Setup(int nMapSize);
	// Object Add
	void AddObjectOnGrid(cGameObject* pGameObejct, int nX, int nZ);
	// Grid 좌표 입력시 해당 Grid 위에 있는 Object의 중심좌표가 나옴
	D3DXVECTOR3 GetObjectCenter(int nX, int nZ);
	// Grid 좌표 입력하면 해당 Grid위의 Object값이 set형태로 리턴
	std::set<cGameObject*> GetObjectOnGrid(int nX, int nZ);
	// Grid 좌표 입력시 중앙의 좌표가 vector 형태로 리턴
	D3DXVECTOR3 GetTileCenterCoord(int nX, int nZ);
	// Grid 좌표 입력시 8방향의 Object들이 vector 형태로 리턴
	std::vector<cGameObject*> GetAdjObject(int nX, int nZ);
	// 해당 Grid 위에 Object가 있을시 Object를 삭제함
	void RemoveObejctOnTile(cGameObject* pGameObejct, int nX, int nZ);


	// 좌표 입력시 해당 타일의 그리드 좌표를 돌려주는 함수(차후에 사용할수도 있음)
	// 아마 GetObjectOnGrid안에 있겠지만 쪼개는 편이 좋음
	// 최대한 프로세스타임을 줄일것
	// 몬스터나 플레이어가 매번 업데이트때 실행되서 현재 타일이 자신의 타일과 다를경우
	// 자신이 있는 타일 업데이트를 실시(빼서 넣기 등등)해야 하기 때문
	// POINT GetTileCoord(D3DXVECTOR3& vecPos)
};



//std::vector<POINT> vecTest;
//D3DXVECTOR3 vCenter;
//for (int z = 0; z < 256; z++)
//{
//	for (int x = 0; x < 256; x++)
//	{
//		vCenter = pGridSystem->GetObjectCenter(x, z);
//		if (vCenter.x == 0.f && vCenter.z == 0.f)
//		{
//
//		}
//		else
//		{
//			POINT pos;
//			pos.x = vCenter.x;
//			pos.y = vCenter.z;
//			vecTest.push_back(pos);
//		}
//	}
//}