#include "stdafx.h"
#include "cBillingExample.h"
#include "cTransform.h"


cBillingExample::cBillingExample()
	: cGameBillBoardingObject()
	, m_pMesh(NULL)
{
	ZeroMemory(&m_stFontMtl, sizeof(D3DMATERIAL9));
}


cBillingExample::~cBillingExample()
{
	SAFE_RELEASE(m_pMesh);
}

void cBillingExample::Setup(){
	m_stFontMtl.Ambient = D3DXCOLOR(0.8f, 0.8f, 0.8f, 1.0f);
	m_stFontMtl.Diffuse = D3DXCOLOR(0.8f, 0.8f, 0.8f, 1.0f);
	m_stFontMtl.Specular = D3DXCOLOR(0.8f, 0.8f, 0.8f, 1.0f);

	HDC hdc = CreateCompatibleDC(0);
	LOGFONT lf;
	ZeroMemory(&lf, sizeof(LOGFONT));

	lf.lfHeight = 15;    // in logical units
	lf.lfWidth = 10;    // in logical units
	lf.lfEscapement = 0;
	lf.lfOrientation = 0;
	lf.lfWeight = 0;   // boldness, range 0(light) - 1000(bold)
	lf.lfItalic = false;
	lf.lfUnderline = false;
	lf.lfStrikeOut = false;
	lf.lfCharSet = DEFAULT_CHARSET;
	lf.lfOutPrecision = 0;
	lf.lfClipPrecision = 0;
	lf.lfQuality = 0;
	lf.lfPitchAndFamily = 0;
	strcpy_s(lf.lfFaceName, "Times New Roman"); // font style

	//
	// Create an ID3DXFont based on 'lf'.
	//
	HFONT hFont;
	HFONT hFontOld;
	hFont = CreateFontIndirect(&lf);
	hFontOld = (HFONT)SelectObject(hdc, hFont);

	std::string f = "BillingExample";
	D3DXCreateText(g_pD3DDevice, hdc, f.c_str(), 0.001f, 0.01f, &m_pMesh, 0, 0);

	SelectObject(hdc, hFontOld);
	DeleteObject(hFont);
	DeleteDC(hdc);
}

void cBillingExample::Update(float fDelta){
	cGameBillBoardingObject::Update(fDelta);
}

void cBillingExample::Render(){
	cGameBillBoardingObject::Render();
	g_pD3DDevice->SetMaterial(&m_stFontMtl);
	g_pD3DDevice->SetTexture(0, NULL);
	m_pMesh->DrawSubset(0);
}
