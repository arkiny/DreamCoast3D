#pragma once
#include "cLoader.h"

class cSceneManager;

class cGameLoader : public cLoader
{
public:
	cGameLoader();
	~cGameLoader();

	cSceneManager* LoadFromGameFile(std::string& folderName, std::string& fileName);
};

