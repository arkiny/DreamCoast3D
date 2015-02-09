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

	// �� ���� ���� ������ ���Ͽ��� �ҷ��´�.
	// TODO : ������ ���� ���� ���� ����
	void Setup(std::string sFilePath);
	void Update(float delta);
	void Render();

	void Destroy();

	// delegate override
	void SceneFinished(cScene* pSender) override;
};

