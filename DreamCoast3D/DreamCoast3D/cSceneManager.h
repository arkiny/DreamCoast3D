#pragma once

class cScene;

class cSceneManager : public cObject, public iSceneDelegate
{
private:
	//std::vector<ST_SCENE_INFO>	m_vecSceneInfo;
	std::vector<std::string>	m_vecSceneInfoFilePath;

	cScene*						m_pCurrentScene;
	bool						m_bIsLoading = false;

	int							m_nNextSceneIndex = -1;
	bool						m_bIsChangeSceneCall = false;

public:
	cSceneManager();
	virtual ~cSceneManager();

	// 각 씬에 대한 정보는 파일에서 불러온다.
	// TODO : 씬정보 저장 파일 포멧 논의
	void Setup(std::string sFolder, std::string sFile);
	void Start();

	//void AddScene(cScene* pScene);
	void Update(float delta);
	void Render();
	void Destroy();

	void AddSceneFilePath(std::string sFilePath);

	// delegate override
	void SceneFinished(cScene* pSender) override;
	void ChangeScene(int nNextSceneIndex) override;
	void ChangeSceneFromLoader(cScene* pNextScene) override;
};

