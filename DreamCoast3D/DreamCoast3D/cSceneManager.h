#pragma once

class cScene;

class cSceneManager : public cObject, public iSceneDelegate
{
private:
	std::vector<cScene*>	m_vecScenes;
	cScene*					m_pCurrentScene;

public:
	cSceneManager();
	~cSceneManager();

	// 각 씬에 대한 정보는 파일에서 불러온다.
	// TODO : 씬정보 저장 파일 포멧 논의
	void Setup(std::string sFilePath);
	void Update(float delta);
	void Render();

	void Destroy();

	// delegate override
	void SceneFinished(cScene* pSender) override;
};

