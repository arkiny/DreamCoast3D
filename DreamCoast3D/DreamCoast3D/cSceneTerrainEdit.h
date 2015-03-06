#pragma once
#include "cScene.h"

class cMousePicking;

class cSceneTerrainEdit : public cScene
{
protected:
	//cHeightMapTerrainEdit*	m_pHeightMapEdit;
	cMousePicking*			m_pMousPicking;

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

