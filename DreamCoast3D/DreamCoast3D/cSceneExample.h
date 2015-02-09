#pragma once
#include "cScene.h"

// ¾À°ü·Ã ¿¹½Ã
class cSceneExample : public cScene
{
public:
	cSceneExample();
	virtual ~cSceneExample();
	
	virtual void Setup(std::string sFilePath);
	virtual void Update(float delta);
	virtual void Render();
};

