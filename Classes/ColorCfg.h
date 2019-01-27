#pragma once

#include "base.h"


#define COLOR_BOOT_EquiPquality 3		//��ɫ���У�װ��Ʒ����ɫ�������￪ʼ���е�

#define COLOR_BOOT_Property	3			//������ɫ�Ǵ����￪ʼ����

#define COLOR_BOOT_Property_ColorSize	5.0f	//�������Ե���ɫ����



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
	//����װ��Ʒ�ʵȼ�����ɫ
	const ColorData *getQualityColorCfgData( int QualityLevel )
	{
		map<int, ColorData>::iterator it = m_mapColorCfgTable.find( QualityLevel+COLOR_BOOT_EquiPquality );
		if( it == m_mapColorCfgTable.end() )
			return NULL;
		return &it->second;
	}
	//����װ�����Ե���ɫ
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