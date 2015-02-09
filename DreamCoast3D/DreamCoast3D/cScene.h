#pragma once

class cGameObject;

class cScene : public cObject
{
private:
	SYNTHESIZE(iSceneDelegate*, m_pDelegate, Delegate);
	SYNTHESIZE(cScene*, m_pNextScene, NextScene);

	// 해당 씬에서 렌더/업데이트 할 오브젝트들
	std::vector<cGameObject*> m_vecGameObjects;

public:
	cScene();
	~cScene();

	// 차후 Setup의 인자로 해당 씬에 관련된 파일 주소를 받아와 해당씬의 모든 리소스 파일들을 
	// 로딩하는 데에 이용한다.
	// 혹은 로딩화면을 위해 쓰인다.
	void Setup(std::string sFilePath);
	void Update(float delta);
	void Render();
};

