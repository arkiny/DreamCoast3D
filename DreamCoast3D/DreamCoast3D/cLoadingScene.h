#pragma once
#include "cScene.h"

LPD3DXEFFECT LoadShader(const char * filename);
LPDIRECT3DTEXTURE9 LoadTexture(const char * filename);
LPD3DXMESH LoadModel(const char * filename);

class cUIImageView;

class cLoadingScene : public cScene
{
protected:
	volatile int m_nLoadCount = 0;
	volatile bool m_bBoolWorkDone = false;
	LPD3DXSPRITE m_pSprite = NULL;

	float		m_fLoadingTime = 0.0f;
	LPD3DXFONT	m_pFont;
	RECT		m_recFontRect;

	SYNTHESIZE(std::string, m_sNextScenePath, NextScenePath);

	LPDIRECT3DSURFACE9 pHWBackBuffer = NULL;
	LPDIRECT3DSURFACE9 pHWDepthStencilBuffer = NULL;

	cUIImageView* pUIImageView = NULL;

protected:
	static void LoadNextScene(LPVOID pParam);

public:
	cLoadingScene();
	virtual ~cLoadingScene();

	virtual void Setup(std::string sNextScene);
	
	virtual void Start() override;
	virtual void Update(float fDelta) override;
	virtual void Render() override;

	virtual void Exit() override;
	virtual void SetWorkDone(bool bDone){ m_bBoolWorkDone = bDone; }
};

