#pragma once

#include "base.h"

int splitString(const string& str, vector<string>& ret_, string sep = ",");


struct TTable;
struct TTableGroup;

/*
func: loadAllGroupTable()
Description: load all config tables and for the checking OF tasks,
call this function while initialize the app.
*/

extern TTable*	getSingleGroupTable( const char* pszTableName );

struct TTable
{
	friend struct TTableGroup;

	bool FindRecords( const string& filedName, int value, vector<int>& recordIds );
	bool GetOneColumFileds( const string& attrName, map<int, int>& attrs );	

	int GetField( int id, int filedID );	
	int GetField( int id, const string& filedName );

	string GetFieldString( int id, int filedID );	
	string GetFieldString( int id, const string& filedName );	

	int GetFiledIndex( const string& filedName );

	TTable()
	{
		m_tableGroup = NULL;
		m_data = NULL;
		m_filedCount = 0;
		m_recordCount = 0;
	}

	~TTable()
	{
		if ( m_data )
		{
			delete m_data;
		}
	}

public:
	int  m_filedCount;
	int  m_recordCount;
	
private:
	int m_id;
	int* m_data;
	TTableGroup* m_tableGroup;

	vector<int>  m_fieldstrKey;

	string m_name;
};

struct TTableGroup
{
	friend struct TTable;

	TTable* GetTable(const string& filedName);
	TTable* GetTable(int index);

	int GetStringKey( const string& str );

	void Load(const string& fileName);

	TTableGroup();
	~TTableGroup();

private:
	vector<TTable*> m_tables;
	vector<string>  m_strKey;

public:
	string m_fileName;
};

class BinTable
{
public:
	BinTable(void);
	~BinTable(void);
	
	static TTableGroup* GetTalbeGroup( const string& fileName );

private:
	vector<TTableGroup*> m_tableGroups;
};
