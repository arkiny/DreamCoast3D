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
#include "cUIImageButtonMenu.h"


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
				
				LPD3DXSPRITE sprite;
				D3DXCreateSprite(g_pD3DDevice, &sprite);
				cUIImageButtonMenu* pTestButton = new cUIImageButtonMenu(sprite);
				sprite->Release();

				pTestButton->Setup(
					std::string("../Resources/UI/UI_AcceptButton.png"),
					std::string("../Resources/UI/UI_AcceptButtonOn.png"),
					std::string("../Resources/UI/UI_AcceptButtonClick.png"));

				RECT tempRect;
				GetClientRect(g_hWnd, &tempRect);

				float x = tempRect.right - pTestButton->GetSize().fWidth;
				float y = tempRect.bottom - pTestButton->GetSize().fHeight;
				pTestButton->SetPosition(D3DXVECTOR3(x, y - 10.0f, 0));
				p1->AddPopUpWindowAndButton(pTestButton, p2);

				if (pTestButton){
					pUIManager->AddUI(pTestButton);
					pTestButton->Release();
				}
				if (p2){
					pUIManager->AddUI(p2);
					p2->Release();
				}

				cUIInventory* p3 = new cUIInventory;
				p3->Setup();

				pTestButton = new cUIImageButtonMenu(sprite);
				sprite->Release();

				pTestButton->Setup(
					std::string("../Resources/UI/UI_AcceptButton.png"),
					std::string("../Resources/UI/UI_AcceptButtonOn.png"),
					std::string("../Resources/UI/UI_AcceptButtonClick.png"));

				x = tempRect.right - pTestButton->GetSize().fWidth;
				y = tempRect.bottom - pTestButton->GetSize().fHeight;
				pTestButton->SetPosition(D3DXVECTOR3(x - 10.0f - pTestButton->GetSize().fWidth, y - 10.0f, 0));

				p1->AddPopUpWindowAndButton(pTestButton, p3);

				if (pTestButton){
					pUIManager->AddUI(pTestButton);
					pTestButton->Release();
				}
				if (p3){
					pUIManager->AddUI(p3);
					p3->Release();
				}
				if (p1){
					pUIManager->AddUI(p1);
					p1->Release();
				}
			}
		}
	} while (nLevel > 0);

	//return ret;
}