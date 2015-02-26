#include "stdafx.h"
#include "cAstarNode.h"

cAstarNode::cAstarNode()
	: m_nTile(0)
	, m_nTileSize(0)
	, m_pParent(NULL)
{
}


cAstarNode::~cAstarNode()
{
}


void cAstarNode::Setup(int nIndex, eNodeType e)
{
	m_nRow = nIndex / m_nTile;
	m_nCol = nIndex % m_nTile;
	m_eNodeType = e;

	m_rc.left = m_nCol * m_nTileSize;
	m_rc.top = m_nRow * m_nTileSize;
	m_rc.right = m_nCol * m_nTileSize + m_nTileSize;
	m_rc.bottom = m_nRow * m_nTileSize + m_nTileSize;
}