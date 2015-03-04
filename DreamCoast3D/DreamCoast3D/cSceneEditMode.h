#pragma once
#include "cScene.h"

class cMousePicking;

class cSceneEditMode : public cScene
{
protected:
	std::map<std::string, cGameMapObject*> m_mapLoadedMap;
	std::vector<std::string> m_vecMapRawPath;
	std::vector<std::string> m_vecMapTexturePath;

	// 차후 멀티쓰레딩용
	volatile bool m_bMapLoaded = true;
	volatile bool m_bObjectLoaded = true;

	int m_nCurrentMapIndex = 0;
	RECT m_rectFontArea;

	cMousePicking* m_pMousPicking;

protected:
	static void LoadNextMap(LPVOID pParam);
	static void LoadNextObj(LPVOID pParam);

public:
	cSceneEditMode();
	virtual ~cSceneEditMode();

	// 차후 Setup의 인자로 해당 씬에 관련된 파일 주소를 받아와 해당씬의 모든 리소스 파일들을 
	// 로딩하는 데에 이용한다.
	// 혹은 로딩화면을 위해 쓰인다.
	virtual void Setup(std::string sFilePath);

	// Start에서 리소스를 로딩한다.
	virtual void Start();
	virtual void Update(float delta);
	virtual void Render();

	virtual void AddMapPath(std::string sRawPath, std::string sTexturePath);

	// 모든 리소스를 해제한다.
	virtual void Exit();
};

