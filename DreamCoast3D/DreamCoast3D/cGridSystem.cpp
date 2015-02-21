#include "stdafx.h"
#include "cGridSystem.h"
#include <assert.h>


cGridSystem::cGridSystem()
	: m_nMapSize(0)
	, m_nConstTileSize(0.0f)
{
}


cGridSystem::~cGridSystem()
{
}

void cGridSystem::Setup(int nMapSize)
{
	int nTotalTile = nMapSize * nMapSize;

	m_nMapSize = nMapSize;

	for (int i = 0; i < nTotalTile; i++)
	{
		std::set<cGameObject*> setGameObject;
		m_vecTileData.push_back(setGameObject);
	}
}

void cGridSystem::AddObjectOnGrid(cGameObject* pGameObejct, int nX, int nZ, int nObjectWidth, int nObjectHeight)
{
	//int GridX = nX * m_nConstTileSize;
	//int GridZ = nZ * m_nConstTileSize;
	// Object 시작 X,Z 좌표
	int nBeginGridX = nX - nObjectWidth / 2;
	int nBeginGridZ = nZ - nObjectHeight / 2;

	for (int z = nBeginGridZ; z < nBeginGridZ + nObjectWidth; z++)
	{
		for (int x = nBeginGridX; x < nBeginGridX + nObjectWidth; x++)
		{
			// 그리드 위에 같은 Object가 있는지 확인
			if (m_vecTileData[x + z*m_nMapSize].find(pGameObejct)
				== m_vecTileData[x + z*m_nMapSize].end())
			{
				m_vecTileData[x + z*m_nMapSize].insert(pGameObejct);
				m_vecGameObject[x + z*m_nMapSize].push_back(pGameObejct);
			}
			else
			{
				assert(m_vecTileData[x + z*m_nMapSize].find(pGameObejct)
					== m_vecTileData[x + z*m_nMapSize].end() && "이미 추가된 오브젝트 입니다");
			}
		}
	}
}

std::set<cGameObject*> cGridSystem::GetObjectOnGrid(int nX, int nZ)
{
	return m_vecTileData[nX + nZ*m_nMapSize];
}

void cGridSystem::RemoveObejctOnTile(cGameObject* pGameObejct, int nX, int nZ)
{

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
	return GameObject->GetPosition();
}

D3DXVECTOR3 cGridSystem::GetTileCenterCoord(int nX, int nZ)
{
	D3DXVECTOR3 vCenter(0.0f, 0.0f, 0.0f);

	int x = nX / m_nConstTileSize;
	int z = nZ / m_nConstTileSize;

	vCenter.x = m_nConstTileSize*(x + 0.5); // x * m_nConstTileSize + m_nConstTileSize/2;
	vCenter.z = m_nConstTileSize*(z + 0.5); // z * m_nConstTileSize + m_nConstTileSize/2;

	return vCenter;
}

std::vector<cGameObject*> cGridSystem::GetAdjObject(int nX, int nZ)
{
	std::vector<cGameObject*> vecGameObject;
	
	int x = nX - 1;
	int z = nZ - 1;

	if (nX < 1) x = nX;
	if (nZ < 1)	z = nZ;

	for (z; z < nZ + 1; z++)
	{
		for (x; x < nX + 1; x++)
		{
			std::set<cGameObject*> setGameObject;
			setGameObject = m_vecTileData[x + z*m_nMapSize];

			if (setGameObject.size() > 0)
			{
				std::vector<cGameObject*> vecGO;
				vecGO = m_vecGameObject[x + z*m_nMapSize];

				for (int i = 0; i < setGameObject.size(); i++)
				{
					vecGameObject.push_back(vecGO[i]);
				}
			}
		}
	}

	return vecGameObject;
}
