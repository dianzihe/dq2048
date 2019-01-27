#include "Text.h"
#include "base.h"

static std::string _GetLocaledTextFileFullPath( Language type )
{
	const char* postFix = GetLanguageTypeString( type );
	std::string filePath( "Data/Config/gameUTF8" );
	if ( postFix[0] != '\0' ) {
		filePath.push_back( '_' );
		filePath.append( postFix );
	}
	filePath.append( ".text" );
	return FileUtils::getInstance()->fullPathForFilename(filePath.c_str());
}

bool _ReloadLocaledText()
{
	CText::Instance()->Uninit();
	return CText::Instance()->Init();
}

bool _IsLanguageEnable_LocaledText( Language type )
{
	return CCFileUtils::sharedFileUtils()->isFileExist( _GetLocaledTextFileFullPath( type ).c_str() );
}


CText::CText()
{
	m_count = 0;
	Uninit();
    Init();
}

CText::~CText()
{
	Uninit();
}

CText* CText::Instance()
{
	static CText gText;
	return &gText;
}

bool CText::Init()
{
	ssize_t len = 0;
	char* data = (char*)CCFileUtils::sharedFileUtils()->getFileData( _GetLocaledTextFileFullPath( GetCurrentLanguage() ).c_str(), "rb", &len );
	if ( !data ) {
		return false;
	}
	int off = 0;
	while(off < (int)len)
	{
		int next = off + 1;
		while(next < (int)len)
		{
			//if(data[next] == '\r' && data[next + 1] == '\n')
			if(data[next] == '^')
				break;
			next++;
		}
		if(next >= (int)len)
			break;
		char buf[1024] = {0};
		memcpy(buf, &(data[off]), next - off);
		string s(buf);
		m_textCache.push_back(s);
		m_count++;
		off = next + 1;
	}
	SAFE_DELETE_ARRAY(data);
	return true;
}

void CText::Uninit()
{
    m_count = 0;
	m_textCache.clear();
}

std::string CText::GetText( int id )
{
	if(id < 0 || id >= CText::Instance()->m_count)
		return " ";
	return CText::Instance()->m_textCache[id];
}