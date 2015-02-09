#pragma once

class cCamera;
class cLightSource;
class cGameObject;
class cGameObjManager;

// 앞으로의 모든 씬은 cScene을 상속받아서 만들어진다.
class cScene : public cObject
{
protected:
	SYNTHESIZE(iSceneDelegate*, m_pDelegate, Delegate);
	SYNTHESIZE(cScene*, m_pNextScene, NextScene);

	// 게임 오브젝트들을 가지고 처리해주는 매니저
	cGameObjManager*			m_pGameObjManager;

	// 한개의 씬은 여러개의 광원(최대 8개)을 가질수 있다.
	std::vector<cLightSource*>	m_vecLightSources;

	// 각 씬은 각 씬에 맞는 다른 카메라를 가지고 있다.
	// 여러개의 카메라를 가지는 것은 차후 고민해서 추가
	cCamera*					m_pCamera;
	

public:
	cScene();
	virtual ~cScene();

	// 차후 Setup의 인자로 해당 씬에 관련된 파일 주소를 받아와 해당씬의 모든 리소스 파일들을 
	// 로딩하는 데에 이용한다.
	// 혹은 로딩화면을 위해 쓰인다.
	virtual void Setup(std::string sFilePath);
	virtual void Update(float delta);
	virtual void Render();

	virtual void AddGameObj(cGameObject* pGameObj);
	virtual void Destroy();
};

