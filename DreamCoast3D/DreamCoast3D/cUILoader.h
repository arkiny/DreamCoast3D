#pragma once
#include "cLoader.h"

class cUIObjManager;

class cUILoader : public cLoader
{
private:
	void ParseUIList(OUT cUIObjManager* pUIManager);
	void ParseUI(OUT cUIObjManager* pUIManager);

public:
	cUILoader();
	virtual ~cUILoader();

	void LoadGameUIFromFile(OUT cUIObjManager* pUIManager, IN std::string sPath);
	void LoadGameUIFromFile(OUT cUIObjManager* pUIManager, IN std::string sFolder, IN std::string sFile);
};

