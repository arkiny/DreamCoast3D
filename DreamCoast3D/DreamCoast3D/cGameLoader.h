#pragma once
#include "cLoader.h"

class cSceneManager;


class cGameLoader : public cLoader
{
private:
	void ParseSceneList(OUT cSceneManager* pSceneMng);
	

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

