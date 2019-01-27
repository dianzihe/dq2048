#include "ColorCfg.h"
#include "BinTable.h"

ColorCfg::ColorCfg()
{
	m_pHeroDeadColor = NULL;
	m_pCanAttackColor = NULL;
	m_pCanNotAttackColor = NULL;
	init("color");
}

ColorCfg::~ColorCfg()
{
}


bool	ColorCfg::init( const char* pFile )
{
	if( pFile == NULL )
		return false;
	m_mapColorCfgTable.clear(); 

	TTableGroup group;
	group.Load(pFile);

	TTable* pTable =  group.GetTable( "color" );
	if( !pTable )
		return false;

	for( int i=1; i<=pTable->m_recordCount; ++i )
	{
		ColorData data;
		data.id = pTable->GetField(i, "id");
		data.color.r= pTable->GetField(i, "red");
		data.color.g = pTable->GetField(i, "green");
		data.color.b = pTable->GetField(i, "blue");
		m_mapColorCfgTable[data.id] = data;
	}

	m_pHeroDeadColor = getColorCfgData(9);
	m_pCanAttackColor = getColorCfgData(1);
	m_pCanNotAttackColor = getColorCfgData(20);
	return true;
}