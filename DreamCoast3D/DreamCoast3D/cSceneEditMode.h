#pragma once
#include "cScene.h"

// �� ����
// ����� ����� Ŭ������ �ɵ��մϴ�. ��_��
// �����ս��� �߿��� �÷��� �ܰ谡 �ƴϹǷ� �������� �ø� �������

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
	// m_pCurrentMap�� ���ε� �Ǿ��ִ� ��

	// ���콺�� �������� ������Ʈ
	cGameObject*							m_pCurrentBindingObject = NULL;

	// Static Object Preset
	int m_nCurrentBindingStaticIndex = 0;
	std::vector<cGameObject*>				m_vecStaticGameObjectPreset;
	std::set<cGameObject*>					m_setStaticGameObjectAdded;

	// Action Object Preset
	// ó�� �ε��� �ø� ������Ʈ ����Ʈ��
	// ������ Ű�� ���ؼ� �̵��� ������
	// ���Ŀ��� Ŭ���ؼ� ������ �� �ֵ���
	int m_nCurrentBindingActionIndex = 0;
	std::vector<cGameObject*>				m_vecActionGameObjectPreset;

	// AddedObjectInformation will be saved as a file
	// ������Ʈ ���� �ʰ� ������ �ϰ� ó��
	// ��� Ŭ�н��Ѽ� �����Ұǵ�? -> �� ���� ������Ʈ�� Ŭ�п������̵�
	std::set<cGameObject*>					m_setActionGameObjectAdded;
	
	// ���� ������ �ܰ� �÷��̾� ���� ��ġ
	// ������ �ϳ������� �����ؾ� �Ѵ�.
	int m_nCurrentBindingPlayerIndex = 0;
	std::vector<cGameObject*>				m_vecGamePlayableObjectPreset;
	cGameObject*							m_pPlayableObjectSave = NULL;


	// ���� ��Ƽ��������
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

	// ���� Setup�� ���ڷ� �ش� ���� ���õ� ���� �ּҸ� �޾ƿ� �ش���� ��� ���ҽ� ���ϵ��� 
	// �ε��ϴ� ���� �̿��Ѵ�.
	// Ȥ�� �ε�ȭ���� ���� ���δ�.
	virtual void Setup(std::string sFilePath);

	// Start���� ���ҽ��� �ε��Ѵ�.
	virtual void Start();
	virtual void Update(float delta);
	virtual void Render();

	virtual void AddMapPath(std::string sRawPath, std::string sTexturePath);
	
	virtual void AddGameAIObjectToPreset(cGameObject* pGameObject);
	virtual void AddGamePlayableObjectToPreset(cGamePlayableObject* pGameObject);
	virtual void AddStaticGameObjectToPreset(cGameObject* pGameObject);

	// ��� ���ҽ��� �����Ѵ�.
	virtual void Exit();


private:
	
	void HelperRender();
};

