#pragma once
#include "cScene.h"

class cMousePicking;
class cEditCursor;
class cHeightMapTerrainEdit;

class cSceneTerrainEdit : public cScene
{
protected:
	//cHeightMapTerrainEdit*	m_pHeightMapEdit;
	cMousePicking*			m_pMousPicking;
	cEditCursor*			m_pCursor;

	cHeightMapTerrainEdit*	m_pEditMap;

	bool					m_bIsClickDown = false;
	bool					m_bIsClickUP = false;

	float					m_fDelayAcuum = 2.0f;
	const float				m_fDelay = 0.5f;

public:
	cSceneTerrainEdit();
	virtual ~cSceneTerrainEdit();

	virtual void Setup(std::string sFilePath);

	// Start에서 리소스를 로딩한다.
	virtual void Start();
	virtual void Update(float delta);
	virtual void Render();

	virtual void Exit();
};

