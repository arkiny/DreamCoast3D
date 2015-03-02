#pragma once

class cScene;

class cSceneManager : public cObject, public iSceneDelegate
{
private:
	//std::vector<ST_SCENE_INFO>	m_vecSceneInfo;
	std::vector<std::string>	m_vecSceneInfoFilePath;

	cScene*						m_pCurrentScene;

public:
	cSceneManager();
	virtual ~cSceneManager();

	// �� ���� ���� ������ ���Ͽ��� �ҷ��´�.
	// TODO : ������ ���� ���� ���� ����
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
};

