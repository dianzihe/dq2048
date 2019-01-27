
#include "base.h"
#include <algorithm>
#include <assert.h>

extern bool _ReloadLocaledDataStr();
extern bool _ReloadLocaledUI();
extern bool _ReloadLocaledText();
//extern bool _ReloadLocaledTask();
extern bool _IsLanguageEnable_LocaledUI( Language type );
extern bool _IsLanguageEnable_LocaledDataStr( Language type );
extern bool _IsLanguageEnable_LocaledText( Language type );
//extern bool _IsLanguageEnable_LocaledTask( Language type );

static Language _curLanguage = LT_Default;

void SetupLanguage( Language type )
{
	if ( _curLanguage != LT_Invalid ) {
		return;
	}
	if ( type != LT_Invalid ) {
		bool lan[ LT_MAX ] = { false };
		int cnt = GetSupportedLanguages( lan );
		if ( cnt > 0 && lan[ type ] ) {
			_curLanguage = type;
			return;
		} else {
			log( "Setup language failed! no language file found!" );
		}
	}
	Language lan[] = { LT_CHS, LT_CHT, LT_KOR, LT_YHZ };
	for ( int i = 0; i < sizeof( lan ) / sizeof( lan[0] ); ++i ) {
		if ( IsLanguageEnable( lan[ i ] ) ) {
			_curLanguage = lan[ i ];
			log("Setup language: %s", GetLanguageTypeString(_curLanguage));
			return;
		}
	}
	log("Setup language failed!");
}

int GetSupportedLanguages( bool lan[ LT_MAX ] )
{
	int count = 0;
	for ( int i = LT_Default; i < LT_MAX; ++i ) {
		lan[ i ] = IsLanguageEnable( ( Language )i );
		if ( lan[ i ] ) {
			++count;
		}
	}
	return count;
}

bool IsLanguageEnable( Language type )
{
	return true;
	/*
	return _IsLanguageEnable_LocaledUI( type ) &&
		_IsLanguageEnable_LocaledDataStr( type ) &&
		_IsLanguageEnable_LocaledText( type ) &&
		_IsLanguageEnable_LocaledTask( type );
		*/
}

bool IsCHTOnly()
{
	return IsLanguageEnable( LT_CHT ) && !IsLanguageEnable( LT_CHS ) && !IsLanguageEnable( LT_KOR ) && !IsLanguageEnable( LT_YHZ );
}

bool IsCHSOnly()
{
	return IsLanguageEnable( LT_CHS ) && !IsLanguageEnable( LT_CHT ) && !IsLanguageEnable( LT_KOR ) && !IsLanguageEnable( LT_YHZ );
}

bool IsKOROnly()
{
	return IsLanguageEnable( LT_KOR ) && !IsLanguageEnable( LT_CHS ) && !IsLanguageEnable( LT_CHT ) && !IsLanguageEnable( LT_YHZ );
}

bool IsYHZOnly()
{
	return  IsLanguageEnable( LT_YHZ ) && IsLanguageEnable( LT_KOR ) && !IsLanguageEnable( LT_CHS ) && !IsLanguageEnable( LT_CHT );
}

const char* GetLanguageTypeString( Language type )
{
	static const char* s[] = {
		"",
		"cht",
		"kor",
		"yhz"
	};
	assert( type >= 0 && type < LT_MAX );
	return s[ type ];
}

Language GetLanguageTypeFromString( const char* _name )
{
	if ( !_name || _name[0] == '\0' ) {
		return LT_Default;
	}
	std::string name( _name );
	std::transform( name.begin(), name.end(), name.begin(), ::tolower );
	if ( name == "cht" ) {
		return LT_CHT;
	} else if(  name == "kor" ){
		return LT_KOR;
	} else if(  name == "yhz" ){
		return LT_KOR;
	}else
	{
		return LT_Default;
	}
}

Language GetCurrentLanguage()
{
	return _curLanguage;
}

bool ChangeLanguage( Language type, bool reload )
{
	if ( !IsLanguageEnable( type ) ) {
		return false;
	}

	_curLanguage = type;

	if ( reload ) {
		return _ReloadLocaledDataStr() &&
			_ReloadLocaledUI() &&
			_ReloadLocaledText();
	}
	return true;
}

bool gLoadTask = false;

void LoadTask()
{
	if (!gLoadTask)
	{
		gLoadTask = true;
		//_ReloadLocaledTask();
	}
}

bool gIsScissorEnabled = false;

void myGLEnableScissorTest()
{
	if (gIsScissorEnabled)
		return;
	gIsScissorEnabled = true;

	glEnable(GL_SCISSOR_TEST);
}

void myGLDisableScissorTest()
{
	if (!gIsScissorEnabled)
		return;
	gIsScissorEnabled = false;
	glDisable(GL_SCISSOR_TEST);
}

bool myIsScissorTestEnabled()
{
	return gIsScissorEnabled;
}


#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)

void CheckMemoryHeap()
{
	if ( !_CrtCheckMemory() ) {
		_asm {
			int 3
		}
	}
}

void DebugLog( char const * format, ...)
{
	//const int MAX_LEN = 10240;
	//char szBuf[MAX_LEN];

	//va_list ap;
	//va_start(ap, format);
	//vsnprintf_s(szBuf, MAX_LEN, MAX_LEN, format, ap);
	//va_end(ap);

	//SYSTEMTIME stLocalTime;
	//GetLocalTime( &stLocalTime );
	//WCHAR szTime[256];
	//swprintf_s( szTime, 256, L"[%04d_%02d_%02d-%02d_%02d_%02d] ",
	//	stLocalTime.wYear, stLocalTime.wMonth, stLocalTime.wDay, 
	//	stLocalTime.wHour, stLocalTime.wMinute, stLocalTime.wSecond );
	//OutputDebugStringW(szTime);

	//WCHAR wszBuf[MAX_LEN] = {0};
	//MultiByteToWideChar(CP_UTF8, 0, szBuf, -1, wszBuf, sizeof(wszBuf));
	//OutputDebugStringW(wszBuf);

	//char szOutLog[MAX_LEN];
	//sprintf(szOutLog,"[%04d_%02d_%02d-%02d_%02d_%02d] [DEBUG] %s\n\0",
	//	    stLocalTime.wYear, stLocalTime.wMonth, stLocalTime.wDay, 
	//	    stLocalTime.wHour, stLocalTime.wMinute, stLocalTime.wSecond, szBuf);

	//Log::GetInstance().WriteLog(szOutLog, strlen(szOutLog));
}

void ErrorLog(  char const * format, ... )
{
	//const int MAX_LEN = 10240;
	//char szBuf[MAX_LEN];

	//va_list ap;
	//va_start(ap, format);
	//vsnprintf_s(szBuf, MAX_LEN, MAX_LEN, format, ap);
	//va_end(ap);	

	//SYSTEMTIME stLocalTime;
	//GetLocalTime( &stLocalTime );
	//char szOutLog[MAX_LEN];
	//sprintf(szOutLog,"[%04d_%02d_%02d-%02d_%02d_%02d] [ERROR] %s\n\0",
	//	stLocalTime.wYear, stLocalTime.wMonth, stLocalTime.wDay, 
	//	stLocalTime.wHour, stLocalTime.wMinute, stLocalTime.wSecond, szBuf);

	//Log::GetInstance().WriteLog(szOutLog, strlen(szOutLog));
}

void InfoLog(  char const * format, ... )
{
	//const int MAX_LEN = 10240;
	//char szBuf[MAX_LEN];

	//va_list ap;
	//va_start(ap, format);
	//vsnprintf_s(szBuf, MAX_LEN, MAX_LEN, format, ap);
	//va_end(ap);	

	//SYSTEMTIME stLocalTime;
	//GetLocalTime( &stLocalTime );
	//char szOutLog[MAX_LEN];
	//sprintf(szOutLog,"[%04d_%02d_%02d-%02d_%02d_%02d] [INFO] %s\n\0",
	//	stLocalTime.wYear, stLocalTime.wMonth, stLocalTime.wDay, 
	//	stLocalTime.wHour, stLocalTime.wMinute, stLocalTime.wSecond, szBuf);

	//Log::GetInstance().WriteLog(szOutLog, strlen(szOutLog));
}

#else


#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "..\libiconv-1.14\include\iconv.h"
#else
#include "iconv.h"
#endif

void DebugLog(const char * format, ...)
{
}

void ErrorLog( const char * format, ... )
{
	CCLog("format");
}

void WarningLog( const char * format, ... )
{

}

 

int GBKToUTF8(string & gbkStr)  
{  
	const char* toCode = "gb2312";
	const char* fromCode = "utf-8";


	iconv_t iconvH;  
	iconvH = iconv_open(fromCode, toCode);  
	if (iconvH == 0)  
	{  
		return -1;  
	}  

	const char* strChar = gbkStr.c_str();  
	const char** pin = &strChar;  
	size_t strLength = gbkStr.length();  
	char* outbuf = (char*) malloc(strLength*4); 
 
	char* pBuff = outbuf;  

	memset( outbuf, 0, strLength*4);  
	size_t outLength = strLength*4;  

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	if (-1 == iconv(iconvH, pin, &strLength, (char **)(&pBuff), &outLength))  
#else
	if (-1 == iconv(iconvH, (char **)pin, &strLength, (char **)(&pBuff), &outLength))  
#endif
	{  
		iconv_close(iconvH);  
        
        free(outbuf);
		return -1;  
	}  
	gbkStr = outbuf;  
    free(outbuf);
	iconv_close(iconvH); 
	return 0;  
}
#endif

//////////////////////////////////////////////////////////////////////////
// 返回一段内存数据的crc值，32位的
unsigned long MakeCRC32( const void* pBuffer, unsigned long cbBuffer, unsigned long crc )
{
	static const unsigned long s_Crc32Table[256] =
	{
		0x00000000, 0x77073096, 0xEE0E612C, 0x990951BA,
		0x076DC419, 0x706AF48F, 0xE963A535, 0x9E6495A3,
		0x0EDB8832, 0x79DCB8A4, 0xE0D5E91E, 0x97D2D988,
		0x09B64C2B, 0x7EB17CBD, 0xE7B82D07, 0x90BF1D91,
		0x1DB71064, 0x6AB020F2, 0xF3B97148, 0x84BE41DE,
		0x1ADAD47D, 0x6DDDE4EB, 0xF4D4B551, 0x83D385C7,
		0x136C9856, 0x646BA8C0, 0xFD62F97A, 0x8A65C9EC,
		0x14015C4F, 0x63066CD9, 0xFA0F3D63, 0x8D080DF5,
		0x3B6E20C8, 0x4C69105E, 0xD56041E4, 0xA2677172,
		0x3C03E4D1, 0x4B04D447, 0xD20D85FD, 0xA50AB56B,
		0x35B5A8FA, 0x42B2986C, 0xDBBBC9D6, 0xACBCF940,
		0x32D86CE3, 0x45DF5C75, 0xDCD60DCF, 0xABD13D59,
		0x26D930AC, 0x51DE003A, 0xC8D75180, 0xBFD06116,
		0x21B4F4B5, 0x56B3C423, 0xCFBA9599, 0xB8BDA50F,
		0x2802B89E, 0x5F058808, 0xC60CD9B2, 0xB10BE924,
		0x2F6F7C87, 0x58684C11, 0xC1611DAB, 0xB6662D3D,

		0x76DC4190, 0x01DB7106, 0x98D220BC, 0xEFD5102A,
		0x71B18589, 0x06B6B51F, 0x9FBFE4A5, 0xE8B8D433,
		0x7807C9A2, 0x0F00F934, 0x9609A88E, 0xE10E9818,
		0x7F6A0DBB, 0x086D3D2D, 0x91646C97, 0xE6635C01,
		0x6B6B51F4, 0x1C6C6162, 0x856530D8, 0xF262004E,
		0x6C0695ED, 0x1B01A57B, 0x8208F4C1, 0xF50FC457,
		0x65B0D9C6, 0x12B7E950, 0x8BBEB8EA, 0xFCB9887C,
		0x62DD1DDF, 0x15DA2D49, 0x8CD37CF3, 0xFBD44C65,
		0x4DB26158, 0x3AB551CE, 0xA3BC0074, 0xD4BB30E2,
		0x4ADFA541, 0x3DD895D7, 0xA4D1C46D, 0xD3D6F4FB,
		0x4369E96A, 0x346ED9FC, 0xAD678846, 0xDA60B8D0,
		0x44042D73, 0x33031DE5, 0xAA0A4C5F, 0xDD0D7CC9,
		0x5005713C, 0x270241AA, 0xBE0B1010, 0xC90C2086,
		0x5768B525, 0x206F85B3, 0xB966D409, 0xCE61E49F,
		0x5EDEF90E, 0x29D9C998, 0xB0D09822, 0xC7D7A8B4,
		0x59B33D17, 0x2EB40D81, 0xB7BD5C3B, 0xC0BA6CAD,

		0xEDB88320, 0x9ABFB3B6, 0x03B6E20C, 0x74B1D29A,
		0xEAD54739, 0x9DD277AF, 0x04DB2615, 0x73DC1683,
		0xE3630B12, 0x94643B84, 0x0D6D6A3E, 0x7A6A5AA8,
		0xE40ECF0B, 0x9309FF9D, 0x0A00AE27, 0x7D079EB1,
		0xF00F9344, 0x8708A3D2, 0x1E01F268, 0x6906C2FE,
		0xF762575D, 0x806567CB, 0x196C3671, 0x6E6B06E7,
		0xFED41B76, 0x89D32BE0, 0x10DA7A5A, 0x67DD4ACC,
		0xF9B9DF6F, 0x8EBEEFF9, 0x17B7BE43, 0x60B08ED5,
		0xD6D6A3E8, 0xA1D1937E, 0x38D8C2C4, 0x4FDFF252,
		0xD1BB67F1, 0xA6BC5767, 0x3FB506DD, 0x48B2364B,
		0xD80D2BDA, 0xAF0A1B4C, 0x36034AF6, 0x41047A60,
		0xDF60EFC3, 0xA867DF55, 0x316E8EEF, 0x4669BE79,
		0xCB61B38C, 0xBC66831A, 0x256FD2A0, 0x5268E236,
		0xCC0C7795, 0xBB0B4703, 0x220216B9, 0x5505262F,
		0xC5BA3BBE, 0xB2BD0B28, 0x2BB45A92, 0x5CB36A04,
		0xC2D7FFA7, 0xB5D0CF31, 0x2CD99E8B, 0x5BDEAE1D,

		0x9B64C2B0, 0xEC63F226, 0x756AA39C, 0x026D930A,
		0x9C0906A9, 0xEB0E363F, 0x72076785, 0x05005713,
		0x95BF4A82, 0xE2B87A14, 0x7BB12BAE, 0x0CB61B38,
		0x92D28E9B, 0xE5D5BE0D, 0x7CDCEFB7, 0x0BDBDF21,
		0x86D3D2D4, 0xF1D4E242, 0x68DDB3F8, 0x1FDA836E,
		0x81BE16CD, 0xF6B9265B, 0x6FB077E1, 0x18B74777,
		0x88085AE6, 0xFF0F6A70, 0x66063BCA, 0x11010B5C,
		0x8F659EFF, 0xF862AE69, 0x616BFFD3, 0x166CCF45,
		0xA00AE278, 0xD70DD2EE, 0x4E048354, 0x3903B3C2,
		0xA7672661, 0xD06016F7, 0x4969474D, 0x3E6E77DB,
		0xAED16A4A, 0xD9D65ADC, 0x40DF0B66, 0x37D83BF0,
		0xA9BCAE53, 0xDEBB9EC5, 0x47B2CF7F, 0x30B5FFE9,
		0xBDBDF21C, 0xCABAC28A, 0x53B39330, 0x24B4A3A6,
		0xBAD03605, 0xCDD70693, 0x54DE5729, 0x23D967BF,
		0xB3667A2E, 0xC4614AB8, 0x5D681B02, 0x2A6F2B94,
		0xB40BBE37, 0xC30C8EA1, 0x5A05DF1B, 0x2D02EF8D,
	};


	for ( unsigned long i = 0; i < cbBuffer; ++i )
		crc = ((crc) >> 8) ^ s_Crc32Table[(((char const*)pBuffer)[i]) ^ ((crc) & 0x000000FF)];
	return crc;
}


bool	isNeedChangeName( string name )
{
	int linePos = name.find_last_of('_');
	if( linePos != string::npos )
	{
		if( linePos>=(int)name.size()-1 )
		{
			return false;
		}
		else
		{
			for( int i=linePos+1; i<(int)name.size(); ++i )
			{
				if( name.c_str()[i] > '9' || name.c_str()[i]<0 )
				{
					return false;
				}
			}
		}
		return true;
	}
	return false;
}


std::string _fixPathByLanguage( const char* resName )
{	
	std::string _resName;
	std::string dirName( resName );
	std::string::size_type p = dirName.find( '/' );
	if ( p != std::string::npos ) {
		_resName = dirName.substr( p );
		dirName = dirName.substr( 0, p );
	} else {
		_resName = resName;
	}
	std::string _resPath( dirName );
	const char* lan = GetLanguageTypeString( GetCurrentLanguage() );
	if ( lan && lan[0] != '\0' ) {
		_resPath.append( "/" );
		_resPath.append( lan );
	}
	_resPath.append( _resName );
	return _resPath;
}

bool _isTextUTF8( const char* str, size_t length )
{
	int bytes = 0;
	unsigned char chr;
	bool allAscii = true;
	for ( size_t i = 0; i < length; ++i ) {
		chr= *( str + i );
		if ( ( chr & 0x80 ) != 0 ) {
			allAscii= false;
		}
		if ( bytes == 0 ) {
			if ( chr >= 0x80 ) {
				if ( chr >= 0xFC && chr <= 0xFD )
					bytes = 6;
				else if ( chr >= 0xF8 )
					bytes = 5;
				else if ( chr >= 0xF0 )
					bytes = 4;
				else if ( chr>=0xE0 )
					bytes = 3;
				else if ( chr>=0xC0 )
					bytes = 2;
				else {
					return false;
				}
				--bytes;
			}
		} else {
			if ( ( chr & 0xC0 ) != 0x80 ) {
				return false;
			}
			--bytes;
		}
	}
	if ( bytes > 0 ) {
		return false;
	}
	if ( allAscii ) {
		return false;
	}
	return true;
}