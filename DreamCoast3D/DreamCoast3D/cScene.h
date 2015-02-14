#pragma once

class cCamera;
class cLightSource;
class cGameObjManager;
class cUIObjManager;
class cGameMapObject;

// �������� ��� ���� cScene�� ��ӹ޾Ƽ� ���������.
class cScene : public cObject
{
protected:
	SYNTHESIZE(iSceneDelegate*, m_pDelegate, Delegate);
	SYNTHESIZE(cScene*, m_pNextScene, NextScene);

	// ���� ������Ʈ���� ������ ó�����ִ� �Ŵ���
	// �� ������ ����� ��������� �Ѵ�.
	cGameObjManager*					m_pGameObjManager;
	
	// ������ �������� UI�� ������ �ִ�.
	cUIObjManager*						m_pUIObjManager;	

	// ���� Seamless(Ÿ����) �ϼ��� �����Ƿ� �� ���� �������� �������� �ִ�.
	std::vector<cGameMapObject*>		m_vecGameMaps;
	cGameMapObject*						m_pCurrentMap;


	// �Ѱ��� ���� �������� ����(�ִ� 8��)�� ������ �ִ�.
	std::vector<cLightSource*>			m_vecLightSources;

	// �� ���� �� ���� �´� �ٸ� ī�޶� ������ �ִ�.
	// �������� ī�޶� ������ ���� ���� ����ؼ� �߰�
	cCamera*							m_pCamera;
	

public:
	cScene();
	virtual ~cScene();

	// ���� Setup�� ���ڷ� �ش� ���� ���õ� ���� �ּҸ� �޾ƿ� �ش���� ��� ���ҽ� ���ϵ��� 
	// �ε��ϴ� ���� �̿��Ѵ�.
	// Ȥ�� �ε�ȭ���� ���� ���δ�.
	virtual void Setup(std::string sFilePath);

	virtual void Start();

	virtual void Update(float delta);
	virtual void Render();

	virtual void AddGameObj(cGameObject* pGameObj);
	virtual void AddUIObj(cUIObject* pUIObj);

	// ���� �ѹ� �ֵ�Ǹ� �������ֱ�� �Ѵ�.
	virtual void AddMap(cGameMapObject* pGameMap);
	virtual void SetCurrentMap(int nIndex);

	//
	virtual cGameObjManager* GetGameObjMng(){
		_ASSERT(m_pGameObjManager);
		return m_pGameObjManager;
	}

	virtual void SetCamera(cCamera* pCamera);
	virtual void AddLightSrc(cLightSource* pLightSource);

	virtual size_t GetLightSrcSize(){ return m_vecLightSources.size(); }
	virtual size_t GetMapSize(){ return m_vecGameMaps.size(); }
	

	virtual void Destroy();
};

