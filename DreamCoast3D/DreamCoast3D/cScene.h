#pragma once

class cCamera;
class cLightSource;
class cGameObject;
class cGameObjManager;

// �������� ��� ���� cScene�� ��ӹ޾Ƽ� ���������.
class cScene : public cObject
{
protected:
	SYNTHESIZE(iSceneDelegate*, m_pDelegate, Delegate);
	SYNTHESIZE(cScene*, m_pNextScene, NextScene);

	// ���� ������Ʈ���� ������ ó�����ִ� �Ŵ���
	cGameObjManager*			m_pGameObjManager;

	// �Ѱ��� ���� �������� ����(�ִ� 8��)�� ������ �ִ�.
	std::vector<cLightSource*>	m_vecLightSources;

	// �� ���� �� ���� �´� �ٸ� ī�޶� ������ �ִ�.
	// �������� ī�޶� ������ ���� ���� ����ؼ� �߰�
	cCamera*					m_pCamera;
	

public:
	cScene();
	virtual ~cScene();

	// ���� Setup�� ���ڷ� �ش� ���� ���õ� ���� �ּҸ� �޾ƿ� �ش���� ��� ���ҽ� ���ϵ��� 
	// �ε��ϴ� ���� �̿��Ѵ�.
	// Ȥ�� �ε�ȭ���� ���� ���δ�.
	virtual void Setup(std::string sFilePath);
	virtual void Update(float delta);
	virtual void Render();

	virtual void AddGameObj(cGameObject* pGameObj);
	virtual void Destroy();
};

