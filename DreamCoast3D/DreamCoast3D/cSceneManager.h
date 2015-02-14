#pragma once

class cScene;

class cSceneManager : public cObject, public iSceneDelegate
{
private:
	std::set<cScene*>		m_setScenes;
	cScene*					m_pCurrentScene;

public:
	cSceneManager();
	~cSceneManager();

	// 각 씬에 대한 정보는 파일에서 불러온다.
	// TODO : 씬정보 저장 파일 포멧 논의
	void Setup(std::string sFolder, std::string sFile);
	void Start();

	void AddScene(cScene* pScene);
	void Update(float delta);
	void Render();
	void Destroy();

	// delegate override
	void SceneFinished(cScene* pSender) override;
};

