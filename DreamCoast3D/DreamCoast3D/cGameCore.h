#pragma once
class cSceneManager;

class cGameCore : public cObject
{
private:
	cSceneManager* m_pSceneManager;

public:
	cGameCore();
	virtual ~cGameCore();

	void Setup();
	void Update(float fDelta);
	void Render();

};

