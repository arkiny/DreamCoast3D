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

	std::vector<POINT*> GetAdjCircle(int nX, int nZ, int nRange);

	// HeightMap Setup
	void Setup(int nMapSize);

	void AddMovingObject(cGameObject* pGameObejct, int nX, int nZ);
	void RemoveMovingObject(cGameObject* pGameObejct, int nX, int nZ);

	// Object Add
	void AddObjectOnGrid(cGameObject* pGameObejct, int nX, int nZ);
	// Object Add Customer
	void AddObjectCustomer(cGameObject* pGameObject, int nX, int nZ, int nHeight, int nWidth);
	// Grid ��ǥ �Է½� �ش� Grid ���� �ִ� Object�� �߽���ǥ�� ����
	D3DXVECTOR3 GetObjectCenter(int nX, int nZ);
	// Grid ��ǥ �Է��ϸ� �ش� Grid���� Object���� set���·� ����
	std::set<cGameObject*> GetObjectOnGrid(int nX, int nZ);
	// Grid ��ǥ �Է��ϸ� �ش� Grid���� Object���� vector���·� ����
	std::vector<cGameObject*> GetObjectOnGridVec(int nX, int nZ);
	// Grid ��ǥ �Է½� �߾��� ��ǥ�� vector ���·� ����
	D3DXVECTOR3 GetTileCenterCoord(int nX, int nZ);
	// Grid ��ǥ �Է½� 8������ Object���� vector ���·� ����
	std::vector<cGameObject*> GetAdjObject(int nX, int nZ);
	// Grid ��ǥ �Է½� �Է� ��ǥ �߽����� Size x Size Object���� vector ���·� ����
	std::vector<cGameObject*> GetAdjObjectCustomer(int nX, int nZ, int nSize);
	// �ش� Grid ���� Object�� ������ Object�� ������
	void RemoveObejctOnTile(cGameObject* pGameObejct, int nX, int nZ);
	// ��ǥ�� ���� �Է��ϸ� �������� ���� ��ŭ �˻��ؼ� �� ���� �ִ� ������Ʈ �˻��ؼ� vector���·� �Ѱ���
	std::vector<cGameObject*> GetAdjObjectCircle(int nX, int nZ, int nRange);

	void Destroy();

	// ��ǥ �Է½� �ش� Ÿ���� �׸��� ��ǥ�� �����ִ� �Լ�(���Ŀ� ����Ҽ��� ����)
	// �Ƹ� GetObjectOnGrid�ȿ� �ְ����� �ɰ��� ���� ����
	// �ִ��� ���μ���Ÿ���� ���ϰ�
	// ���ͳ� �÷��̾ �Ź� ������Ʈ�� ����Ǽ� ���� Ÿ���� �ڽ��� Ÿ�ϰ� �ٸ����
	// �ڽ��� �ִ� Ÿ�� ������Ʈ�� �ǽ�(���� �ֱ� ���)�ؾ� �ϱ� ����
	// POINT GetTileCoord(D3DXVECTOR3& vecPos)



};