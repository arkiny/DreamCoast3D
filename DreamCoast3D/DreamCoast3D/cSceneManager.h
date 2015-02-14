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

	// �� ���� ���� ������ ���Ͽ��� �ҷ��´�.
	// TODO : ������ ���� ���� ���� ����
	void Setup(std::string sFolder, std::string sFile);
	void Start();

	void AddScene(cScene* pScene);
	void Update(float delta);
	void Render();
	void Destroy();

	// delegate override
	void SceneFinished(cScene* pSender) override;
};

