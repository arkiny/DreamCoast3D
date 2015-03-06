#pragma once
#include "cGameMapHeight.h"

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
};

