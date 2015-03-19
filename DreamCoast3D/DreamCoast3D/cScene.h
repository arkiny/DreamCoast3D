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

// 앞으로의 모든 씬은 cScene을 상속받아서 만들어진다.
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

	// 게임 오브젝트들을 가지고 처리해주는 매니저
	// 각 씬마다 변경시 갱신해줘야 한다.
	cGameObjManager*					m_pGameObjManager;

	
	// 각씬은 여러개의 UI를 가질수 있다.
	cUIObjManager*						m_pUIObjManager;	

	// 맵은 Seamless(타일형) 일수도 있으므로 각 씬당 여러개를 가질수도 있다.
	std::vector<cGameMapObject*>		m_vecGameMaps;
	cGameMapObject*						m_pCurrentMap;


	// 한개의 씬은 여러개의 광원(최대 8개)을 가질수 있다.
	std::vector<cLightSource*>			m_vecLightSources;

	// 각 씬은 각 씬에 맞는 다른 카메라를 가지고 있다.
	// 여러개의 카메라를 가지는 것은 차후 고민해서 추가
	cCamera*							m_pCamera;

	cEffectManager*						m_pEffectManager;

	// MS 이벤트 작업중

	cGameEventManager*					m_pGameEventManager;
	
	/////////////////// Shader //////////////////////////
	//// 쉐이더
	//LPD3DXEFFECT			gpApplyShadowShader = NULL;
	//LPD3DXEFFECT			gpCreateShadowShader = NULL;

	//// 빛의 위치
	//D3DXVECTOR4				gWorldLightPosition = { 500.0f, 500.0f, -500.0f, 1.0f };

	//// 카메라 위치
	//D3DXVECTOR4				gWorldCameraPosition = { 0.0f, 0.0f, -500.0f, 1.0f };
	//
	//// 물체의 색상, 차후 텍스쳐 적용으로...
	//D3DXVECTOR4				gTorusColor = { 1, 1, 0, 1 };
	//D3DXVECTOR4				gDiscColor = { 0, 1, 1, 1 };

	//// 그림자맵 렌더타깃
	//LPDIRECT3DTEXTURE9		gpShadowRenderTarget = NULL;
	//LPDIRECT3DSURFACE9		gpShadowDepthStencil = NULL;


public:
	cScene();
	virtual ~cScene();

	// 차후 Setup의 인자로 해당 씬에 관련된 파일 주소를 받아와 해당씬의 모든 리소스 파일들을 
	// 로딩하는 데에 이용한다.
	// 혹은 로딩화면을 위해 쓰인다.
	virtual void Setup(std::string sFilePath);

	// Start에서 리소스를 로딩한다.
	virtual void Start();
	virtual void Update(float delta);
	virtual void Render();


	// 모든 리소스를 해제한다.
	virtual void Exit();


	virtual void SetDelegate(iSceneDelegate* pSceneDeligate);
	virtual iSceneDelegate* GetDelegate(){ return m_pDelegate; }

	virtual void AddGameObj(cGameObject* pGameObj);
	virtual void AddUIObj(cUIObject* pUIObj);

	virtual void AddStaticGameObj(cGameObject* pGameObj);

	// 맵은 한번 애드되면 고정되있기로 한다.
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

