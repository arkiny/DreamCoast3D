#pragma once
class cAstarNode
{
public:
	cAstarNode();
	~cAstarNode();

	enum eNodeType
	{
		E_NONE,
		E_EMPTY,
		E_WALL,
		E_START,
		E_DEST,
		E_PATH,
		E_OPEN,
		E_CLOSE,
	};

	float			m_fF;
	float			m_fG;
	float			m_fH;
	eNodeType		m_eNodeType;
	int				m_nRow;
	int				m_nCol;
	cAstarNode*		m_pParent;
	RECT			m_rc;
	int				m_nTile;
	int				m_nTileSize;

	void Setup(int nIndex, eNodeType e);
};

