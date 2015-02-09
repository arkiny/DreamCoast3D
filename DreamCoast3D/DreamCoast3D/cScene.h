#pragma once

class cGameObject;

class cScene : public cObject
{
private:
	SYNTHESIZE(iSceneDelegate*, m_pDelegate, Delegate);
	SYNTHESIZE(cScene*, m_pNextScene, NextScene);

	// �ش� ������ ����/������Ʈ �� ������Ʈ��
	std::vector<cGameObject*> m_vecGameObjects;

public:
	cScene();
	~cScene();

	// ���� Setup�� ���ڷ� �ش� ���� ���õ� ���� �ּҸ� �޾ƿ� �ش���� ��� ���ҽ� ���ϵ��� 
	// �ε��ϴ� ���� �̿��Ѵ�.
	// Ȥ�� �ε�ȭ���� ���� ���δ�.
	void Setup(std::string sFilePath);
	void Update(float delta);
	void Render();
};

