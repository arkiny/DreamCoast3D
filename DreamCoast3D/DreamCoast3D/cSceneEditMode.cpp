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
#include <sstream>

static CRITICAL_SECTION				gCriticalSectionEditMode;

cSceneEditMode::cSceneEditMode()
	:m_pMousPicking(NULL)
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

	for (auto p : m_mapLoadedMap){
		SAFE_RELEASE(p.second);
	}
	for (auto p : m_vecStaticGameObjectPreset){
		SAFE_RELEASE(p);
	}
	for (auto p : m_vecStaticGameObjectAdded){
		SAFE_RELEASE(p);
	}
	for (auto p : m_vecActionGameObjectPreset){
		SAFE_RELEASE(p);
	}
	for (auto p : m_vecActionGameObjectAdded){
		SAFE_RELEASE(p);
	}
	for (auto p : m_vecGamePlayableObjectPreset){
		SAFE_RELEASE(p);
	}
	SAFE_RELEASE(m_pPlayableObjectSave);
}

void cSceneEditMode::Setup(std::string sFilePath){
	cScene::Setup(sFilePath);
	m_pMousPicking = new cMousePicking;
	m_pMousPicking->SetEffectDelegate(m_pEffectManager);
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
		
		for (auto p : m_vecActionGameObjectAdded){
			p->Render();
		}

		for (auto p : m_vecStaticGameObjectAdded){
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
	m_vecActionGameObjectPreset.push_back(pGameObject);
	pGameObject->AddRef();
}

void cSceneEditMode::AddGamePlayableObjectToPreset(cGamePlayableObject* pGameObject){
	m_vecGamePlayableObjectPreset.push_back(pGameObject);
	pGameObject->AddRef();
}

void cSceneEditMode::AddStaticGameObjectToPreset(cGameObject* pGameObject){
	m_vecStaticGameObjectPreset.push_back(pGameObject);
	pGameObject->AddRef();
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
		m_vecActionGameObjectAdded.push_back(p);
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
		m_vecStaticGameObjectAdded.push_back(p);
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