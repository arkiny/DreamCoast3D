#pragma once
#include "cScene.h"

class cMousePicking;

class cSceneEditMode : public cScene
{
protected:
	std::map<std::string, cGameMapObject*> m_mapLoadedMap;
	std::vector<std::string> m_vecMapRawPath;
	std::vector<std::string> m_vecMapTexturePath;

	// ���� ��Ƽ��������
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

	// ���� Setup�� ���ڷ� �ش� ���� ���õ� ���� �ּҸ� �޾ƿ� �ش���� ��� ���ҽ� ���ϵ��� 
	// �ε��ϴ� ���� �̿��Ѵ�.
	// Ȥ�� �ε�ȭ���� ���� ���δ�.
	virtual void Setup(std::string sFilePath);

	// Start���� ���ҽ��� �ε��Ѵ�.
	virtual void Start();
	virtual void Update(float delta);
	virtual void Render();

	virtual void AddMapPath(std::string sRawPath, std::string sTexturePath);

	// ��� ���ҽ��� �����Ѵ�.
	virtual void Exit();
};

