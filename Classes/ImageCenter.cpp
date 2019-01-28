#include "ImageCenter.h"
//#include "CCFileUtils.h"
#include "base.h"
//#include "NetDef.h"
#include "ImageResource.h"

USING_NS_CC;

ImageCenter::ImageCenter()
{
	Initialize();
}

ImageCenter::~ImageCenter()
{
	UnInitialize();
}

void ImageCenter::Initialize()
{
}

void ImageCenter::UnInitialize()
{
	{
		ResourceMap::iterator it = mResourceMap.begin();
		ResourceMap::iterator end = mResourceMap.end();
		while( it != end )
		{
			SAFE_DELETE(it->second);
			++it;
		}
		mResourceMap.clear();
	}
	mRenderBatchIDMap.clear();
	mRenderBatchNameMap.clear();
}

ImageResource* ImageCenter::_LoadResource( const char * resName )
{
	std::string filePath = resName;
	ResourceType resType = eResourceType_png_pvr;
	if(filePath.find(".wen") != -1) {
		resType = eResourceType_jpg;
	}
	filePath = filePath.substr(0, filePath.find("."));
	std::string fileName = filePath.substr(filePath.find_last_of("/") + 1, filePath.size());
	ImageResource* retImageResource = NULL;
	ResourceMap::iterator it  = mResourceMap.find(fileName);
	if(it == mResourceMap.end())
	{
		ImageResource* pImageRes = new ImageResource(this);
		if(pImageRes->Load(filePath.c_str(), resType))
		{
			mResourceMap[fileName] = pImageRes;
			retImageResource = pImageRes;
		} else {
#if defined _WIN32 | WIN32
			log("%s can't found", resName);//找不到资源
#endif
			SAFE_DELETE(pImageRes);
		}
	} else {
		retImageResource = it->second;
		if (retImageResource->GetResourceState() == eResourceState_UnLoaded) {
			retImageResource->ReLoad(filePath.c_str(), resType);
		}
	}
	return retImageResource;
}

void ImageCenter::_UnLoadResource( const char * resName )
{
	std::string filePath = resName;
	filePath = filePath.substr(0, filePath.find("."));
	std::string fileName = filePath.substr(filePath.find_last_of("/") + 1, filePath.size());
	ResourceMap::iterator it  = mResourceMap.find(fileName);
	if(it != mResourceMap.end())
	{
		if (it->second->GetResourceState() == eResourceState_Loaded)
		{
			it->second->UnLoad();
		}
	}
}

static std::string _fixPath( const char* resName )
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

static std::string _fixPath2( const char* resName )
{	
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

ImageResource* ImageCenter::LoadLocalResource( const char* resName )
{
	return _LoadResource( _fixPath( resName ).c_str() );
}

void ImageCenter::UnLocalLoadResource( const char* resName )
{
	_UnLoadResource( _fixPath( resName ).c_str() );
}

ImageResource* ImageCenter::LoadLocalResource2( const char* resName )
{
	return _LoadResource( _fixPath2( resName ).c_str() );
}

void ImageCenter::UnLocalLoadResource2( const char* resName )
{
	_UnLoadResource( _fixPath2( resName ).c_str() );
}

ImageResource* ImageCenter::LoadResource( const char* resName )
{
	return _LoadResource( resName );
}

void ImageCenter::UnLoadResource( const char* resName )
{
	_UnLoadResource( resName );
}

void ImageCenter::UnLoadAllResource( )
{
	ResourceMap::iterator it  = mResourceMap.begin();
	ResourceMap::iterator end  = mResourceMap.end();
	if(it != end)
	{
		if (it->second->GetResourceState() == eResourceState_Loaded)
		{
			it->second->UnLoad();
		}
		++it;
	}
}

RenderBatchData* ImageCenter::GetRenderBatch( BatchID id )
{
	RenderBatchIDMap::iterator it  = mRenderBatchIDMap.find(id);
	if(it != mRenderBatchIDMap.end())
	{
		return it->second;
	}
	return NULL;
}

RenderBatchData* ImageCenter::GetRenderBatch( const char* name )
{
	std::string picName = name;
	picName = picName.substr( picName.find_last_of("/") + 1, picName.size() );
	picName = picName.substr( 0, picName.find(".") );
	RenderBatchNameMap::iterator it  = mRenderBatchNameMap.find(picName);
	if(it != mRenderBatchNameMap.end())
	{
		return it->second;
	}
	return NULL;
}

BatchID ImageCenter::GenerateNewBatchID()
{
	static BatchID sID = 0;
	return sID++;
}

void ImageCenter::AddRenderBatch( RenderBatchData* pBatch )
{
	if(pBatch)
	{
		mRenderBatchIDMap[pBatch->_BatchID] = pBatch;
		mRenderBatchNameMap[pBatch->_TextureName] = pBatch;
	}
}

void ImageCenter::RemoveRenderBatch( RenderBatchData* pBatch )
{
	{
		RenderBatchIDMap::iterator it  = mRenderBatchIDMap.find(pBatch->_BatchID);
		if(it != mRenderBatchIDMap.end())
		{
			mRenderBatchIDMap.erase(it);
		}
	}
	{
		RenderBatchNameMap::iterator it  = mRenderBatchNameMap.find(pBatch->_TextureName);
		if(it != mRenderBatchNameMap.end())
		{
			mRenderBatchNameMap.erase(it);
		}
	}
}


