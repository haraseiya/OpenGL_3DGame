#pragma once
#include <vector>

class BitMapText;

class DebugLog
{
public:
	DebugLog();
	~DebugLog();

	void ShowDebugLog(const char* format,int parm);

private:
	BitMapText* m_font;

	std::vector<char*> buffer[256];

	int m_debugLogNum;
	int m_offsetY;
};

