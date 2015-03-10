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
	int nWidth = 0.0f;
	int nHeight = 0.0f;

	if (pGameObejct->GetBoundingBox()){
		ST_BOUNDING_BOX* stBox = pGameObejct->GetBoundingBox();
		nWidth = (int)(stBox->vMax.x - stBox->vMin.x);
		nHeight = (int)(stBox->vMax.z - stBox->vMin.z);
	}

	int nBeginGridX = nX - ceil(nWidth / 2);
	int nBeginGridZ = nZ - ceil(nHeight / 2);

	int SizeX = ceil(nWidth / 2);
	int SizeZ = ceil(nHeight / 2);

	for (int z = nBeginGridZ; z < nBeginGridZ + nWidth; z++)
	{
		for (int x = nBeginGridX; x < nBeginGridX + nHeight; x++)
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
				m_vecTileData[x + z*m_nMapSize].insert(pGameObejct);
				//assert(m_vecTileData[x + z*m_nMapSize].find(pGameObejct)
				//	== m_vecTileData[x + z*m_nMapSize].end() && "이미 추가된 오브젝트 입니다");
			}
		}
	}
}

void cGridSystem::AddObjectCustomer(cGameObject* pGameObejct, int nX, int nZ, int nHeight, int nWidth)
{
	int nBeginGridX = nX - ceil(nWidth / 2);
	int nBeginGridZ = nZ - ceil(nHeight / 2);

	for (int z = nBeginGridZ; z < nBeginGridZ + nWidth; z++)
	{
		for (int x = nBeginGridX; x < nBeginGridX + nHeight; x++)
		{
			m_vecTileData[x + z*m_nMapSize].insert(pGameObejct);
		}
	}
}

std::set<cGameObject*> cGridSystem::GetObjectOnGrid(int nX, int nZ)
{
	return m_vecTileData[nX + nZ*m_nMapSize];
}

std::vector<cGameObject*> cGridSystem::GetObjectOnGridVec(int nX, int nZ)
{
	return m_vecGameObject[nX + nZ*m_nMapSize];
}

D3DXVECTOR3 cGridSystem::GetObjectCenter(int nX, int nZ)
{
	cGameObject* GameObject = NULL;

	if (m_vecTileData[nX + nZ*m_nMapSize].size() > 0)
	{
		std::set<cGameObject*> setGameObject;
		setGameObject = m_vecTileData[nX + nZ*m_nMapSize];

		if (setGameObject.size() > 0)
		{
			GameObject = *setGameObject.begin();
		}
	}
	D3DXVECTOR3 ret = GameObject->GetPosition();
	return ret;
}

D3DXVECTOR3 cGridSystem::GetTileCenterCoord(int nX, int nZ)
{
	D3DXVECTOR3 vCenter(0.0f, 0.0f, 0.0f);

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

	for (z; z <= nZ + 1; z++)
	{
		for (x; x <= nX + 1; x++)
		{
			std::set<cGameObject*> setGO;
			setGO = m_vecTileData[x + z*m_nMapSize];

			if (setGO.size() > 0)
			{
				for (auto p : setGO)
				{
					setGameObject.insert(p);
				}
			}
		}
		x = nX - 1;
		if (nX < 1) x = nX;
	}

	for (auto p : setGameObject)
	{
		vecGameObject.push_back(p);
	}

	return vecGameObject;
}

std::vector<cGameObject*> cGridSystem::GetAdjObjectCustomer(int nX, int nZ, int nSize)
{
	std::vector<cGameObject*> vecGameObject;
	std::set<cGameObject*> setGameObject;

	int x = nX - nSize;
	int z = nZ - nSize;

	if (nX < nSize) x = nX;
	if (nZ < nSize)	z = nZ;

	for (z; z <= nZ + nSize; z++)
	{
		for (x; x <= nX + nSize; x++)
		{
			std::set<cGameObject*> setGO;
			setGO = m_vecTileData[x + z*m_nMapSize];

			if (setGO.size())
			{
				for (auto p : setGO)
				{
					setGameObject.insert(p);
				}
			}

		}
		x = nX - nSize;
		if (nX < nSize) x = nX;
	}

	for (auto p : setGameObject)
	{
		vecGameObject.push_back(p);
	}

	return vecGameObject;
}

void cGridSystem::RemoveObejctOnTile(cGameObject* pGameObejct, int nX, int nZ)
{
	D3DXVECTOR3 vec = GetObjectCenter(nX, nZ);

	int nCenterX = (int)ceil(vec.x);
	int nCenterZ = (int)ceil(vec.z);

	if (m_vecTileData[nCenterX + nCenterZ*m_nMapSize].count(pGameObejct) > 0)
	{
		while (m_vecTileData[nCenterX + nCenterZ*m_nMapSize].count(pGameObejct) != 0)
		{
			cGameObject* GameObject = NULL;
			GameObject = *GetObjectOnGrid(nX, nZ).find(pGameObejct);
			int nWidth = 0;
			int nHeight = 0;
			if (GameObject->GetBoundingBox()){
				ST_BOUNDING_BOX *stBoundingBox = GameObject->GetBoundingBox();
				nWidth = (int)(stBoundingBox->vMax.x - stBoundingBox->vMin.x);
				nHeight = (int)(stBoundingBox->vMax.z - stBoundingBox->vMin.z);
			}

			int nBeginGridX = nCenterX - ceil((float)nWidth / 2);
			int nBeginGridZ = nCenterZ - ceil((float)nHeight / 2);

			for (int z = nBeginGridZ; z <= nBeginGridZ + nHeight; z++)
			{
				for (int x = nBeginGridX; x <= nBeginGridX + nWidth; x++)
				{
					if (m_vecTileData[x + z*m_nMapSize].find(pGameObejct)
						!= m_vecTileData[x + z*m_nMapSize].end())
					{
						m_vecTileData[x + z*m_nMapSize].erase(pGameObejct);
					}
				}
			}
		}

	}
	else
	{
		return;
	}
}

void cGridSystem::Destroy(){
	for (size_t i = 0; i < m_vecTileData.size(); i++){
		for (auto p : m_vecTileData[i]){
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
}

std::vector<cGameObject*> cGridSystem::GetAdjObjectCircle(int nX, int nZ, int nRange)
{
	std::vector<POINT*> vecPoint;
	vecPoint.reserve(nRange*nRange * 4);
	vecPoint = GetAdjCircle(nX, nZ, nRange);
	std::vector<cGameObject*> vecGameObject;
	std::set<cGameObject*> setGameObject;


	for (int i = 0; i < vecPoint.size(); i++)
	{
		std::set<cGameObject*> setGO;
		setGO = GetObjectOnGrid(vecPoint[i]->x, vecPoint[i]->y);
		if (setGO.size())
		{
			for (auto p : setGO)
			{
				setGameObject.insert(p);
			}
		}
		//if (vecGO.size() > 0)
		//{
		//	for (int i = 0; i < vecGO.size(); i++)
		//	{
		//		if (vecGameObject.size() == 0)
		//		{
		//			vecGameObject.push_back(vecGO[i]);
		//		}
		//		else if (setGameObject.find(vecGO[i]) == setGameObject.end())
		//		{
		//			vecGameObject.push_back(vecGO[i]);
		//		}
		//		setGameObject.insert(vecGO[i]);
		//	}
		//}
	}

	for (auto p : setGameObject)
	{
		vecGameObject.push_back(p);
	}

	return vecGameObject;
}

std::vector<POINT*> cGridSystem::GetAdjCircle(int nX, int nZ, int nRange)
{
	int nRan = nRange;

	std::vector<POINT*> vecPoint;
	std::set<POINT*> setPoint;
	vecPoint.reserve(nRange*nRange * 4);
	int x = 0;
	int z = 0;

	int PosX = nX;
	int PosY = nZ;

	for (int i = 0; i < nRan; i++)
	{

		POINT* pos = new POINT;
		pos->x = x;
		pos->y = nRan - z;
		x += 1;
		z += 1;
		if (setPoint.find(pos) == setPoint.end())
		{
			vecPoint.push_back(pos);
		}
		setPoint.insert(pos);
	}
	x = nRan;
	z = nRan;
	for (int i = 0; i < nRan; i++)
	{
		POINT* pos = new POINT;
		pos->x = x;
		pos->y = -(nRan - z);
		x -= 1;
		z -= 1;

		if (setPoint.find(pos) == setPoint.end())
		{
			vecPoint.push_back(pos);
		}
		setPoint.insert(pos);
	}
	x = nRan;
	z = nRan;

	for (int i = 0; i < nRan; i++)
	{
		POINT* pos = new POINT;
		pos->x = -x;
		pos->y = +(nRan - z);
		x -= 1;
		z -= 1;

		if (setPoint.find(pos) == setPoint.end())
		{
			vecPoint.push_back(pos);
		}
		setPoint.insert(pos);
	}
	x = 0;
	z = 0;

	for (int i = 0; i < nRan; i++)
	{
		POINT* pos = new POINT;
		pos->x = -x;
		pos->y = -(nRan - z);
		x += 1;
		z += 1;

		if (setPoint.find(pos) == setPoint.end())
		{
			vecPoint.push_back(pos);
		}
		setPoint.insert(pos);
	}

	for (int i = 0; i < vecPoint.size(); i++)
	{
		vecPoint[i]->x += PosX;
		vecPoint[i]->y += PosY;
	}

	std::vector<POINT*> vecTest;
	vecTest = vecPoint;

	for (int i = 0; i < vecTest.size(); i++)
	{
		bool m_isTest = false;
		int nA = 0;
		int nB = 0;
		int x = vecTest[i]->x;
		int y = vecTest[i]->y;
		for (nB = y - nRan; nB < y + nRan; nB++)
		{
			for (nA = x - nRan; nA < x + nRan; nA++)
			{
				POINT* pos = new POINT;
				pos->x = nA;
				pos->y = nB;
				for (int n = 0; n < vecPoint.size(); n++)
				{
					if (vecPoint[n]->x == pos->x && vecPoint[n]->y == pos->y)
					{
						m_isTest = true;
						break;
					}
				}
				if (m_isTest == false)
				{
					vecPoint.push_back(pos);
				}
			}
		}
	}
	return vecPoint;
}