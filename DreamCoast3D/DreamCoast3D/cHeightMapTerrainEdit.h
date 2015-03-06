#pragma once
#include "cGameMapHeight.h"

class cMousePicking;

class cHeightMapTerrainEdit : public cGameMapHeight
{
protected:
	IDirect3DVertexBuffer9*		m_pVertexBuffer;
	IDirect3DIndexBuffer9*		m_pIndexBuffer;

	LPD3DXMESH					m_pBoxMesh = NULL;
	D3DXVECTOR3					m_vBoxPos;
	D3DXVECTOR3					m_vBoxScale;
	D3DMATERIAL9				m_stBoxMtl;

public:
	cHeightMapTerrainEdit();
	virtual ~cHeightMapTerrainEdit();

	virtual void Setup();
	virtual void Update(float fDelta);
	virtual void Render();

	virtual void ChangeMapYVertexCoord(D3DXVECTOR2 vMin, D3DXVECTOR2 vMax, float fDelta);
	virtual std::vector<ST_PNT_VERTEX>& GetVertexVectorByRef() { return m_vecVertex; }
	virtual void SaveToRawFile();
	//virtual void 

private:
	// MS
	bool m_isRclick;

	enum eClickMessage
	{
		E_ClickOn,
		E_Clicking,
		E_ClickOff,
		E_ClickEmpty,
		E_Max,
	};

	SYNTHESIZE(D3DXVECTOR3, m_vClickFrom, ClickFrom);
	SYNTHESIZE(D3DXVECTOR3, m_vClickTo, ClickTo);
	SYNTHESIZE(D3DXVECTOR3, m_vTileFrom, TileFrom);
	SYNTHESIZE(D3DXVECTOR3, m_vTileTo, TileTo);
	SYNTHESIZE(float, m_fHeight, Height);
	SYNTHESIZE(float, m_fWidth, Width);
	eClickMessage eClick;
	


	//cMousePicking*			m_pMousPicking;

private:
	// 잠시 대기
	void SetClickRange(D3DXVECTOR3 vFrom, D3DXVECTOR3 vTo);

	D3DXVECTOR3 Linear(D3DXVECTOR3 vFrom, D3DXVECTOR3 vTo, float fDelta);
	D3DXVECTOR3 Bazier(D3DXVECTOR3 vFirst, D3DXVECTOR3 vSecond, D3DXVECTOR3 vThird, float fDelta);
};

