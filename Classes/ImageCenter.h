#ifndef _ImageCenter_H__
#define _ImageCenter_H__

#include <cocos2d.h>
#include "Singleton.h"
//#include "CCSprite.h"
//#include "CCTexture2D.h"
#include "ImageResource.h"

class UI;
class ImageCenter : public Singleton<ImageCenter ,Tag_Singleton_Auto>
{
protected:
	friend class Singleton<ImageCenter, Tag_Singleton_Auto>;
	typedef std::map<std::string, ImageResource*> ResourceMap;
	typedef std::map<BatchID, RenderBatchData*> RenderBatchIDMap;
	typedef std::map<std::string, RenderBatchData*> RenderBatchNameMap;
	ImageCenter();
	virtual ~ImageCenter();
public:
	void Initialize( );

	void UnInitialize( );

	// load / unload files locate ImageCenter
	ImageResource* LoadLocalResource( const char* resName );
	void UnLocalLoadResource( const char* resName );

	// load / unload files locate ImageCenter
	ImageResource* LoadLocalResource2( const char* resName );
	void UnLocalLoadResource2( const char* resName );

	ImageResource* LoadResource( const char* resName );
	void UnLoadResource( const char* resName );

	void UnLoadAllResource( );

	RenderBatchData* GetRenderBatch( BatchID id );

	RenderBatchData* GetRenderBatch( const char* name );

	BatchID GenerateNewBatchID( );

	void AddRenderBatch( RenderBatchData* pBatch );

	void RemoveRenderBatch( RenderBatchData* pBatch );
private:
	ImageResource* _LoadResource( const char* resName );
	void _UnLoadResource( const char* resName );
protected:
	RenderBatchIDMap mRenderBatchIDMap;
	RenderBatchNameMap mRenderBatchNameMap;
	ResourceMap mResourceMap;
};

#endif
