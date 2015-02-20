#pragma once
// �׸���� GameObject�� �ƴ϶� ������ �ý����̹Ƿ�
// �׳� cObject�� ��ӹްų� �ƿ� ����� ���� �ʴ´�.
class cGameObject;

class cGridSystem : public cObject
{
private:
	std::vector<std::set<cGameObject*>> m_vecTileData;
	int m_nMapSize;

	// �ش� Ÿ�� ��ü�� ���� �ִ� Ÿ���ΰ� ���� ���� Ÿ���ΰ�
	// 1�������� ���������� �� ��Ʈ�ѿ� �ʿ��ҵ�
	std::vector<bool>	m_vecMovable;

public:
	cGridSystem();
	~cGridSystem();

	// HeightMap Setup
	void Setup(int nMapSize);
	// HeigthMap ���� Object�߰��ϸ鼭 ���� ��
	void AddObjectOnGrid(cGameObject* pGameObejct, int nX, int nZ, int nObjectWidth, int nObjectHeight);
	// Grid ��ǥ �Է��ϸ� ������Ʈ�� Ƣ���
	// HACK
	// (Set�� �״�� �����ֱ� ���ٴ� ���� ����Ʈ�� ���۷����� �����ټ� �ְ�?)
	// ����� �� ������/�� ���Ͱ� �̵��̳�, ����üŷ�ҽÿ�
	// ���� �޸𸮸� �Ҵ��ؼ� ������� �ϹǷ� ���μ���Ÿ�� ������...
	// ���߿� �����غ���
	std::set<cGameObject*> GetObjectOnGrid(int nX, int nZ);



	// ���� �����ȵ�
	void RemoveObejctOnTile(cGameObject* pGameObejct, int nX, int nZ);
	
	// TODO
	// Ÿ�� �Է½� �ش� Ÿ���� �߽���ǥ�� �����ִ� �Լ�(���߿� ��ã�⶧�� ���)
	// D3DXVECTOR3 GetTileCenterCoord(int nX, int nZ)

	// Ÿ�� �Է½� �ֺ� Ÿ���� ������Ʈ�� �����ִ� �Լ�(�ֺ� �浹Ȯ�ν�)
	// std::vector<cGameObject*> GetAdjObject(int nX, int nZ);

	// ��ǥ �Է½� �ش� Ÿ���� �׸��� ��ǥ�� �����ִ� �Լ�(���Ŀ� ����Ҽ��� ����)
	// �Ƹ� GetObjectOnGrid�ȿ� �ְ����� �ɰ��� ���� ����
	// �ִ��� ���μ���Ÿ���� ���ϰ�
	// ���ͳ� �÷��̾ �Ź� ������Ʈ�� ����Ǽ� ���� Ÿ���� �ڽ��� Ÿ�ϰ� �ٸ����
	// �ڽ��� �ִ� Ÿ�� ������Ʈ�� �ǽ�(���� �ֱ� ���)�ؾ� �ϱ� ����
	// POINT GetTileCoord(D3DXVECTOR3& vecPos)
};

