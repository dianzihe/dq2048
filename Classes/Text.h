#ifndef __TEXT_H__
#define __TEXT_H__

#include "base.h"

typedef std::vector<string> TextCache;

class CText
{
	CC_SYNTHESIZE(int, m_count, Count);
public:
	CText();
	~CText();
	static CText* Instance();

	bool Init();
	void Uninit();

	static string GetText(int id);
private:
	TextCache m_textCache;
};
#endif
//EOF
