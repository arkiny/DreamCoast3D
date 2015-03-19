#pragma once

class cCamera;
class cLightSource;
class cGameObjManager;
class cUIObjManager;
class cGameMapObject;
class cEffectManager;

// MS
class cGamePlayableObject;
class cGameEventManager;

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
	//SYNTHESIZE(iSceneDelegate*, m_pDelegate, Delegate);


	SYNTHESIZE(cScene*, m_pNextScene, NextScene);

	SYNTHESIZE(std::string, m_sGameMapPath, GameMapPath);
	SYNTHESIZE(std::string, m_sGameObjPath, GameObjPath);
	SYNTHESIZE(std::string, m_sGameUIPath, GameUIPath);

	iSceneDelegate* m_pDelegate;

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

	cEffectManager*						m_pEffectManager;

	// MS �̺�Ʈ �۾���

	cGameEventManager*					m_pGameEventManager;
	
	/////////////////// Shader //////////////////////////
	//// ���̴�
	//LPD3DXEFFECT			gpApplyShadowShader = NULL;
	//LPD3DXEFFECT			gpCreateShadowShader = NULL;

	//// ���� ��ġ
	//D3DXVECTOR4				gWorldLightPosition = { 500.0f, 500.0f, -500.0f, 1.0f };

	//// ī�޶� ��ġ
	//D3DXVECTOR4				gWorldCameraPosition = { 0.0f, 0.0f, -500.0f, 1.0f };
	//
	//// ��ü�� ����, ���� �ؽ��� ��������...
	//D3DXVECTOR4				gTorusColor = { 1, 1, 0, 1 };
	//D3DXVECTOR4				gDiscColor = { 0, 1, 1, 1 };

	//// �׸��ڸ� ����Ÿ��
	//LPDIRECT3DTEXTURE9		gpShadowRenderTarget = NULL;
	//LPDIRECT3DSURFACE9		gpShadowDepthStencil = NULL;


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


	virtual void SetDelegate(iSceneDelegate* pSceneDeligate);
	virtual iSceneDelegate* GetDelegate(){ return m_pDelegate; }

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

	virtual cGameEventManager* GetGameEvtMng()
	{
		_ASSERT(m_pGameEventManager);
		return m_pGameEventManager;
	}

	bool m_isPauseUpdate;
	bool m_isPlayerPause;
	virtual void PauseObjectUpdate(bool isPause);
	virtual void PausePlayerUpdate(bool isPause);
};

