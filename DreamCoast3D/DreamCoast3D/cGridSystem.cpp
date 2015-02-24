#include "stdafx.h"
#include "cGridSystem.h"
#include "cASEInstance.h"
#include <assert.h>


cGridSystem::cGridSystem()
	: m_nMapSize(0)
	, m_nConstTileSize(0.0f)
{
}


cGridSystem::~cGridSystem()
{
	this->Destroy();
}

void cGridSystem::Setup(int nMapSize)
{
	int nTotalTile = nMapSize * nMapSize;

	m_nMapSize = nMapSize;

	m_vecTileData.reserve(nTotalTile);
	m_vecGameObject.reserve(nTotalTile);

	for (int i = 0; i < nTotalTile; i++)
	{
		std::set<cGameObject*> setGameObject;
		std::vector<cGameObject*> vecGameObject;
		m_vecTileData.push_back(setGameObject);
		m_vecGameObject.push_back(vecGameObject);
	}
}

void cGridSystem::AddObjectOnGrid(cGameObject* pGameObejct, int nX, int nZ)
{
	SAFE_ADD_REF(pGameObejct);
	//int GridX = nX * m_nConstTileSize;
	//int GridZ = nZ * m_nConstTileSize;
	// Object ���� X,Z ��ǥ

	float fWidth = 0.0f;
	float fHeight = 0.0f;

	if (pGameObejct->GetBoundingBox()){
		ST_BOUNDING_BOX stBox;
		stBox = *pGameObejct->GetBoundingBox();
		fWidth = stBox.vMax.x - stBox.vMin.x;
		fHeight = stBox.vMax.z - stBox.vMin.z;
	}

	int nBeginGridX = nX - fWidth / 2;
	int nBeginGridZ = nZ - fHeight / 2;

	for (int z = nBeginGridZ; z < nBeginGridZ + fWidth; z++)
	{
		for (int x = nBeginGridX; x < nBeginGridX + fHeight; x++)
		{
			// �׸��� ���� ���� Object�� �ִ��� Ȯ��
			if (m_vecTileData[x + z*m_nMapSize].find(pGameObejct)
				== m_vecTileData[x + z*m_nMapSize].end())
			{
				m_vecTileData[x + z*m_nMapSize].insert(pGameObejct);
				m_vecGameObject[x + z*m_nMapSize].push_back(pGameObejct);
				SAFE_ADD_REF(pGameObejct);
			}
			else
			{
				assert(m_vecTileData[x + z*m_nMapSize].find(pGameObejct)
					== m_vecTileData[x + z*m_nMapSize].end() && "�̹� �߰��� ������Ʈ �Դϴ�");
			}
		}
	}
}

std::set<cGameObject*> cGridSystem::GetObjectOnGrid(int nX, int nZ)
{
	return m_vecTileData[nX + nZ*m_nMapSize];
}

D3DXVECTOR3 cGridSystem::GetObjectCenter(int nX, int nZ)
{
	cGameObject* GameObject = new cGameObject;

	if (m_vecTileData[nX + nZ*m_nMapSize].size() > 0)
	{
		std::set<cGameObject*> setGameObject;
		setGameObject = m_vecTileData[nX + nZ*m_nMapSize];

		//std::vector<cGameObject*> vecGameObject;
		//vecGameObject = m_vecGameObject[nX + nZ*m_nMapSize];

		if (setGameObject.size() > 0)
		{
			GameObject = *setGameObject.begin();
			//GameObject = vecGameObject[0];
		}
	}
	D3DXVECTOR3 ret = GameObject->GetPosition();
	SAFE_RELEASE(GameObject);
	return ret;
}

D3DXVECTOR3 cGridSystem::GetTileCenterCoord(int nX, int nZ)
{
	D3DXVECTOR3 vCenter(0.0f, 0.0f, 0.0f);

	//�Ʒ� �ּ��� ��ǥ �Է½� �ش� Ÿ���� �߽���ǥ�� ã���ְ� �ϴ°�
	//int x = nX / m_nConstTileSize;
	//int z = nZ / m_nConstTileSize;

	//vCenter.x = m_nConstTileSize*(x + 0.5f); // x * m_nConstTileSize + m_nConstTileSize/2;
	//vCenter.z = m_nConstTileSize*(z + 0.5f); // z * m_nConstTileSize + m_nConstTileSize/2;

	//vCenter.x = m_nConstTileSize*(nX + 0.5f);
	//vCenter.z = m_nConstTileSize*(nZ + 0.5f);

	vCenter.x = nX / 2;
	vCenter.z = nZ / 2;

	return vCenter;
}

std::vector<cGameObject*> cGridSystem::GetAdjObject(int nX, int nZ)
{
	std::vector<cGameObject*> vecGameObject;
	std::set<cGameObject*> setGameObject;
	
	int x = nX - 1;
	int z = nZ - 1;

	if (nX < 1) x = nX;
	if (nZ < 1)	z = nZ;

	for (z; z < nZ + 1; z++)
	{
		for (x; x < nX + 1; x++)
		{
			std::set<cGameObject*> setGO;
			setGO = m_vecTileData[x + z*m_nMapSize];

			if (setGO.size() > 0)
			{
				std::vector<cGameObject*> vecGO;
				vecGO = m_vecGameObject[x + z*m_nMapSize];

				for (int i = 0; i < setGO.size(); i++)
				{
					setGameObject.insert(vecGO[i]);
					
					// vector�� ������� Ȯ�� ������� �ٷ� �־���
					if (vecGameObject.size() == 0)
					{
						vecGameObject.push_back(vecGO[i]);
					}

					// ���� object�� ��� pass 
					// ���� object�� �ƴϸ� add
					else if (setGameObject.find(vecGO[i]) == setGameObject.end())
					{
						vecGameObject.push_back(vecGO[i]);
					}

				}
			}
		}
		x = nX - 1;
		if (nX < 1) x = nX;
	}

	return vecGameObject;
}

void cGridSystem::RemoveObejctOnTile(cGameObject* pGameObejct, int nX, int nZ)
{
	D3DXVECTOR3 vec = GetObjectCenter(nX, nZ);

	int nCenterX = (int)vec.x;
	int nCenterZ = (int)vec.z;

	if (m_vecTileData[nCenterX + nCenterZ*m_nMapSize].count(pGameObejct) > 0)
	{
		//cASEInstance* AseObject = new cASEInstance;

		//AseObject = (cASEInstance*)*GetObjectOnGrid(nX, nZ).begin();

		cGameObject* GameObject = new cGameObject;

		GameObject = *GetObjectOnGrid(nX, nZ).begin();
		int nWidth = 1;
		int nHeight = 1;
		if (GameObject->GetBoundingBox()){
			ST_BOUNDING_BOX *stBoundingBox = GameObject->GetBoundingBox();
			nWidth = (int)(stBoundingBox->vMax.x - stBoundingBox->vMin.x);
			nHeight = (int)(stBoundingBox->vMax.z - stBoundingBox->vMin.z);
		}

		int nBeginGridX = nCenterX - nWidth / 2;
		int nBeginGridZ = nCenterZ - nHeight / 2;

		for (int z = nBeginGridZ; z < nBeginGridZ + nHeight; z++)
		{
			for (int x = nBeginGridX; x < nBeginGridX + nWidth; x++)
			{
				if (m_vecTileData[x + z*m_nMapSize].find(pGameObejct)
					!= m_vecTileData[x + z*m_nMapSize].end())
				{
					m_vecTileData[x + z*m_nMapSize].erase(pGameObejct);
					pGameObejct->Release();
				}
			}
		}
	}
	else
	{
		return;
	}
}

//void cGridSystem::RenderSetup()
//{
//	D3DXCreateBox(g_pD3DDevice, 10.f, 10.f, 10.f, &m_pMesh, NULL);
//}
//
//void cGridSystem::Render()
//{
//	//D3DXMatrixIdentity(&mat);
//	for (int i = 0; i < m_vecTileData[i].size(); i++)
//	{
//		if (m_vecTileData[i].size() > 0)
//		{
//			D3DXMatrixTranslation(&m_mat, i / 256, 0, i % 256);
//			g_pD3DDevice->SetTransform(D3DTS_WORLD, &m_mat);
//			m_pMesh->DrawSubset(0);
//		}
//	}
//}

void cGridSystem::Destroy(){
	for (size_t i = 0; i < m_vecTileData.size(); i++){
		for (auto p : m_vecTileData[i]){
			p->Release();
		}
	}
}

void cGridSystem::AddMovingObject(cGameObject* pGameObejct, int nX, int nZ){
	SAFE_ADD_REF(pGameObejct);
	m_vecTileData[nX + nZ*m_nMapSize].insert(pGameObejct);
	m_vecGameObject[nX + nZ*m_nMapSize].push_back(pGameObejct);
	SAFE_ADD_REF(pGameObejct);
}

void cGridSystem::RemoveMovingObject(cGameObject* pGameObejct, int nX, int nZ){
	m_vecTileData[nX + nZ*m_nMapSize].erase(pGameObejct);
	pGameObejct->Release();
}