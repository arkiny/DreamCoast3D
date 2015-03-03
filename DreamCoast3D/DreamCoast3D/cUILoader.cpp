#include "stdafx.h"
#include "cUILoader.h"
#include "cUIObjManager.h"
#include "cUIMinimap.h"
#include "cUISkillShortCut.h"
#include "cUIStatWindow.h"
#include "cUIExpBar.h"
#include "cUIInventory.h"
#include "cUISystemTest.h"
#include "cUISystemMenu.h"
#include "cUIPopupWindow.h"


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
			ParseUI(pUIManager);
		}
	} while (nLevel > 0);
}

void cUILoader::ParseUI(OUT cUIObjManager* pUIManager){
	//cUIObject* ret = NULL;
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
				cUIObject* p = new cUIStatWindow;
				p->Setup();
				if (p){
					pUIManager->AddUI(p);
					SAFE_RELEASE(p);
				}
			}
			else if (isEqual(szTypeName, "UISKILL")){
				cUIObject* p = new cUISkillShortCut;
				p->Setup();
				if (p){
					pUIManager->AddUI(p);
					SAFE_RELEASE(p);
				}
			}
			else if (isEqual(szTypeName, "UIEXP")){
				cUIObject* p = new cUIExpBar;
				p->Setup();
				if (p){
					pUIManager->AddUI(p);
					SAFE_RELEASE(p);
				}
			}
			else if (isEqual(szTypeName, "UIMINIMAP")){
				cUIObject* p = new cUIMinimap;
				p->Setup();
				if (p){
					pUIManager->AddUI(p);
					SAFE_RELEASE(p);
				}
			}

			else if (isEqual(szTypeName, "UIINVENTORY")){
				cUIObject* p = new cUIInventory;
				p->Setup();
				if (p){
					pUIManager->AddUI(p);
					SAFE_RELEASE(p);
				}
			}
			else if (isEqual(szTypeName, "UIMAIN")){
				cUIObject* p = new cUISystemTest;
				p->Setup();
				if (p){
					pUIManager->AddUI(p);
					SAFE_RELEASE(p);
				}
			}
			else if (isEqual(szTypeName, "UISYSTEMMENU")){
				cUISystemMenu* p1 = new cUISystemMenu;
				p1->Setup();

				cUIPopupWindow* p2 = new cUIPopupWindow;
				p2->Setup();
				
				p1->SetPopUpSystemMenuUI(p2);

				if (p1){
					pUIManager->AddUI(p1);
					SAFE_RELEASE(p1);
				}

				if (p2){
					pUIManager->AddUI(p2);
					SAFE_RELEASE(p2);
				}
			}
		}
	} while (nLevel > 0);

	//return ret;
}