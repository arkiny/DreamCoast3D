#pragma once

class cGameObject;

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
	// Object Add
	void AddObjectOnGrid(cGameObject* pGameObejct, int nX, int nZ);
	// Grid ��ǥ �Է½� �ش� Grid ���� �ִ� Object�� �߽���ǥ�� ����
	D3DXVECTOR3 GetObjectCenter(int nX, int nZ);
	// Grid ��ǥ �Է��ϸ� �ش� Grid���� Object���� set���·� ����
	std::set<cGameObject*> GetObjectOnGrid(int nX, int nZ);
	// Grid ��ǥ �Է½� �߾��� ��ǥ�� vector ���·� ����
	D3DXVECTOR3 GetTileCenterCoord(int nX, int nZ);
	// Grid ��ǥ �Է½� 8������ Object���� vector ���·� ����
	std::vector<cGameObject*> GetAdjObject(int nX, int nZ);
	// �ش� Grid ���� Object�� ������ Object�� ������
	void RemoveObejctOnTile(cGameObject* pGameObejct, int nX, int nZ);


	// ��ǥ �Է½� �ش� Ÿ���� �׸��� ��ǥ�� �����ִ� �Լ�(���Ŀ� ����Ҽ��� ����)
	// �Ƹ� GetObjectOnGrid�ȿ� �ְ����� �ɰ��� ���� ����
	// �ִ��� ���μ���Ÿ���� ���ϰ�
	// ���ͳ� �÷��̾ �Ź� ������Ʈ�� ����Ǽ� ���� Ÿ���� �ڽ��� Ÿ�ϰ� �ٸ����
	// �ڽ��� �ִ� Ÿ�� ������Ʈ�� �ǽ�(���� �ֱ� ���)�ؾ� �ϱ� ����
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