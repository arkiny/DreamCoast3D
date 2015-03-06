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
	//virtual void 

protected:

	cMousePicking* m_pMousPicking;
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
	void MouseUpdate();
	void MouseRangeUpdate();
	eClickMessage eClick;
	D3DXVECTOR3 m_vClickFrom;
	D3DXVECTOR3 m_vClickTo;

	D3DXVECTOR3 m_vTileFrom;
	D3DXVECTOR3 m_vTileTo;

	void TileRangeUpdate();
	float m_fHeight;
	float m_fWidth;

	// 잠시 대기
	void SetClickRange(D3DXVECTOR3 vFrom, D3DXVECTOR3 vTo);
};

