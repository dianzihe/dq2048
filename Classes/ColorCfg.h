#pragma once

#include "base.h"


#define COLOR_BOOT_EquiPquality 3		//颜色表中，装备品质颜色是以这里开始排列的

#define COLOR_BOOT_Property	3			//其他颜色是从这里开始排列

#define COLOR_BOOT_Property_ColorSize	5.0f	//附加属性的颜色区间



struct ColorData
{
	int	id;								//id
	Color3B		color;
};

class ColorCfg
{
private:
	ColorCfg();
public:
	~ColorCfg();

	static ColorCfg& instance( void )
	{
		static ColorCfg	s;
		return s;
	}

	bool	init( const char* pFile );

	const map<int, ColorData> &getColorCfgTable(){ return m_mapColorCfgTable;};

	const ColorData *getColorCfgData( int id )
	{
		map<int, ColorData>::iterator it = m_mapColorCfgTable.find(id);
		if( it == m_mapColorCfgTable.end() )
			return NULL;
		return &it->second;
	}
	//返回装备品质等级的颜色
	const ColorData *getQualityColorCfgData( int QualityLevel )
	{
		map<int, ColorData>::iterator it = m_mapColorCfgTable.find( QualityLevel+COLOR_BOOT_EquiPquality );
		if( it == m_mapColorCfgTable.end() )
			return NULL;
		return &it->second;
	}
	//返回装备属性的颜色
	const ColorData *getEqyipPropertyColorCfgData( int sizeid )
	{
		map<int, ColorData>::iterator it = m_mapColorCfgTable.find( sizeid+COLOR_BOOT_Property );
		if( it == m_mapColorCfgTable.end() )
			return NULL;
		return &it->second;
	}

	const ColorData*	 getHeroDeadColor( void )	{ return m_pHeroDeadColor; }
	const ColorData*	 getCanAttackColor( void ){ return m_pCanAttackColor; }
	const ColorData*	 getCanNotAttackColor( void ){ return m_pCanNotAttackColor; }
private:
	map<int, ColorData>  m_mapColorCfgTable;

	const ColorData*	 m_pHeroDeadColor;
	const ColorData*	 m_pCanAttackColor;
	const ColorData*	 m_pCanNotAttackColor;
};