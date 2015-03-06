#include "stdafx.h"
#include "cSceneEditMode.h"
#include "cLightSource.h"
#include "cGameObjManager.h"
#include "cUIObjManager.h"
#include "cCameraEditing.h"
#include "cTransform.h"
#include "cMapLoader.h"
#include "cUILoader.h"
#include "cEffectManager.h"
#include "cMapLoader4Edit.h"
#include "cGameMapHeightGridSystem.h"
#include "cMousePicking.h"
#include "cGameObjectLoader4Edit.h"
#include "cGamePlayableObject.h"
#include "cGameObjectEraser.h"
#include <sstream>

static CRITICAL_SECTION				gCriticalSectionEditMode;

cSceneEditMode::cSceneEditMode()
	:m_pMousPicking(NULL),
	m_pObjectEraser(NULL)
{
	InitializeCriticalSection(&gCriticalSectionEditMode);
	m_rectFontArea = { 0, 0, 200, 200 };


}


cSceneEditMode::~cSceneEditMode()
{
	WaitForSingleObject(LoadNextMap, INFINITE);
	WaitForSingleObject(LoadNextObj, INFINITE);
	LeaveCriticalSection(&gCriticalSectionEditMode);
	DeleteCriticalSection(&gCriticalSectionEditMode);

	SAFE_RELEASE(m_pObjectEraser);

	for (auto p : m_mapLoadedMap){
		SAFE_RELEASE(p.second);
	}
	for (auto p : m_vecStaticGameObjectPreset){
		SAFE_RELEASE(p);
	}
	for (auto p : m_setStaticGameObjectAdded){
		SAFE_RELEASE(p);
	}
	for (auto p : m_vecActionGameObjectPreset){
		SAFE_RELEASE(p);
	}
	for (auto p : m_setActionGameObjectAdded){
		SAFE_RELEASE(p);
	}
	for (auto p : m_vecGamePlayableObjectPreset){
		SAFE_RELEASE(p);
	}
	SAFE_RELEASE(m_pPlayableObjectSave);
	SAFE_DELETE(m_pMousPicking);
}

void cSceneEditMode::Setup(std::string sFilePath){
	cScene::Setup(sFilePath);
	m_pMousPicking = new cMousePicking;
	m_pMousPicking->SetEffectDelegate(m_pEffectManager);

	m_pObjectEraser = new cGameObjectEraser;
	m_pObjectEraser->Setup();
}

// Start에서 리소스를 로딩한다.
void cSceneEditMode::Start(){
	// 일단 UI는 기존 세팅이 되있는 UI를 이용한다.
	cUILoader cUL;
	cUL.LoadGameUIFromFile(this->GetUIObjMng(), m_sGameUIPath);

	cMapLoader4Edit cML;
	cML.LoadGameMapFromFile(this, m_sGameMapPath);

	cGameObjectLoader4Edit cGL;
	cGL.LoadGameObjectsFromFile(this, m_sGameObjPath);

	g_pD3DDevice->SetRenderState(D3DRS_NORMALIZENORMALS, true);
	g_pD3DDevice->SetRenderState(D3DRS_LIGHTING, true);

	// 역시 기존 씬의 광원을 이용한다.
	for (auto p : m_vecLightSources){
		p->Start();
	}

	if (m_pEffectManager){
		if (m_pGameObjManager){
			m_pGameObjManager->SetEffectDeligate(m_pEffectManager);
		}
		m_pEffectManager->Start();
	}

	assert(m_pCurrentMap == NULL && "m_pCurrentMap Should not has pointer in EditMode");
	assert(m_pEffectManager && m_pGameObjManager && m_pUIObjManager && m_pCamera && "All Manager Should have begun");
}

void cSceneEditMode::Update(float delta){
	if (m_pCamera){
		m_pCamera->Update(delta);
	}
	
	if (m_pCurrentMap){
		if (m_pCurrentMap){
			m_pCurrentMap->Update(delta);
		}

		if (m_pGameObjManager){
			m_pGameObjManager->Update(delta);
		}

		// 매 프레임 게임 오브젝트의 Y축을 조정해준다.
		if (m_pCurrentMap && m_pGameObjManager && m_pCurrentMap){
			m_pGameObjManager->AdjustYPositionByHeightMap(m_pCurrentMap);
		}
	}

	if (m_pEffectManager){
		m_pEffectManager->Update(delta);
	}

	if (m_pUIObjManager){
		m_pUIObjManager->Update(delta);
	}

	//
	if (m_pMousPicking && m_pCurrentMap){
		m_pMousPicking->Update();
		if (m_pCurrentBindingObject){
			m_pCurrentBindingObject->SetPosition(m_pMousPicking->GetPickingPoint());
		}
	}

	if (GetAsyncKeyState(VK_LBUTTON)){
		m_bIsClickDown = true;
	}

	///
	if (m_fKeyDelayCurrent > m_fKeyDelay){
		m_fKeyDelayCurrent = m_fKeyDelay + 1.0f;
	}
	else {
		m_fKeyDelayCurrent += delta;
	}
	///
	if (m_fKeyDelayCurrent > m_fKeyDelay){
		if (GetAsyncKeyState(VK_NEXT)){
			BindingNextMap();
			m_fKeyDelayCurrent = 0;
		}
		if (GetAsyncKeyState(VK_PRIOR)){
			BindingPrevMap();
			m_fKeyDelayCurrent = 0;
		}
		if (m_pCurrentMap){
			if (GetAsyncKeyState(VK_RIGHT)){
				BindingNextActionObject();
				m_fKeyDelayCurrent = 0;
			}
			if (GetAsyncKeyState(VK_LEFT)){
				BindingPrevActionObject();
				m_fKeyDelayCurrent = 0;
			}
			if (GetAsyncKeyState(VK_UP)){
				BindingNextPlayableObject();
				m_fKeyDelayCurrent = 0;
			}
			if (GetAsyncKeyState(VK_DOWN)){
				BindingPrevPlayableObject();
				m_fKeyDelayCurrent = 0;
			}
			if (GetAsyncKeyState(VK_END)){
				BindingNextStaticObject();
				m_fKeyDelayCurrent = 0;
			}
			if (GetAsyncKeyState(VK_HOME)){
				BindingPrevStaticObject();
				m_fKeyDelayCurrent = 0;
			}
			if (GetAsyncKeyState(VK_F5)){
				SaveToFile();
				m_fKeyDelayCurrent = 0;
			}
			if (GetAsyncKeyState(VK_BACK)){
				m_pCurrentBindingObject = m_pObjectEraser;
				m_fKeyDelayCurrent = 0;
			}
			if (GetAsyncKeyState(VK_LBUTTON) == false && m_bIsClickDown == true && m_pCurrentBindingObject){
				AddCurrentObjectToSaveStack(m_pCurrentBindingObject);
				m_bIsClickDown = false;
			}
		}
	}
}

void cSceneEditMode::Render(){
	cScene::Render();

	if (m_pCurrentBindingObject){
		m_pCurrentBindingObject->Render();
	}

	std::stringstream s;
	s.precision(2);
	if (m_pCurrentMap){
		s << "Map Preset Binded: " << m_vecMapRawPath[m_nCurrentMapIndex] << std::endl;
		
		for (auto p : m_setActionGameObjectAdded){
			p->Render();
		}

		for (auto p : m_setStaticGameObjectAdded){
			p->Render();
		}

		if (m_pPlayableObjectSave){
			m_pPlayableObjectSave->Render();
		}
	}

	if (m_pCurrentBindingObject){
		s << "\t GameObject Binded" << std::endl;

		switch (m_pCurrentBindingObject->GetGameObjectType())
		{
		case cGameObject::eGameObjectType::E_MOP :
			s << "\t\t Mob Preset:" << m_nCurrentBindingActionIndex << std::endl;
			break;
		case cGameObject::eGameObjectType::E_PLAYABLE :
			s << "\t\t Playable Preset:" << m_nCurrentBindingPlayerIndex << std::endl;
			break;
		case cGameObject::eGameObjectType::E_STATIC :
			s << "\t\t Static Preset:" << m_nCurrentBindingStaticIndex << std::endl;
			break;
		default:
			break;
		}

	}

	g_pFontManager->GetFont(g_pFontManager->FONT_DEFAULT)->DrawText(NULL,				 //pSprite
					s.str().c_str(),	 //pString
					-1,					//Count
					&m_rectFontArea,	//pRect
					DT_LEFT | DT_NOCLIP,//Format,
					0xFFFFFFFF);		//Color
}

// 모든 리소스를 해제한다.
void cSceneEditMode::Exit(){
	cScene::Exit();	
}

void cSceneEditMode::AddMapPath(std::string sRawPath, std::string sTexturePath){
	m_vecMapRawPath.push_back(sRawPath);
	m_vecMapTexturePath.push_back(sTexturePath);
}

void cSceneEditMode::LoadNextMap(LPVOID pParam){
	// todo
}
void cSceneEditMode::LoadNextObj(LPVOID pParam){
	// todo
}

void cSceneEditMode::BindingNextMap(){
	if (m_pCurrentMap == NULL){
		cGameMapHeightGridSystem* ret = new cGameMapHeightGridSystem;
		ret->LoadFromFiles(m_vecMapRawPath[0], m_vecMapTexturePath[0]);
		m_mapLoadedMap[m_vecMapRawPath[0]] = ret;
		m_pCurrentMap = ret;
		m_pMousPicking->SetVertex(ret->GetVertexVectorByRef());
		m_pCurrentMap->AddRef();
	}
	else {
		if (m_nCurrentMapIndex == m_vecMapRawPath.size() - 1){
			m_nCurrentMapIndex = 0;
		}
		else {
			m_nCurrentMapIndex++;
		}
		if (m_mapLoadedMap.find(m_vecMapRawPath[m_nCurrentMapIndex]) == m_mapLoadedMap.end()){
			cGameMapHeightGridSystem* ret = new cGameMapHeightGridSystem;
			ret->LoadFromFiles(m_vecMapRawPath[m_nCurrentMapIndex], m_vecMapTexturePath[m_nCurrentMapIndex]);
			m_mapLoadedMap[m_vecMapRawPath[m_nCurrentMapIndex]] = ret;
			m_pMousPicking->SetVertex(ret->GetVertexVectorByRef());
			m_pCurrentMap->Release();
			m_pCurrentMap = ret;
			m_pCurrentMap->AddRef();
		}
		else{
			m_pCurrentMap->Release();
			m_pCurrentMap = m_mapLoadedMap[m_vecMapRawPath[m_nCurrentMapIndex]];
			cGameMapHeightGridSystem* ret = (cGameMapHeightGridSystem*)m_pCurrentMap;
			m_pMousPicking->SetVertex(ret->GetVertexVectorByRef());
			m_pCurrentMap->AddRef();
		}
	}
	
}

void cSceneEditMode::BindingPrevMap(){
	if (m_pCurrentMap == NULL){
		cGameMapHeightGridSystem* ret = new cGameMapHeightGridSystem;
		ret->LoadFromFiles(m_vecMapRawPath[0], m_vecMapTexturePath[0]);
		m_mapLoadedMap[m_vecMapRawPath[0]] = ret;
		m_pCurrentMap = ret;
		m_pMousPicking->SetVertex(ret->GetVertexVectorByRef());
		m_pCurrentMap->AddRef();
	}
	else {
		if (m_nCurrentMapIndex == 0){
			m_nCurrentMapIndex = m_vecMapRawPath.size() - 1;
		}
		else {
			m_nCurrentMapIndex--;
		}
		if (m_mapLoadedMap.find(m_vecMapRawPath[m_nCurrentMapIndex]) == m_mapLoadedMap.end()){
			cGameMapHeightGridSystem* ret = new cGameMapHeightGridSystem;
			ret->LoadFromFiles(m_vecMapRawPath[m_nCurrentMapIndex], m_vecMapTexturePath[m_nCurrentMapIndex]);
			m_mapLoadedMap[m_vecMapRawPath[m_nCurrentMapIndex]] = ret;
			m_pMousPicking->SetVertex(ret->GetVertexVectorByRef());
			m_pCurrentMap->Release();
			m_pCurrentMap = ret;
			m_pCurrentMap->AddRef();
		}
		else{
			m_pCurrentMap->Release();
			m_pCurrentMap = m_mapLoadedMap[m_vecMapRawPath[m_nCurrentMapIndex]];
			cGameMapHeightGridSystem* ret = (cGameMapHeightGridSystem*)m_pCurrentMap;
			m_pMousPicking->SetVertex(ret->GetVertexVectorByRef());
			m_pCurrentMap->AddRef();
		}
	}
}

void cSceneEditMode::AddGameAIObjectToPreset(cGameObject* pGameObject){
	SAFE_ADD_REF(pGameObject);
	m_vecActionGameObjectPreset.push_back(pGameObject);
}

void cSceneEditMode::AddGamePlayableObjectToPreset(cGamePlayableObject* pGameObject){
	SAFE_ADD_REF(pGameObject);
	m_vecGamePlayableObjectPreset.push_back(pGameObject);
}

void cSceneEditMode::AddStaticGameObjectToPreset(cGameObject* pGameObject){
	SAFE_ADD_REF(pGameObject);
	m_vecStaticGameObjectPreset.push_back(pGameObject);
}

void cSceneEditMode::BindingNextActionObject(){
	if (m_pCurrentBindingObject == NULL 
		|| m_pCurrentBindingObject->GetGameObjectType() != m_pCurrentBindingObject->E_MOP){
		m_pCurrentBindingObject = m_vecActionGameObjectPreset[m_nCurrentBindingActionIndex];
	}
	else{
		if (m_nCurrentBindingActionIndex == m_vecActionGameObjectPreset.size()-1){
			m_nCurrentBindingActionIndex = 0;
		}
		else {
			m_nCurrentBindingActionIndex++;
		}
		m_pCurrentBindingObject = m_vecActionGameObjectPreset[m_nCurrentBindingActionIndex];
	}
}

void cSceneEditMode::BindingPrevActionObject(){
	if (m_pCurrentBindingObject == NULL 
		|| m_pCurrentBindingObject->GetGameObjectType() != m_pCurrentBindingObject->E_MOP){
		m_pCurrentBindingObject = m_vecActionGameObjectPreset[m_nCurrentBindingActionIndex];
	}
	else{
		if (m_nCurrentBindingActionIndex == 0){
			m_nCurrentBindingActionIndex = m_vecActionGameObjectPreset.size() - 1;
		}
		else {
			m_nCurrentBindingActionIndex--;
		}
		m_pCurrentBindingObject = m_vecActionGameObjectPreset[m_nCurrentBindingActionIndex];
	}
}

void cSceneEditMode::AddCurrentObjectToSaveStack(cGameObject* pToBeAdded){
	if (pToBeAdded->GetGameObjectType() == pToBeAdded->E_MOP){
		cGameObject* p = NULL;
		m_pCurrentBindingObject->Clone(&p);
		m_setActionGameObjectAdded.insert(p);
		//p = NULL;
	}
	else if (pToBeAdded->GetGameObjectType() == pToBeAdded->E_PLAYABLE){
		cGameObject* p = NULL;
		m_pCurrentBindingObject->Clone(&p);

		if (m_pPlayableObjectSave){
			m_pPlayableObjectSave->Release();
		}

		m_pPlayableObjectSave = p;
	}
	else if (pToBeAdded->GetGameObjectType() == pToBeAdded->E_STATIC){
		cGameObject* p = NULL;
		m_pCurrentBindingObject->Clone(&p);
		m_setStaticGameObjectAdded.insert(p);
	}
	else if (pToBeAdded->GetGameObjectType() == pToBeAdded->E_ERASER){
		// Object 삭제 작업
		ST_BOUNDING_SPHERE stSphere = *m_pObjectEraser->GetBoundingSphere();
		//
		std::queue<cGameObject*> willbeDelete;
		for (auto p : m_setActionGameObjectAdded){
			D3DXVECTOR3 vObjectPosition = p->GetPosition();
			float fDist = D3DXVec3Length(&(stSphere.m_vCenter - vObjectPosition));
			if (fDist < stSphere.m_fRadius){
				willbeDelete.push(p);
			}
		}
		while (!willbeDelete.empty()){
			cGameObject* erase = willbeDelete.front();
			m_setActionGameObjectAdded.erase(erase);
			SAFE_RELEASE(erase);
			willbeDelete.pop();
		}
		for (auto p : m_setStaticGameObjectAdded){
			D3DXVECTOR3 vObjectPosition = p->GetPosition();
			float fDist = D3DXVec3Length(&(stSphere.m_vCenter - vObjectPosition));
			if (fDist < stSphere.m_fRadius){
				willbeDelete.push(p);
			}
		}
		while (!willbeDelete.empty()){
			cGameObject* erase = willbeDelete.front();
			m_setActionGameObjectAdded.erase(erase);
			SAFE_RELEASE(erase);
			willbeDelete.pop();
		}
		if (m_pPlayableObjectSave){
			D3DXVECTOR3 vObjectPosition = m_pPlayableObjectSave->GetPosition();
			float fDist = D3DXVec3Length(&(stSphere.m_vCenter - vObjectPosition));
			if (fDist < stSphere.m_fRadius){
				SAFE_RELEASE(m_pPlayableObjectSave);
			}
		}
	}
}

void cSceneEditMode::BindingNextPlayableObject(){
	if (m_pCurrentBindingObject == NULL 
		|| m_pCurrentBindingObject->GetGameObjectType() != m_pCurrentBindingObject->E_PLAYABLE){
		m_pCurrentBindingObject = m_vecGamePlayableObjectPreset[m_nCurrentBindingPlayerIndex];
	}
	else {
		if (m_nCurrentBindingPlayerIndex == m_vecGamePlayableObjectPreset.size() - 1){
			m_nCurrentBindingPlayerIndex = 0;
		}
		else {
			m_nCurrentBindingActionIndex++;
		}
		m_pCurrentBindingObject = m_vecActionGameObjectPreset[m_nCurrentBindingActionIndex];
	}
}

void cSceneEditMode::BindingPrevPlayableObject(){
	if (m_pCurrentBindingObject == NULL || m_pCurrentBindingObject->GetGameObjectType() != m_pCurrentBindingObject->E_PLAYABLE){
		m_pCurrentBindingObject = m_vecGamePlayableObjectPreset[m_nCurrentBindingPlayerIndex];
	}
	else {
		if (m_nCurrentBindingPlayerIndex == 0){
			m_nCurrentBindingPlayerIndex = m_vecGamePlayableObjectPreset.size() - 1;
		}
		else {
			m_nCurrentBindingActionIndex--;
		}
		m_pCurrentBindingObject = m_vecActionGameObjectPreset[m_nCurrentBindingActionIndex];
	}
}

void cSceneEditMode::BindingNextStaticObject(){
	if (m_pCurrentBindingObject == NULL 
		|| m_pCurrentBindingObject->GetGameObjectType() != m_pCurrentBindingObject->E_STATIC){
		m_pCurrentBindingObject = m_vecStaticGameObjectPreset[m_nCurrentBindingStaticIndex];
	}
	else {
		if (m_nCurrentBindingStaticIndex == m_vecStaticGameObjectPreset.size() - 1){
			m_nCurrentBindingStaticIndex = 0;
		}
		else {
			m_nCurrentBindingStaticIndex++;
		}
		m_pCurrentBindingObject = m_vecStaticGameObjectPreset[m_nCurrentBindingStaticIndex];
	}
}

void cSceneEditMode::BindingPrevStaticObject(){
	if (m_pCurrentBindingObject == NULL 
		|| m_pCurrentBindingObject->GetGameObjectType() != m_pCurrentBindingObject->E_STATIC){
		m_pCurrentBindingObject = m_vecStaticGameObjectPreset[m_nCurrentBindingStaticIndex];
	}
	else {
		if (m_nCurrentBindingStaticIndex == 0){
			m_nCurrentBindingStaticIndex = m_vecStaticGameObjectPreset.size() - 1;
		}
		else {
			m_nCurrentBindingStaticIndex++;
		}
		m_pCurrentBindingObject = m_vecStaticGameObjectPreset[m_nCurrentBindingStaticIndex];
	}
}

void cSceneEditMode::SaveToFile(){
	SaveMainDataToFile();
	SaveMapDataToFile();
	SaveGameObjectDataToFile(); 
}



//*SCENE 1 {
//}

// MainData
void cSceneEditMode::SaveMainDataToFile(){
	FILE* fp;
	fopen_s(&fp, "../Resources/SCENE5_MAIN.txt", "w+");
	std::stringstream ss;
	ss << "*SCENE 1 {" << std::endl;
	ss << GetCameraAsString() << std::endl;
	ss << GetLightAsString() << std::endl;
	ss << GetUIListAsString() << std::endl;
	ss << GetGameMapListAsString() << std::endl;
	ss << GetGameObjectListAsString() << std::endl;
	ss << "}" << std::endl;
	fprintf(fp, ss.str().c_str());
	fclose(fp);
}

//	*CAMERA_LIST{
//		*CAMERA_COUNT 1
//		*CAMERA 0 {
//			*CAMERA_TYPE		*EDIT
//				*CAMERA_EYE 		0.0 	2.0 - 10.0
//				*CAMERA_LOOKAT		0.0	0.0	2.0
//				*CAMERA_UP		0.0	1.0	0.0
//				*CAMERA_XANGLE		0.0
//				*CAMERA_YANGLE		0.0
//				*CAMERA_DIST		43.0
//				*CAMERA_POS		128.0	2.0	128.0
//				*CAMERA_MIN		2.0
//				*CAMERA_MAX		1000.0
//		}
//	}
std::string cSceneEditMode::GetCameraAsString(){
	std::stringstream ss;
	ss << std::endl;
	ss.precision(2);
	ss << "*CAMERA_LIST {" << std::endl;
	ss << "*CAMERA " << 0 << " {" << std::endl;
	ss << "*CAMERA_TYPE\t" << "*EDIT" << std::endl;
	ss << std::fixed << "*CAMERA_EYE\t" << 0.0f << "\t" << 2.0f << "\t" << -10.0f << std::endl;
	ss << std::fixed << "*CAMERA_LOOKAT\t" << 0.0f << "\t" << 0.0f << "\t" << 2.0f << std::endl;
	ss << std::fixed << "*CAMERA_UP\t" << 0.0f << "\t" << 1.0f << "\t" << 0.0f << std::endl;
	ss << std::fixed << "*CAMERA_XANGLE\t" << 0.0f << std::endl;
	ss << std::fixed << "*CAMERA_YANGLE\t" << 0.0f << std::endl;
	ss << std::fixed << "*CAMERA_DIST\t" << 43.0f << std::endl;
	ss << std::fixed << "*CAMERA_POS\t" << 128.0f << "\t" << 2.0f << "\t" << 128.0f << std::endl;
	ss << std::fixed << "*CAMERA_MIN\t" << 2.0f << std::endl;
	ss << std::fixed << "*CAMERA_MAX\t" << 1000.0f << std::endl;
	ss << "}" << std::endl;
	ss << "}" << std::endl; 
	return ss.str();
}

//		*LIGHTSOURCE_LIST{
//		*LIGHTSOURCE_COUNT 1
//		* LIGHTSOURCE 0 {
//			*LIGHTSOURCE_TYPE	3
//				*LIGHTSOURCE_DIRECTION	1.5 - 1.0	1.0
//				*LIGHTSOURCE_AMBIENT	1.0	1.0	1.0
//				*LIGHTSOURCE_DIFFUSE	1.0	1.0	1.0
//				*LIGHTSOURCE_SPECULAR	1.0	1.0	1.0
//		}
//	}
std::string cSceneEditMode::GetLightAsString(){
	std::stringstream ss;
	ss << std::endl;
	ss.precision(2);
	ss << "*LIGHTSOURCE_LIST {" << std::endl;
	ss << "*LIGHTSOURCE_COUNT " << 1 << std::endl;
	// TODO 차후 멀티 광원쓸때 수정
	ss << "*LIGHTSOURCE 0 { " << std::endl;
	ss << "*LIGHTSOURCE_TYPE\t" << 3 << std::endl;
	ss << "*LIGHTSOURCE_DIRECTION\t" << 1.0f << "\t" << -1.0f << "\t" << 1.0f << std::endl;
	ss << "*LIGHTSOURCE_AMBIENT\t" << 1.0f << "\t" << 1.0f << "\t" << 1.0f << std::endl;
	ss << "*LIGHTSOURCE_DIFFUSE\t" << 1.0f << "\t" << 1.0f << "\t" << 1.0f << std::endl;
	ss << "*LIGHTSOURCE_SPECULAR\t" << 1.0f << "\t" << 1.0f << "\t" << 1.0f << std::endl;
	ss << "}" << std::endl;
	ss << "}" << std::endl;
	return ss.str();
}

//	*UI_LIST{
//		*UI_LIST_PATH "../Resources/SCENE2_UIDATA.txt"
//	}
std::string cSceneEditMode::GetUIListAsString(){
	std::stringstream ss;
	ss << std::endl;
	ss.precision(2);
	ss << "*UI_LIST {" << std::endl;
	ss << "*UI_LIST_PATH " << "\"" << "../Resources/SCENE5_UIDATA.txt" << "\"" << std::endl;
	ss << "}" << std::endl;
	return ss.str();
}

//	*GAMEMAP_LIST{
//		*GAMEMAP_PATH "../Resources/SCENE2_MAPDATA.txt"
//	}
std::string cSceneEditMode::GetGameMapListAsString(){
	std::stringstream ss;
	ss << std::endl;
	ss.precision(2);
	ss << "*GAMEMAP_LIST {" << std::endl;
	ss << "*GAMEMAP_PATH " << "\"" << "../Resources/SCENE5_MAPDATA.txt" << "\"" << std::endl;
	ss << "}" << std::endl;
	return ss.str();
}

//	*GAMEOBJECT_LIST{
//		*GAMEOBJECT_PATH "../Resources/SCENE2_GAMEOBJDATA.txt"
//	}
std::string cSceneEditMode::GetGameObjectListAsString(){
	std::stringstream ss;
	ss << std::endl;
	ss.precision(2);
	ss << "*GAMEOBJECT_LIST {" << std::endl;
	ss << "*GAMEOBJECT_PATH " << "\"" << "../Resources/SCENE5_GAMEOBJDATA.txt" << "\"" << std::endl;
	ss << "}" << std::endl;
	return ss.str();
}

// MapData;
void cSceneEditMode::SaveMapDataToFile(){
	std::stringstream ss;
	FILE* fp;

	fopen_s(&fp, "../Resources/SCENE5_MAPDATA.txt", "w+");
	ss << GetMapInfoAsString() << std::endl;
	ss << GetMapObjectMaterialList() << std::endl;
	ss << GetStaticGameObjectAddedAsString() << std::endl;
	fprintf(fp, ss.str().c_str());

	fclose(fp);
}

std::string cSceneEditMode::GetMapInfoAsString(){
	std::stringstream ss;
	ss << m_pCurrentMap->SaveAsStringInfo();
	return ss.str();
}

std::string cSceneEditMode::GetMapObjectMaterialList(){
	std::string ret = g_pAseManager->GetAseMaterialListAsString();
	return ret;
}

std::string cSceneEditMode::GetStaticGameObjectAddedAsString(){
	std::stringstream ss;
	for (auto p : m_setStaticGameObjectAdded){
		ss << p->SaveAsStringInfo();
	}
	return ss.str();
}


// GameObjectData
void cSceneEditMode::SaveGameObjectDataToFile(){
	std::stringstream ss;
	FILE* fp;

	fopen_s(&fp, "../Resources/SCENE5_GAMEOBJDATA.txt", "w+");
	ss << "*GAMESKYOBJ" << std::endl;
	ss << GetMeshListAsString() << std::endl;
	ss << GetActionGameObjectAddedAsString() << std::endl;
	ss << GetPlayableGameObjectAsString() << std::endl;
	fprintf(fp, ss.str().c_str());

	fclose(fp);
}

std::string cSceneEditMode::GetMeshListAsString(){
	std::string skinnedinfo = g_pSkinnedMeshManager->GetSkinnedMeshListAsString();
	return skinnedinfo;
}

std::string cSceneEditMode::GetActionGameObjectAddedAsString(){
	std::stringstream ss;
	for (auto p : m_setActionGameObjectAdded){
		ss << p->SaveAsStringInfo();
	}
	return ss.str();
}

std::string cSceneEditMode::GetPlayableGameObjectAsString(){
	std::stringstream ss;
	if (m_pPlayableObjectSave){
		ss << m_pPlayableObjectSave->SaveAsStringInfo();
	}
	return ss.str();
}



