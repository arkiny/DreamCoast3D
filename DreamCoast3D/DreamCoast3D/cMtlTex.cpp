#include "stdafx.h"
#include "cMtlTex.h"

cMtlTex::cMtlTex(void)
	: cObject(std::string("cMtlTex"))
	, pTex(NULL)
	, m_nAttrId(0)
{
	ZeroMemory(&stMtl, sizeof(D3DMATERIAL9));
}


cMtlTex::~cMtlTex(void)
{
}
