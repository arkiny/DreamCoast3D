#include "stdafx.h"
#include "cUILoader.h"
#include "cUIObjManager.h"
#include "cUIMinimap.h"
#include "cUISkillShortCut.h"
#include "cUIStatWindow.h"
#include "cUIExpBar.h"


cUILoader::cUILoader()
{
}


cUILoader::~cUILoader()
{
}

void cUILoader::LoadGameUIFromFile(OUT cUIObjManager* pUIManager, IN std::string sPath){
	fopen_s(&m_fp, sPath.c_str(), "r");

	while (char* szToken = GetToken()){
		if (isEqual(szToken, "*UI_LIST")){
			ParseUIList(pUIManager);
		}
		else if (isEqual(szToken, "#")){
			SkipBlock();
		}
	}

	fclose(m_fp);
}

void cUILoader::LoadGameUIFromFile(OUT cUIObjManager* pUIManager, IN std::string sFolder, IN std::string sFile){
	LoadGameUIFromFile(pUIManager, sFolder + sFile);
}

void cUILoader::ParseUIList(OUT cUIObjManager* pUIManager){
	int nLevel = 0;
	do{
		char* szToken = GetToken();
		if (isEqual(szToken, "{")){
			++nLevel;
		}
		else if (isEqual(szToken, "}")){
			--nLevel;
		}
		else if (isEqual(szToken, "*UI_COUNT")){
			int nUI = GetInteger();
		}
		else if (isEqual(szToken, "*UI")){
			int nUINum = GetInteger();
			cUIObject* p = ParseUI();
			if (p){
				pUIManager->AddUI(p);
				SAFE_RELEASE(p);
			}
		}
	} while (nLevel > 0);
}

cUIObject* cUILoader::ParseUI(){
	cUIObject* ret = NULL;
	int nLevel = 0;
	do{
		char* szToken = GetToken();
		if (isEqual(szToken, "{")){
			++nLevel;
		}
		else if (isEqual(szToken, "}")){
			--nLevel;
		}
		else if (isEqual(szToken, "*UI_TYPE")){
			char* szTypeName = GetToken();
			if (isEqual(szTypeName, "UISTAT")){
				ret = new cUIStatWindow;
				ret->Setup();
			}
			else if (isEqual(szTypeName, "UISKILL")){
				ret = new cUISkillShortCut;
				ret->Setup();
			}
			else if (isEqual(szTypeName, "UIEXP")){
				ret = new cUIExpBar;
				ret->Setup();
			}
			else if (isEqual(szTypeName, "UIMINIMAP")){
				ret = new cUIMinimap;
				ret->Setup();
			}
		}
	} while (nLevel > 0);

	return ret;
}