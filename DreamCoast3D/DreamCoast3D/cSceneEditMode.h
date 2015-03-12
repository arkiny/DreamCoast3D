#pragma once
#include "cScene.h"

// 아 망함
// 대단히 헤비한 클래스가 될듯합니다. ㅡ_ㅡ
// 퍼포먼스가 중요한 플레잉 단계가 아니므로 프러스텀 컬링 적용안함

class cMousePicking;
class cGamePlayableObject;
class cGameObjectEraser;

class cSceneEditMode : public cScene
{
protected:
	// KeyControl
	bool									m_bIsClickDown	= false;
	bool									m_bIsClickUP	= false;
	bool									m_bKeyDown		= false;
	bool									m_bKeyUPTrigger = false;

	float									m_fKeyDelayCurrent	= 2.0f;
	const float								m_fKeyDelay			= 0.5f;

	//
	cGameObjectEraser*						m_pObjectEraser;

	// Map Preset
	std::map<std::string, cGameMapObject*>	m_mapLoadedMap;
	std::vector<std::string>				m_vecMapRawPath;
	std::vector<std::string>				m_vecMapTexturePath;
	int m_nCurrentMapIndex = 0;
	// m_pCurrentMap이 바인딩 되어있는 맵

	// 마우스에 묶여있을 오브젝트
	cGameObject*							m_pCurrentBindingObject = NULL;

	// Static Object Preset
	int m_nCurrentBindingStaticIndex = 0;
	std::vector<cGameObject*>				m_vecStaticGameObjectPreset;
	std::set<cGameObject*>					m_setStaticGameObjectAdded;

	// Action Object Preset
	// 처음 로딩시 올릴 오브젝트 리스트들
	// 지금은 키를 통해서 이동할 거지만
	// 차후에는 클릭해서 선택할 수 있도록
	int m_nCurrentBindingActionIndex = 0;
	std::vector<cGameObject*>				m_vecActionGameObjectPreset;

	// AddedObjectInformation will be saved as a file
	// 업데이트 하지 않고 렌더만 하게 처리
	// 어떻게 클론시켜서 저장할건데? -> 각 게임 오브젝트에 클론오버라이딩
	std::set<cGameObject*>					m_setActionGameObjectAdded;
	
	// 가장 마지막 단계 플레이어 시작 위치
	// 무조건 하나만으로 조정해야 한다.
	int m_nCurrentBindingPlayerIndex = 0;
	std::vector<cGameObject*>				m_vecGamePlayableObjectPreset;
	cGameObject*							m_pPlayableObjectSave = NULL;


	// 차후 멀티쓰레딩용
	volatile bool m_bMapLoaded = true;
	volatile bool m_bObjectLoaded = true;

	RECT m_rectFontArea;

	cMousePicking* m_pMousPicking;

protected:
	static void LoadNextMap(LPVOID pParam);
	static void LoadNextObj(LPVOID pParam);

	virtual void BindingNextMap();
	virtual void BindingPrevMap();

	virtual void BindingNextActionObject();
	virtual void BindingPrevActionObject();

	virtual void BindingNextPlayableObject();
	virtual void BindingPrevPlayableObject();

	virtual void BindingNextStaticObject();
	virtual void BindingPrevStaticObject();

	virtual void AddCurrentObjectToSaveStack(cGameObject* pToBeAdded);
	
	//
	virtual void SaveToFile();


	// MainData
	virtual void SaveMainDataToFile();
	virtual std::string GetCameraAsString();
	virtual std::string GetLightAsString();
	virtual std::string GetUIListAsString();
	virtual std::string GetGameMapListAsString();
	virtual std::string GetGameObjectListAsString();

	// MapData;
	virtual void SaveMapDataToFile();
	virtual std::string GetMapInfoAsString();
	virtual std::string GetMapObjectMaterialList();
	virtual std::string GetStaticGameObjectAddedAsString();

	// GameObjectData
	virtual void SaveGameObjectDataToFile();
	virtual std::string GetMeshListAsString();
	virtual std::string GetActionGameObjectAddedAsString();
	virtual std::string GetPlayableGameObjectAsString();

public:
	cSceneEditMode();
	virtual ~cSceneEditMode();

	// 차후 Setup의 인자로 해당 씬에 관련된 파일 주소를 받아와 해당씬의 모든 리소스 파일들을 
	// 로딩하는 데에 이용한다.
	// 혹은 로딩화면을 위해 쓰인다.
	virtual void Setup(std::string sFilePath);

	// Start에서 리소스를 로딩한다.
	virtual void Start();
	virtual void Update(float delta);
	virtual void Render();

	virtual void AddMapPath(std::string sRawPath, std::string sTexturePath);
	
	virtual void AddGameAIObjectToPreset(cGameObject* pGameObject);
	virtual void AddGamePlayableObjectToPreset(cGamePlayableObject* pGameObject);
	virtual void AddStaticGameObjectToPreset(cGameObject* pGameObject);

	// 모든 리소스를 해제한다.
	virtual void Exit();


private:
	
	void HelperRender();
};

