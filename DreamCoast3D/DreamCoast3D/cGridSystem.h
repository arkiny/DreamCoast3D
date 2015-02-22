#pragma once
// 그리드는 GameObject가 아니라 일종의 시스템이므로
// 그냥 cObject를 상속받거나 아예 상속을 받지 않는다.
class cGameObject;
class cASEInstance;

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
	// HeigthMap 위에 Object추가하면서 해야함
	void AddObjectOnGrid(cGameObject* pGameObejct, int nX, int nZ, int nObjectWidth, int nObjectHeight);
	// Grid 좌표 입력시 해당 Grid 위에 있는 Object의 중심좌표가 나옴
	D3DXVECTOR3 GetObjectCenter(int nX, int nZ);

	// HACK
	// (Set을 그대로 돌려주기 보다는 셋의 포인트나 리퍼런스를 돌려줄수 있게?)
	// 현재는 매 프레임/매 몬스터가 이동이나, 전투체킹할시에
	// 새로 메모리를 할당해서 돌려줘야 하므로 프로세스타임 문제가...
	// 나중에 생각해보자
	// Grid 좌표 입력하면 오브젝트가 튀어나옴
	std::set<cGameObject*> GetObjectOnGrid(int nX, int nZ);

	// TODO
	// 타일 입력시 해당 타일의 중심좌표를 돌려주는 함수(나중에 길찾기때에 사용)
	D3DXVECTOR3 GetTileCenterCoord(int nX, int nZ);

	// 타일 입력시 주변 타일의 오브젝트를 돌려주는 함수(주변 충돌확인시) 
	// 8방향 검사
	std::vector<cGameObject*> GetAdjObject(int nX, int nZ);

	// 좌표 입력시 해당 타일의 그리드 좌표를 돌려주는 함수(차후에 사용할수도 있음)
	// 아마 GetObjectOnGrid안에 있겠지만 쪼개는 편이 좋음
	// 최대한 프로세스타임을 줄일것
	// 몬스터나 플레이어가 매번 업데이트때 실행되서 현재 타일이 자신의 타일과 다를경우
	// 자신이 있는 타일 업데이트를 실시(빼서 넣기 등등)해야 하기 때문
	// POINT GetTileCoord(D3DXVECTOR3& vecPos)

	// 아직 구현안됨
	void RemoveObejctOnTile(cGameObject* pGameObejct, int nX, int nZ);


};

