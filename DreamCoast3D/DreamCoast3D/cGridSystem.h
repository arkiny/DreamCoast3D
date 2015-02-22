#pragma once
// �׸���� GameObject�� �ƴ϶� ������ �ý����̹Ƿ�
// �׳� cObject�� ��ӹްų� �ƿ� ����� ���� �ʴ´�.
class cGameObject;
class cASEInstance;

class cGridSystem : public cObject
{
private:
	std::vector<std::set<cGameObject*>> m_vecTileData;
	std::vector<std::vector<cGameObject*>> m_vecGameObject;

	int m_nMapSize;
	float m_nConstTileSize;

	// �ش� Ÿ�� ��ü�� ���� �ִ� Ÿ���ΰ� ���� ���� Ÿ���ΰ�
	// 1�������� ���������� �� ��Ʈ�ѿ� �ʿ��ҵ�
	std::vector<bool>	m_vecMovable;

public:
	cGridSystem();
	~cGridSystem();

	// HeightMap Setup
	void Setup(int nMapSize);
	// HeigthMap ���� Object�߰��ϸ鼭 �ؾ���
	void AddObjectOnGrid(cGameObject* pGameObejct, int nX, int nZ, int nObjectWidth, int nObjectHeight);
	// Grid ��ǥ �Է½� �ش� Grid ���� �ִ� Object�� �߽���ǥ�� ����
	D3DXVECTOR3 GetObjectCenter(int nX, int nZ);

	// HACK
	// (Set�� �״�� �����ֱ� ���ٴ� ���� ����Ʈ�� ���۷����� �����ټ� �ְ�?)
	// ����� �� ������/�� ���Ͱ� �̵��̳�, ����üŷ�ҽÿ�
	// ���� �޸𸮸� �Ҵ��ؼ� ������� �ϹǷ� ���μ���Ÿ�� ������...
	// ���߿� �����غ���
	// Grid ��ǥ �Է��ϸ� ������Ʈ�� Ƣ���
	std::set<cGameObject*> GetObjectOnGrid(int nX, int nZ);

	// TODO
	// Ÿ�� �Է½� �ش� Ÿ���� �߽���ǥ�� �����ִ� �Լ�(���߿� ��ã�⶧�� ���)
	D3DXVECTOR3 GetTileCenterCoord(int nX, int nZ);

	// Ÿ�� �Է½� �ֺ� Ÿ���� ������Ʈ�� �����ִ� �Լ�(�ֺ� �浹Ȯ�ν�) 
	// 8���� �˻�
	std::vector<cGameObject*> GetAdjObject(int nX, int nZ);

	// ��ǥ �Է½� �ش� Ÿ���� �׸��� ��ǥ�� �����ִ� �Լ�(���Ŀ� ����Ҽ��� ����)
	// �Ƹ� GetObjectOnGrid�ȿ� �ְ����� �ɰ��� ���� ����
	// �ִ��� ���μ���Ÿ���� ���ϰ�
	// ���ͳ� �÷��̾ �Ź� ������Ʈ�� ����Ǽ� ���� Ÿ���� �ڽ��� Ÿ�ϰ� �ٸ����
	// �ڽ��� �ִ� Ÿ�� ������Ʈ�� �ǽ�(���� �ֱ� ���)�ؾ� �ϱ� ����
	// POINT GetTileCoord(D3DXVECTOR3& vecPos)

	// ���� �����ȵ�
	void RemoveObejctOnTile(cGameObject* pGameObejct, int nX, int nZ);


};

