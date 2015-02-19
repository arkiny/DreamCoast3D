#pragma once
#include "cLoader.h"

class cSceneManager;
class cScene;
class cLightSource;
class cCamera;

class cGameLoader : public cLoader
{
private:
	void ParseSceneList(OUT cSceneManager* pSceneMng);
	void ParseScene(OUT cScene* pScene);
	
	void ParseGameMapList(OUT cScene* pScene);
	void ParseGameObjList(OUT cScene* pScene); 
	void ParseUIList(OUT cScene* pScene);
	
	void ParseLightSourceList(OUT cScene* pScene);
	cLightSource* ParseLightSource(OUT cScene* pScene);

	void ParseCameraList(OUT cScene* pScene);
	cCamera* ParseCamera();

public:
	cGameLoader();
	~cGameLoader();

	void LoadFromGameFile(
		OUT cSceneManager* pSceneMng,
		std::string& sPath);
	void LoadFromGameFile(
		OUT cSceneManager* pSceneMng, 
		std::string& sFolder, 
		std::string& sFile);
};

