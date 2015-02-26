#pragma once
#include "cAstarNode.h"
class cAstar
{
private:
	enum eDirection
	{
		E_LEFT = 1 << 0,
		E_RIGHT = 1 << 1,
		E_UP = 1 << 2,
		E_DOWN = 1 << 3,
	};

	std::vector<cAstarNode*>	m_vecNode;
	std::set<cAstarNode*>		m_setOpenList;
	std::set<cAstarNode*>		m_setCloseList;
	std::vector<int>			m_vecDirection;
	std::map<int, float>		m_mapDistance;

	int m_nTile;
	int m_nTileSize;
	cAstarNode* m_pDestNode;

	cAstar();
	~cAstar();
	void MarkNodeType(cAstarNode* pDestNode);
	void Extend(cAstarNode* pCurrNode, cAstarNode* pDestNode);
	bool IsInList(cAstarNode* pNode, std::set<cAstarNode*>& set);
	float CalcHeuristic(cAstarNode* pNode1, cAstarNode* pNode2);
	cAstarNode*	FindNode(cAstarNode::eNodeType e);
	cAstarNode*	GetMinFNodeAtOpenList();
	cAstarNode*	GetAdjNode(cAstarNode* pCurrNode, int nDirection);

public:

	void Setup(int nTile, int nTileSize, std::vector<std::set<cGameObject*>> vecGameObject);
	void FindRoute(D3DXVECTOR3 vFrom, D3DXVECTOR3 vTo);
	std::vector<D3DXVECTOR3> GetRoute();
};

