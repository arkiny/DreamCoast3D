#pragma once
#include "cLoader.h"

class cScene;
class cLightSource;
class cCamera;

class cSceneLoader : public cLoader
{
protected:
	void ParseGameMapList(OUT cScene* pScene);
	void ParseGameObjList(OUT cScene* pScene);
	void ParseUIList(OUT cScene* pScene);

	void ParseLightSourceList(OUT cScene* pScene);
	cLightSource* ParseLightSource(OUT cScene* pScene);

	void ParseCameraList(OUT cScene* pScene);
	cCamera* ParseCamera();

public:
	cSceneLoader();
	~cSceneLoader();

	cScene* ParseScene(char* szFilePath);
	cScene* ParseScene(std::string sFilePath);
};

