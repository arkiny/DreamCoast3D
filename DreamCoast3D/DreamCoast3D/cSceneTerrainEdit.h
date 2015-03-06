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

	// Start���� ���ҽ��� �ε��Ѵ�.
	virtual void Start();
	virtual void Update(float delta);
	virtual void Render();

	virtual void Exit();
};

