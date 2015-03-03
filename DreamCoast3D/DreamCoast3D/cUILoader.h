#pragma once
#include "cLoader.h"

class cUIObjManager;
class cUISystemMenu;
class cUIPopupWindow;
class cUIImageButtonMenu;

class cUILoader : public cLoader
{
private:
	void ParseUIList(OUT cUIObjManager* pUIManager);
	void ParseUI(OUT cUIObjManager* pUIManager);
	void ParseShortCutMenu(OUT cUIObjManager* pUIManager);
	void ParseShortCutSet(OUT cUIObjManager* pUIManager, OUT cUISystemMenu* pShortCutMenu);
	cUIPopupWindow* ParseShortCutPopup();
	cUIImageButtonMenu* ParseShortCutButton();

public:
	cUILoader();
	virtual ~cUILoader();

	void LoadGameUIFromFile(OUT cUIObjManager* pUIManager, IN std::string sPath);
	void LoadGameUIFromFile(OUT cUIObjManager* pUIManager, IN std::string sFolder, IN std::string sFile);
};

