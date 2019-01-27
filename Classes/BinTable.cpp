#include "BinTable.h"
//#include "Misc.h"
#include "base.h"

static std::string _GetLocaledDataStrFileFullPath( Language type )
{
	const char* postFix = GetLanguageTypeString( type );
	std::string filePath( "Data/Data_utf8str" );
	if ( postFix[0] != '\0' ) {
		filePath.push_back( '_' );
		filePath.append( postFix );
	}
	filePath.append( ".bin" );
	return FileUtils::getInstance()->fullPathForFilename(filePath.c_str());
}

bool _IsLanguageEnable_LocaledDataStr( Language type )
{
	return FileUtils::getInstance()->isFileExist( _GetLocaledDataStrFileFullPath( type ).c_str() );
}

bool _ReloadLocaledDataStr();

static bool loaded = false;
static vector<string> table;
static const vector<string>& _GetLocalTextFile()
{
	if ( loaded ) {
		return table;
	}
	loaded = true;
	_ReloadLocaledDataStr();
	return table;
}

bool _ReloadLocaledDataStr()
{
	table.clear();
	std::string fullPath = _GetLocaledDataStrFileFullPath( GetCurrentLanguage() );

	ssize_t lenth;
	unsigned char* buffer = CCFileUtils::sharedFileUtils()->getFileData( fullPath.c_str(), "rb", & lenth );
	
	unsigned pos = 0;
	while( pos < lenth )
	{
		int size = 0;
		memcpy( &size, buffer+pos, sizeof(int) );
		pos += sizeof(int);
		if( size > 0 )
		{
			char *pStr = new char[size+1];
			memcpy( pStr, buffer+pos, sizeof(char)*size );
			pStr[size] = '\0';
			table.push_back( pStr );
			SAFE_DELETE_ARRAY( pStr );
		}
		else
		{
			table.push_back( "" );
		}
		pos += size;
	}
	SAFE_DELETE_ARRAY( buffer );
	loaded = true;
	return true;
}

TTable*	getSingleGroupTable( const char* pszTableName )
{
	if( !pszTableName )
		return NULL;
	TTableGroup* g = BinTable::GetTalbeGroup( pszTableName );
	if( !g )
		return NULL;

	return g->GetTable(pszTableName);
}

int splitString(const string& str, vector<string>& ret_, string sep)
{
	if (str.empty())
	{
		return 0;
	}

	string tmp;
	string::size_type pos_begin = str.find_first_not_of(sep);
	string::size_type comma_pos = 0;

	while (pos_begin != string::npos)
	{
		comma_pos = str.find(sep, pos_begin);
		if (comma_pos != string::npos)
		{
			tmp = str.substr(pos_begin, comma_pos - pos_begin);
			pos_begin = comma_pos + sep.length();
		}
		else
		{
			tmp = str.substr(pos_begin);
			pos_begin = comma_pos;
		}

		if (!tmp.empty())
		{
			ret_.push_back(tmp);
			tmp.clear();
		}
	}
	return 0;
}

BinTable::BinTable(void)
{
}

BinTable::~BinTable(void)
{
	for (size_t i=0; i<m_tableGroups.size(); i++)
	{
		delete m_tableGroups[i];
	}
	
}

TTableGroup* BinTable::GetTalbeGroup( const string& fileName )
{
	static BinTable bin;

	for (size_t i=0; i<bin.m_tableGroups.size(); i++)
	{
		if ( bin.m_tableGroups[i]->m_fileName == fileName )
		{
			return bin.m_tableGroups[i];
		}
	}

	// Load
	TTableGroup* tableGroup = new TTableGroup();

	tableGroup->Load(fileName);

	bin.m_tableGroups.push_back(tableGroup);

	return tableGroup;
}


// 
// class:TTable
//
int TTable::GetField( int id, int filedID )
{
	if ( id <= 0 ) return 0;
	if ( id > m_recordCount ) return 0;

	if ( filedID < 0 ) return 0;	
	if ( filedID >= m_filedCount ) return 0;

	int index = filedID + m_filedCount*(id-1);
	return m_data[index];
}

int TTable::GetField( int id, const string& filedName )
{
	int filedID = GetFiledIndex(filedName);
	return GetField(id, filedID);
}

string TTable::GetFieldString( int id, int filedID )
{
	int data =  GetField(id, filedID);

	if ( data > 0 )
	{
		return m_tableGroup->m_strKey[data-1];	
	}	

	return "";
}

string TTable::GetFieldString( int id, const string& filedName )
{
	int data =  GetField(id, filedName);

	if ( data > 0 )
	{
		return m_tableGroup->m_strKey[data-1];	
	}	

	return "";
}

bool TTable::GetOneColumFileds( const string& attrName, map<int, int>& attrs )
{
	attrs.clear();

	int id = GetFiledIndex( attrName );
	if ( 0 > id) return false;

	int size = m_recordCount * m_filedCount;
	for (int i = 0; i < m_recordCount; i ++)
	{
		int offset = id + i * m_filedCount;
		attrs.insert(map<int, int>::value_type(m_data[offset], id));
	}

	if(attrs.size() < 0) return false;

	return true;
}

bool TTable::FindRecords( const string& filedName, int value, vector<int>& records )
{
	records.clear();

	int id = GetFiledIndex( filedName );

	if ( id < 0 ) return false;

	int size = m_recordCount * m_filedCount;
	for (int i=id; i<size; i+= m_filedCount )
	{
		if ( m_data[i] == value )
		{
			records.push_back(i/m_filedCount + 1);
		}
	} 
 
	if ( records.size() <= 0 ) return false;

	return true;
}

int TTable::GetFiledIndex( const string& filedName )
{
	int id = m_tableGroup->GetStringKey(filedName);

	if ( id > 0 )
	{
		for (int i=0; i<(int)m_fieldstrKey.size(); i++)
		{
			if ( m_fieldstrKey[i] == id )
			{
				return i;
			}
		} 
	}

	return -1;	
}

TTable* TTableGroup::GetTable(int index)
{
	return m_tables[index];
}

TTable* TTableGroup::GetTable(const string& tableName)
{
	for (size_t i=0; i<m_tables.size(); i++)
	{
		if ( m_tables[i]->m_name == tableName )
		{
			return m_tables[i];
		}
	}

	return NULL;
}

TTableGroup::TTableGroup()
{

}

TTableGroup::~TTableGroup()
{
	for (size_t i=0; i<m_tables.size(); i++)
	{
		delete m_tables[i];
	}
}

int TTableGroup::GetStringKey( const string& str )
{
	for (int i=0; i<(int)m_strKey.size(); i++)
	{
		if ( m_strKey[i] == str )
		{
			return (i+1);
		}
	}


	return -1;
}

void TTableGroup::Load(const string& name)
{
	m_fileName = name;

	string fileName = "Data/" + name + ".str";
	string dataName = "Data/" + name + ".bin";

	const vector<string>& stable = _GetLocalTextFile();

	ssize_t len;
	char* data = (char*)CCFileUtils::sharedFileUtils()->getFileData( fileName.c_str(), "rb", &len );
	unsigned char* p = ( unsigned char* )data;
	bool needLocalize = ( p[0] == ' ' ) && ( p[1] == ' ' ) && ( p[2] == ' ' );

	int pos = 3;
	int startPos = 3;
	size_t startIndex = m_strKey.size();
	while( pos < (int)len )
	{
		if ( data[pos] == '^' )
		{
			data[pos] = '\0';
			m_strKey.push_back(&data[startPos]);

			startPos = pos + 1;
		}
		pos++;
	}	
	if ( needLocalize && !stable.empty() ) {
		for ( size_t i = startIndex; i < m_strKey.size(); ++i ) {
			int index = atoi( m_strKey[ i ].c_str() );
			m_strKey[ i ] = stable[ index ];
		}
	}

	SAFE_DELETE_ARRAY( data );

	data = (char*)CCFileUtils::sharedFileUtils()->getFileData( dataName.c_str(), "rb", &len );

	int* endPtr  = (int*)(data + len);
	int* curPtr  = (int*)(data);
	int nRecordIndex = 0;
	while( curPtr < endPtr )
	{
		TTable* table = new TTable();
		m_tables.push_back(table);		

		table->m_name = m_strKey[*curPtr-1];
		curPtr++;
	
		table->m_filedCount = *curPtr;
		curPtr++;
		table->m_recordCount  = *curPtr;
		curPtr++;

		for (int i=0; i<table->m_filedCount; i++)
		{
			table->m_fieldstrKey.push_back(*curPtr);
			curPtr++;
		}

		table->m_tableGroup = this;

		int dataSize = table->m_filedCount*table->m_recordCount;

		table->m_data = new int[dataSize];

		memcpy( table->m_data, curPtr,  dataSize*4 );		

		curPtr += dataSize;

		++nRecordIndex;
	}	

	if( nRecordIndex >= 2 )
	{
		int n = 0;
	}

	SAFE_DELETE_ARRAY( data );	
}