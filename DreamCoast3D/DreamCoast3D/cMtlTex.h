#pragma once
class cMtlTex : public cObject
{
public:
	D3DMATERIAL9		stMtl;
	LPDIRECT3DTEXTURE9	pTex;

public:
	cMtlTex(void);
	virtual ~cMtlTex(void);

	int					m_nAttrId;
};