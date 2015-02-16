#pragma once
#include "cGameMapObject.h"
class cGameMapHeight : public cGameMapObject
{
protected:
	std::vector<ST_PNT_VERTEX>	m_vecVertex;
	LPDIRECT3DTEXTURE9			m_pTexture;
	
	int							m_nTileN				= 256;
	int							m_nBytePerColor			= 1;

	LPD3DXMESH					m_pMesh;
	D3DMATERIAL9				m_stMtl;

public:
	cGameMapHeight();
	virtual ~cGameMapHeight();

	virtual void LoadFromFiles(char* szFilename, char* szTextureFilename);
	virtual void LoadFromFiles(std::string sFilename, std::string sTextureFilename);
	//virtual void Update(float fDelta);
	virtual void Render();

	// iMap Override
	//�� ���� ���� ��� ���� �ٸ��Ƿ� ���� �������̵� ������Ѵ�.
	virtual float GetHeight(OUT bool& isLand, IN D3DXVECTOR3* pvPosition) override;

	virtual std::vector<ST_PNT_VERTEX>& GetVertexVectorByRef() { return m_vecVertex; }
	virtual void setTileN(int nTileN) { m_nTileN = nTileN; }
	virtual void setBytePerColr(int nByte) { m_nBytePerColor = nByte; }

};
