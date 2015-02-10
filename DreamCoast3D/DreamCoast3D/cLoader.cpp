#include "stdafx.h"
#include "cLoader.h"


cLoader::cLoader()
{
}


cLoader::~cLoader()
{
}

void cLoader::SkipBlock(){
	int nLevel = 0;
	do{
		char* szToken = GetToken();
		if (isEqual(szToken, "{")){
			++nLevel;
		}
		else if (isEqual(szToken, "}")){
			--nLevel;
		}
	} while (nLevel > 0);
}

char* cLoader::GetToken(){
	int nCurrIndex = 0;
	bool isQuote = false;
	while (char c = fgetc(m_fp)){
		if (feof(m_fp))
			break;

		if (isWhite(c)) {
			if (nCurrIndex == 0 || isQuote)
				continue;
			else
				break;
		}

		if (c == '\"'){
			if (isQuote){
				isQuote = false;
				break;
			}
			else {
				isQuote = true;
				continue;
			}
		}

		m_szToken[nCurrIndex++] = c;
	}

	if (nCurrIndex == 0){
		return NULL;
	}

	m_szToken[nCurrIndex] = '\0';

	return m_szToken;
}