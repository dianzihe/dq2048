#pragma  once
#include "base.h"
#include <string>

#define TL_COLOR						'C'
#define TL_COLOR_END					"/C"
#define TL_PLAYERNAME					'N'
#define TL_PLAYERNAME_END				"/N"
#define TL_ITEM							'I'
#define TL_ITEM_END						"/I"
#define TL_PET							'P'
#define TL_PET_NED						"/P"
#define TL_COLOR_ID						'O'
#define TL_COLOR_ID_END					"/O"
#define TL_PLAYER_NAME					"playername"
#define TL_GUILD_NAME					"guildname"
#define TL_FACTION_NAME					"factionname"
#define TL_PLAYER_SEX					"sex"
#define TL_PET_NAME						"petname"
#define TL_MATE_NAME					"matename"
#define TIL_ITEM_NAME					'X'
#define TIL_EQUIP_NAME					'E'
#define TIL_ADDRESS_SHORT				'A'
#define TIL_ADDRESS						'D'
#define TIL_ADDRESS_END					"/D"
#define TIL_ADDRESS_NEWLINE				'L'
#define TIL_VIP							'V'
#define TIL_MONSTER_NAME				'M'
#define TIL_MAP_NAME					'B'
#define TIL_SKILL_NAME					'S'
#define TIL_WORLDBOSS_NAME				'W'


enum FONT_SIZE
{
	FONT_SIZE_20		= 20,
	FONT_SIZE_24		= 24,
};

#define		EXP_MAP_NUM	5		//表情最大数量


static std::string _fixFontPath( const char* resName )
{
	if( strcmp( resName, FONT_BMP_24 ) != 0 )
	{
		return resName;
	}
	std::string _resName;
	std::string dirName( resName );
	std::string::size_type p = dirName.find_last_of( '.' );
	if ( p != std::string::npos ) {
		_resName = dirName.substr( p );
		dirName = dirName.substr( 0, p );
	} else {
		_resName = resName;
	}
	std::string _resPath( dirName );
	const char* lan = GetLanguageTypeString( GetCurrentLanguage() );
	if ( lan && lan[0] != '\0' ) {
		_resPath.append( "_" );
		_resPath.append( lan );
	}
	_resPath.append( _resName );
	return _resPath;
}