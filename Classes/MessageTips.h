#pragma once
#include "base.h"
#include "ui.h"
#include "platform.h"

struct	MSGTips_Data 
{
	UI			*m_pUI;
	UIText	*m_pText;
	string	m_String;
	int		m_iIndex;
	long		m_timeBegin;
	float		m_fSpeed;
};	

class MessageTips
{
public:
	MessageTips(void);
	~MessageTips(void);

	static MessageTips & instance()
	{
		static MessageTips s;
		return s;
	};

	static void showMessageTips( string strText);
	static void showMessageTips( string strText, ccColor3B &color );

	void unInit();

	void removeFontUI(UI *ui);
private:
	list<MSGTips_Data> m_listMSGTips;
	int		m_iTipsCounter;
};
