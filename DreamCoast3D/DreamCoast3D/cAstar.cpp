#include "stdafx.h"
#include "cAstar.h"
#include "cGridSystem.h"


cAstar::cAstar()
	: m_nTile()
	, m_nTileSize()
{
}


cAstar::~cAstar()
{
	for each(auto p in m_vecNode)
	{
		delete p;
	}
}

void cAstar::Setup(int nTile, int nTileSize, std::vector<std::set<cGameObject*>> vecGameObject)
{
	m_nTile = nTile;
	m_nTileSize = nTileSize;

	m_vecDirection.push_back(E_LEFT);
	m_vecDirection.push_back(E_RIGHT);
	m_vecDirection.push_back(E_UP);
	m_vecDirection.push_back(E_DOWN);
	m_vecDirection.push_back(E_LEFT | E_UP);
	m_vecDirection.push_back(E_LEFT | E_DOWN);
	m_vecDirection.push_back(E_RIGHT | E_UP);
	m_vecDirection.push_back(E_RIGHT | E_DOWN);

	m_mapDistance[E_LEFT] = 1.0f;
	m_mapDistance[E_RIGHT] = 1.0f;
	m_mapDistance[E_UP] = 1.0f;
	m_mapDistance[E_DOWN] = 1.0f;
	m_mapDistance[E_LEFT | E_UP] = sqrt(2.0f);
	m_mapDistance[E_LEFT | E_DOWN] = sqrt(2.0f);
	m_mapDistance[E_RIGHT | E_UP] = sqrt(2.0f);
	m_mapDistance[E_RIGHT | E_DOWN] = sqrt(2.0f);

	int nSize = vecGameObject.size();

	for (int i = 0; i < nSize; i++)
	{
		cAstarNode* pAstarNode = new cAstarNode;

		pAstarNode->m_nTile = m_nTile;
		pAstarNode->m_nTileSize = m_nTileSize;

		if (vecGameObject[i].size() == 0)
		{
			pAstarNode->m_eNodeType = cAstarNode::E_EMPTY;
		}
		else if (vecGameObject[i].size() >= 1)
		{
			pAstarNode->m_eNodeType = cAstarNode::E_WALL;
		}
		else
		{
			return;
		}
		pAstarNode->Setup(i, pAstarNode->m_eNodeType);
		m_vecNode.push_back(pAstarNode);
	}
}


void cAstar::FindRoute(D3DXVECTOR3 vFrom, D3DXVECTOR3 vTo)
{
	cAstarNode* pStartNode = m_vecNode[(int)vFrom.x + (int)vFrom.z*m_nTile];
	cAstarNode* pDestNode = m_vecNode[(int)vTo.x + (int)vTo.z*m_nTile];

	assert(pStartNode && pDestNode);

	pStartNode->m_fG = 0.0f;
	pStartNode->m_fH = CalcHeuristic(pStartNode, pDestNode);
	pStartNode->m_fF = pStartNode->m_fG + pStartNode->m_fH;

	m_setOpenList.insert(pStartNode);

	while (true)
	{
		cAstarNode* pMinFNode = GetMinFNodeAtOpenList();
		if (!pMinFNode)
		{
			// 길 없음.
			MarkNodeType(pDestNode);
			break;
		}

		m_setOpenList.erase(pMinFNode);
		m_setCloseList.insert(pMinFNode);

		Extend(pMinFNode, pDestNode);

		if (pMinFNode == pDestNode)
		{
			// 길 찾음.
			m_pDestNode = new cAstarNode;
			m_pDestNode = pMinFNode;
			MarkNodeType(pDestNode);
			break;
		}
	}
}

std::vector<D3DXVECTOR3> cAstar::GetRoute()
{
	std::vector<D3DXVECTOR3> vecRoute;
	D3DXVECTOR3 v(0.f, 0.f, 0.f);
	while (m_pDestNode->m_pParent != nullptr)
	{
		v.x = m_pDestNode->m_nCol;
		v.z = m_pDestNode->m_nRow;

		vecRoute.push_back(v);
		m_pDestNode = m_pDestNode->m_pParent;
	}
	return vecRoute;
	
}

cAstarNode* cAstar::FindNode(cAstarNode::eNodeType e)
{
	for each(auto p in m_vecNode)
	{
		if (p->m_eNodeType == e)
			return p;
	}
	return NULL;
}

float cAstar::CalcHeuristic(cAstarNode* pNode1, cAstarNode* pNode2)
{
	return fabs(pNode1->m_nCol - (float)pNode2->m_nCol) +
		fabs(pNode1->m_nRow - (float)pNode2->m_nRow);
}

cAstarNode* cAstar::GetMinFNodeAtOpenList()
{
	float fMinF = FLT_MAX;
	cAstarNode* pMinFNode = NULL;

	for each(auto n in m_setOpenList)
	{
		if (n->m_fF < fMinF)
		{
			pMinFNode = n;
			fMinF = n->m_fF;
		}
	}

	return pMinFNode;
}

void cAstar::Extend(cAstarNode* pCurrNode, cAstarNode* pDestNode)
{
	for each(auto d in m_vecDirection)
	{
		cAstarNode* pAdjNode = GetAdjNode(pCurrNode, d);

		if (pAdjNode == NULL)
			continue;

		if (IsInList(pAdjNode, m_setOpenList))
		{
			if (pAdjNode->m_fG > pCurrNode->m_fG + m_mapDistance[d])
			{
				pAdjNode->m_pParent = pCurrNode;
				pAdjNode->m_fG = pCurrNode->m_fG + m_mapDistance[d];
				//pAdjNode->m_fH = CalcHeuristic(pAdjNode, pDestNode);
				pAdjNode->m_fF = pAdjNode->m_fG + pAdjNode->m_fH;
			}
		}
		else
		{
			pAdjNode->m_pParent = pCurrNode;
			pAdjNode->m_fG = pCurrNode->m_fG + m_mapDistance[d];
			pAdjNode->m_fH = CalcHeuristic(pAdjNode, pDestNode);
			pAdjNode->m_fF = pAdjNode->m_fG + pAdjNode->m_fH;
			m_setOpenList.insert(pAdjNode);
		}
	}
}

cAstarNode* cAstar::GetAdjNode(cAstarNode* pCurrNode, int nDirection)
{
	int nAdjRow = pCurrNode->m_nRow;
	int nAdjCol = pCurrNode->m_nCol;

	if (nDirection & E_LEFT)
	{
		if (pCurrNode->m_nCol == 0)
			return NULL;

		int nIndex = pCurrNode->m_nRow * m_nTile + pCurrNode->m_nCol - 1;
		if (m_vecNode[nIndex]->m_eNodeType == cAstarNode::E_WALL)
			return NULL;

		nAdjCol -= 1;
	}

	if (nDirection & E_RIGHT)
	{
		if (pCurrNode->m_nCol == m_nTile - 1)
			return NULL;

		int nIndex = pCurrNode->m_nRow * m_nTile + pCurrNode->m_nCol + 1;
		if (m_vecNode[nIndex]->m_eNodeType == cAstarNode::E_WALL)
			return NULL;

		nAdjCol += 1;
	}

	if (nDirection & E_UP)
	{
		if (pCurrNode->m_nRow == 0)
			return NULL;

		int nIndex = pCurrNode->m_nRow * m_nTile + pCurrNode->m_nCol - m_nTile;
		if (m_vecNode[nIndex]->m_eNodeType == cAstarNode::E_WALL)
			return NULL;

		nAdjRow -= 1;
	}

	if (nDirection & E_DOWN)
	{
		if (pCurrNode->m_nRow == m_nTile - 1)
			return NULL;

		int nIndex = pCurrNode->m_nRow * m_nTile + pCurrNode->m_nCol + m_nTile;
		if (m_vecNode[nIndex]->m_eNodeType == cAstarNode::E_WALL)
			return NULL;

		nAdjRow += 1;
	}

	int nIndex = nAdjRow * m_nTile + nAdjCol;
	if (m_vecNode[nIndex]->m_eNodeType == cAstarNode::E_WALL)
		return NULL;

	if (IsInList(m_vecNode[nIndex], m_setCloseList))
		return NULL;

	return m_vecNode[nIndex];
}

bool cAstar::IsInList(cAstarNode* pNode, std::set<cAstarNode*>& set)
{
	return set.find(pNode) != set.end();
}

void cAstar::MarkNodeType(cAstarNode* pDestNode)
{
	for each(cAstarNode* n in m_vecNode)
	{
		if (IsInList(n, m_setOpenList))
		{
			n->m_eNodeType = cAstarNode::E_OPEN;
		}

		if (IsInList(n, m_setCloseList))
		{
			n->m_eNodeType = cAstarNode::E_CLOSE;
		}

		cAstarNode* pTemp = pDestNode;
		while (pTemp)
		{
			pTemp->m_eNodeType = cAstarNode::E_PATH;
			pTemp = pTemp->m_pParent;
		}
	}
}