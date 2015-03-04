#include "stdafx.h"
#include "cSceneLoader.h"
#include "cScene.h"
#include "cSceneEditMode.h"
#include "cLightSource.h"
#include "cCamera.h"
#include "cCameraEditing.h"


cSceneLoader::cSceneLoader()
{
}


cSceneLoader::~cSceneLoader()
{
}

cScene* cSceneLoader::ParseScene(char* szFilePath){
	return ParseScene(std::string(szFilePath));
}

cScene* cSceneLoader::ParseScene(std::string sFilePath){
	fopen_s(&m_fp, (sFilePath).c_str(), "r");

	cScene* ret = NULL;

	while (char* szToken = GetToken()){
		if (isEqual(szToken, "*SCENE")){
			ret = new cScene;
			ret->Setup(std::string(""));
		}
		else if (isEqual(szToken, "*SCENE_EDIT")){
			ret = new cSceneEditMode;
			ret->Setup(std::string(""));
		}
		else if (isEqual(szToken, "*CAMERA_LIST")){
			// TODO 차후 다중 카메라 사용시 수정 
			ParseCameraList(ret);
		}
		else if (isEqual(szToken, "*LIGHTSOURCE_LIST")){
			// TODO 차후 다중 광원 사용시 수정
			ParseLightSourceList(ret);
		}
		else if (isEqual(szToken, "*UI_LIST")){
			// TODO 차후 UI 리스트 추가시 수정
			//SkipBlock();
			ParseUIList(ret);
		}
		else if (isEqual(szToken, "*GAMEMAP_LIST")){
			ParseGameMapList(ret);
		}
		else if (isEqual(szToken, "*GAMEOBJECT_LIST")){
			ParseGameObjList(ret);
		}
	}
	return ret;

	fclose(m_fp);
}


void cSceneLoader::ParseLightSourceList(OUT cScene* pScene){
	int nLevel = 0;
	do{
		char* szToken = GetToken();
		if (isEqual(szToken, "{")){
			++nLevel;
		}
		else if (isEqual(szToken, "}")){
			--nLevel;
		}
		else if (isEqual(szToken, "*LIGHTSOURCE_COUNT")){
			int nListCnt = GetInteger();
		}
		else if (isEqual(szToken, "*LIGHTSOURCE")){
			int nIndex = GetInteger();
			cLightSource* pL = ParseLightSource(pScene);
			pScene->AddLightSrc(pL);
			SAFE_RELEASE(pL);
		}
	} while (nLevel > 0);
}

cLightSource* cSceneLoader::ParseLightSource(OUT cScene* pScene){
	cLightSource* ret = new cLightSource;
	// TODO 차후 포인트 광원등등 기타 광원 추가
	D3DXVECTOR3 vDir(0, 0, 0);
	D3DXCOLOR colorAmbient(0, 0, 0, 1)
		, colorDiffuse(0, 0, 0, 1)
		, colorSpecular(0, 0, 0, 1);
	int nType = -1;
	int nLevel = 0;
	do{
		char* szToken = GetToken();
		if (isEqual(szToken, "{")){
			++nLevel;
		}
		else if (isEqual(szToken, "}")){
			--nLevel;
		}
		else if (isEqual(szToken, "*LIGHTSOURCE_TYPE")){
			nType = GetInteger();
		}
		else if (isEqual(szToken, "*LIGHTSOURCE_DIRECTION")){
			float x = GetFloat();
			float y = GetFloat();
			float z = GetFloat();
			vDir = { x, y, z };
		}
		else if (isEqual(szToken, "*LIGHTSOURCE_AMBIENT")){
			float r = GetFloat();
			float g = GetFloat();
			float b = GetFloat();
			colorAmbient = { r, g, b, 1.0f };
		}
		else if (isEqual(szToken, "*LIGHTSOURCE_DIFFUSE")){
			float r = GetFloat();
			float g = GetFloat();
			float b = GetFloat();
			colorDiffuse = { r, g, b, 1.0f };
		}
		else if (isEqual(szToken, "*LIGHTSOURCE_SPECULAR")){
			float r = GetFloat();
			float g = GetFloat();
			float b = GetFloat();
			colorSpecular = { r, g, b, 1.0f };
		}

	} while (nLevel > 0);

	D3DLIGHT9 stLight;
	ZeroMemory(&stLight, sizeof(D3DLIGHT9));
	D3DXVec3Normalize(&vDir, &vDir);
	stLight.Type = (D3DLIGHTTYPE)nType;
	stLight.Direction = vDir;
	stLight.Ambient = colorAmbient;
	stLight.Diffuse = colorDiffuse;
	stLight.Specular = colorSpecular;
	ret->Setup(stLight, vDir, pScene->GetLightSrcSize());
	return ret;
}

void cSceneLoader::ParseCameraList(OUT cScene* pScene){
	int nLevel = 0;
	do{
		char* szToken = GetToken();
		if (isEqual(szToken, "{")){
			++nLevel;
		}
		else if (isEqual(szToken, "}")){
			--nLevel;
		}
		else if (isEqual(szToken, "*CAMERA_COUNT")){
			int nListCnt = GetInteger();
		}
		else if (isEqual(szToken, "*CAMERA")){
			int nIndex = GetInteger();
			cCamera* p = ParseCamera();
			if (p){
				pScene->SetCamera(p);
				SAFE_RELEASE(p);
			}
		}
	} while (nLevel > 0);
}

cCamera* cSceneLoader::ParseCamera(){
	cCamera* pCamera = NULL;
	D3DXVECTOR3 vEye(0, 0, 0), vLookAt(0, 0, 0), vUp(0, 0, 0), vPos(0, 0, 0);
	int ntype = -1;
	float fAngleX(0), fAngleY(0), fDist(0), fMin(5.0f), fMax(1000.0f);
	int nLevel = 0;
	do{
		char* szToken = GetToken();
		if (isEqual(szToken, "{")){
			++nLevel;
		}
		else if (isEqual(szToken, "}")){
			--nLevel;
		}
		else if (isEqual(szToken, "*CAMERA_TYPE")){
			char* type = GetToken();
			if (isEqual(type, "*EDIT")){
				ntype = 1;
			}
			else {
				ntype = 0;
			}
		}
		else if (isEqual(szToken, "*CAMERA_EYE")){
			float x = GetFloat();
			float y = GetFloat();
			float z = GetFloat();
			vEye = { x, y, z };
		}
		else if (isEqual(szToken, "*CAMERA_LOOKAT")){
			float x = GetFloat();
			float y = GetFloat();
			float z = GetFloat();
			vLookAt = { x, y, z };
		}
		else if (isEqual(szToken, "*CAMERA_UP")){
			float x = GetFloat();
			float y = GetFloat();
			float z = GetFloat();
			vUp = { x, y, z };
		}
		else if (isEqual(szToken, "*CAMERA_XANGLE")){
			fAngleX = GetFloat();
		}
		else if (isEqual(szToken, "*CAMERA_YANGLE")){
			fAngleY = GetFloat();
		}
		else if (isEqual(szToken, "*CAMERA_DIST")){
			fDist = GetFloat();
		}
		else if (isEqual(szToken, "*CAMERA_MIN")){
			fMin = GetFloat();
		}
		else if (isEqual(szToken, "*CAMERA_MAX")){
			fMax = GetFloat();
		}
		else if (isEqual(szToken, "*CAMERA_POS")){
			float x = GetFloat();
			float y = GetFloat();
			float z = GetFloat();
			vPos = { x, y, z };
		}
	} while (nLevel > 0);

	if (ntype == 1){
		pCamera = new cCameraEditing;
		pCamera->Setup(vEye, vLookAt, vUp, fAngleX, fAngleY, fDist, fMin, fMax);
		((cCameraEditing*)pCamera)->SetPosition(vPos);
	}
	else{
		pCamera = new cCamera;
		pCamera->Setup(vEye, vLookAt, vUp, fAngleX, fAngleY, fDist, fMin, fMax);
	}
	return pCamera;
}

void cSceneLoader::ParseGameMapList(OUT cScene* pScene){
	int nLevel = 0;
	do{
		char* szToken = GetToken();
		if (isEqual(szToken, "{")){
			++nLevel;
		}
		else if (isEqual(szToken, "}")){
			--nLevel;
		}
		else if (isEqual(szToken, "*GAMEMAP_PATH")){
			std::string sPath = GetToken();
			pScene->SetGameMapPath(sPath);
		}
	} while (nLevel > 0);
}

void cSceneLoader::ParseGameObjList(OUT cScene* pScene){
	int nLevel = 0;
	do{
		char* szToken = GetToken();
		if (isEqual(szToken, "{")){
			++nLevel;
		}
		else if (isEqual(szToken, "}")){
			--nLevel;
		}
		else if (isEqual(szToken, "*GAMEOBJECT_PATH")){
			std::string sPath = GetToken();
			pScene->SetGameObjPath(sPath);
		}
	} while (nLevel > 0);
}

void cSceneLoader::ParseUIList(OUT cScene* pScene){
	int nLevel = 0;
	do{
		char* szToken = GetToken();
		if (isEqual(szToken, "{")){
			++nLevel;
		}
		else if (isEqual(szToken, "}")){
			--nLevel;
		}
		else if (isEqual(szToken, "*UI_LIST_PATH")){
			std::string sPath = GetToken();
			pScene->SetGameUIPath(sPath);
		}
	} while (nLevel > 0);
}