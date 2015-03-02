#pragma once

class cCamera;
class cLightSource;
class cGameObjManager;
class cUIObjManager;
class cGameMapObject;

// MS
class cGamePlayableObject;

// �������� ��� ���� cScene�� ��ӹ޾Ƽ� ���������.
class cScene : public cObject
{
//public:
//	enum E_SCENE_TYPE{
//		E_SCENE_MAIN,
//		E_SCENE_INGAME,
//		E_SCENE_LOADING,
//		E_SCENE_TYPE_MAX
//	};

protected:
	SYNTHESIZE(iSceneDelegate*, m_pDelegate, Delegate);
	SYNTHESIZE(cScene*, m_pNextScene, NextScene);

	SYNTHESIZE(std::string, m_sGameMapPath, GameMapPath);
	SYNTHESIZE(std::string, m_sGameObjPath, GameObjPath);
	SYNTHESIZE(std::string, m_sGameUIPath, GameUIPath);

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

	// Start���� ���ҽ��� �ε��Ѵ�.
	virtual void Start();
	virtual void Update(float delta);
	virtual void Render();

	// ��� ���ҽ��� �����Ѵ�.
	virtual void Exit();


	virtual void AddGameObj(cGameObject* pGameObj);
	virtual void AddUIObj(cUIObject* pUIObj);

	virtual void AddStaticGameObj(cGameObject* pGameObj);

	// ���� �ѹ� �ֵ�Ǹ� �������ֱ�� �Ѵ�.
	virtual void AddMap(cGameMapObject* pGameMap);
	virtual void SetCurrentMap(int nIndex);

	//
	virtual cGameObjManager* GetGameObjMng(){
		_ASSERT(m_pGameObjManager);
		return m_pGameObjManager;
	}

	//
	virtual cUIObjManager* GetUIObjMng(){
		_ASSERT(m_pUIObjManager);
		return m_pUIObjManager;
	}

	virtual void SetCamera(cCamera* pCamera);
	virtual void AddLightSrc(cLightSource* pLightSource);

	virtual size_t GetLightSrcSize(){ return m_vecLightSources.size(); }
	virtual size_t GetMapSize(){ return m_vecGameMaps.size(); }
	

	virtual void Destroy();


	// MS
	cGamePlayableObject* m_pPlayableObject;


};

