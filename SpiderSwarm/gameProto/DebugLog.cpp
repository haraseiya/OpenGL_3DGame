#include "DebugLog.h"
#include "BitMapText.h"

DebugLog::DebugLog()
	: m_debugLogNum(0)
	, m_offsetY(25)
{
	m_font = new BitMapText();
	m_font->SetFontImage(16, 6, "Assets/font.png");
	m_font->ReMapText(" !\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_\tabcdefghijklmnopqrstuvwxyz{|}~\\");
}

DebugLog::~DebugLog()
{
	delete m_font;
}

void DebugLog::ShowDebugLog(const char* format,int param)
{
	buffer->emplace_back();
	sprintf(buffer[256][m_debugLogNum], format, param);
	m_font->TextDraw(50,m_offsetY,buffer[256][m_debugLogNum]);
	m_debugLogNum++;
	m_offsetY += 25;
}
