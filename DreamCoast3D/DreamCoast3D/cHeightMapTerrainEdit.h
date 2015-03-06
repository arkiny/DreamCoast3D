#pragma once
#include "cGameMapHeight.h"

class cMousePicking;

class cHeightMapTerrainEdit : public cGameMapHeight
{
protected:
	IDirect3DVertexBuffer9*		m_pVertexBuffer;
	IDirect3DIndexBuffer9*		m_pIndexBuffer;

public:
	cHeightMapTerrainEdit();
	virtual ~cHeightMapTerrainEdit();

	virtual void Setup();
	virtual void Update(float fDelta);
	virtual void Render();

	virtual void ChangeMapYVertexCoord(D3DXVECTOR2 vMin, D3DXVECTOR2 vMax, float fDelta);
	virtual std::vector<ST_PNT_VERTEX>& GetVertexVectorByRef() { return m_vecVertex; }
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

	eClickMessage eClick;
	D3DXVECTOR3 m_vClickFrom;
	D3DXVECTOR3 m_vClickTo;

	D3DXVECTOR3 m_vTileFrom;
	D3DXVECTOR3 m_vTileTo;

	float m_fHeight;
	float m_fWidth;

	cMousePicking*			m_pMousPicking;

private:
	void MouseUpdate();
	void MouseRangeUpdate();	
	void TileRangeUpdate();
	// 잠시 대기
	void SetClickRange(D3DXVECTOR3 vFrom, D3DXVECTOR3 vTo);

	D3DXVECTOR3 Linear(D3DXVECTOR3 vFrom, D3DXVECTOR3 vTo, float fDelta);
	D3DXVECTOR3 Bazier(D3DXVECTOR3 vFirst, D3DXVECTOR3 vSecond, D3DXVECTOR3 vThird, float fDelta);
};

