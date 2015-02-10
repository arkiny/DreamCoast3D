#pragma once
#include "cGameMapObject.h"
class cMapExample : public cGameMapObject
{
private:
	class cPyramid
	{
	private:
		LPDIRECT3DVERTEXBUFFER9		m_pVB;
		int							m_nNumTri;
		D3DXMATRIXA16				m_matWorld;
		
	public:
		cPyramid() : m_pVB(NULL), m_nNumTri(0) {}
		~cPyramid()
		{
			SAFE_RELEASE(m_pVB);
		}
		void Setup(D3DXMATRIXA16& mat, D3DCOLOR c);
		void Render();
	};

	D3DMATERIAL9				m_stMaterial;
	LPDIRECT3DVERTEXBUFFER9		m_pVB;
	int							m_nNumLine;
	std::vector<cPyramid*>		m_vecPyramid;

public:
	cMapExample();
	~cMapExample();
	virtual void Setup(int nHalfTile, float fWidth);
	virtual void Update(float fDelta);
	virtual void Render();
	virtual float GetHeight(OUT bool& isLand, IN D3DXVECTOR3* pvPosition) override;
};

