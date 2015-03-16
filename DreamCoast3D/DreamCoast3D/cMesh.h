#pragma once

class cMesh : public cObject
{
private:
	LPD3DXMESH				m_pMesh = NULL;
	DWORD					m_dwMaterial = 0;
	D3DMATERIAL9*			m_pMeshMaterials = NULL;
	LPDIRECT3DTEXTURE9*		m_ppMeshTextures = NULL;
	LPDIRECT3DTEXTURE9		m_pTrTextures = NULL;

public:
	cMesh();
	virtual ~cMesh();

	virtual void LoadFromFile(std::string sPath);
	virtual void Render(D3DXMATRIXA16* pmat);

	LPD3DXMESH			GetMesh()				{ return m_pMesh;				}
	DWORD				GetNumMaterials()		{ return m_dwMaterial;			}
	D3DMATERIAL9*		GetMeshMaterial(int i)	{ return &m_pMeshMaterials[i];	}
	LPDIRECT3DTEXTURE9	GetMeshTexture(int i)	{ return m_ppMeshTextures[i];	}
};

