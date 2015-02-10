#pragma once

class cLoader
{
protected:
	char m_szToken[1024];
	FILE* m_fp;

protected:
	bool isWhite(char c){
		return c < 33;
	}

	bool isEqual(char* szStr1, char* szStr2){
		return strcmp(szStr1, szStr2) == 0;
	}

	float GetFloat(){
		return (float)atof(GetToken());
	}

	int GetInteger(){
		return atoi(GetToken());
	}

	void SkipBlock();
	char* GetToken();

public:
	cLoader();
	~cLoader();
};

