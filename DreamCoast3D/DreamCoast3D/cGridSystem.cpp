#include "stdafx.h"
#include "cGridSystem.h"
#include <assert.h>


cGridSystem::cGridSystem()
	: m_nMapSize(0)
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
	// Object ���� X,Z ��ǥ
	int nBeginGridX = nX - nObjectWidth / 2;
	int nBeginGridZ = nZ - nObjectHeight / 2;

	for (int z = nBeginGridZ; z < nBeginGridZ + nObjectWidth; z++)
	{
		for (int x = nBeginGridX; x < nBeginGridX + nObjectWidth; x++)
		{
			// �׸��� ���� ���� Object�� �ִ��� Ȯ��
			if (m_vecTileData[x + z*m_nMapSize].find(pGameObejct)
				== m_vecTileData[x + z*m_nMapSize].end())
			{
				m_vecTileData[x + z*m_nMapSize].insert(pGameObejct);
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

void cGridSystem::RemoveObejctOnTile(cGameObject* pGameObejct, int nX, int nZ)
{

}