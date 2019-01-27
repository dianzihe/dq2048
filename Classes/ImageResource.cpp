#include "ImageResource.h"
//#include "CCFileUtils.h"
#include "base.h"
//#include "NetDef.h"
#include "ImageCenter.h"

USING_NS_CC;

ImageResource::ImageResource(ImageCenter* pImageCenter)
: mHostImageCenter(pImageCenter)
, mResourceState(eResourceState_UnInitialize)
, mImageSizeX(0)
, mImageSizeY(0)
, mSquareSize(0)
, mReference(0)
{
}

ImageResource::~ImageResource()
{
	{
		RenderBatchMap::iterator it = mRenderBatchMap.begin();
		RenderBatchMap::iterator end = mRenderBatchMap.end();
		while( it != end )
		{
			mHostImageCenter->RemoveRenderBatch(*it);
			SAFE_DELETE(*it);
			++it;
		}
		mRenderBatchMap.clear();
	}

	{
		TextureContainer::iterator it = mTextureContainer.begin();
		TextureContainer::iterator end = mTextureContainer.end();
		while( it != end )
		{
			//CCTextureCache::sharedTextureCache()->removeTexture(*it);
			CC_SAFE_RELEASE(*it);
			++it;
		}
		mTextureContainer.clear();
	}
}

bool ImageResource::LoadBui( const char* filename )
{
	ssize_t lenth;
	unsigned char* buffer = FileUtils::getInstance()->getFileData( filename, "rb", &lenth);
	char* data = (char*)buffer;
	if(lenth <= 0 || data == NULL)
		return false;
	int texLen;
	char texName[256];
	Readint( data, texLen );
	strcpy(texName, data);
	data += texLen;

	Readint( data, mImageSizeX );
	Readint( data, mImageSizeY );
	Readint( data, mSquareSize );

	int frameSize;
	Readint( data, frameSize );
	assert(frameSize > 0);
	struct UsedData
	{
		short _ConvertU;
		short _ConvertV;
		short _OffsetX;
		short _OffsetY;
		short _SourceU;
		short _SourceV;
		short _SourceSizeX;
		short _SourceSizeY;
		short _OriginSizeX;
		short _OriginSizeY;
	};
	for (int i = 0; i < frameSize; ++i)
	{
		RenderBatchData* newBatch = new RenderBatchData;
		int picLen;
		char picName[256];
		Readint( data, picLen );
		strcpy(picName, data);
		data += picLen;
		UsedData _UsedData = *((UsedData*)(data));
		data += sizeof(UsedData);

		newBatch->_TextureName = picName;
		newBatch->_TextureName = newBatch->_TextureName.substr( 0, newBatch->_TextureName.find(".") );
		int startpos = newBatch->_TextureName.find_last_of("/");
		if( startpos!= -1)
			newBatch->_TextureName = newBatch->_TextureName.substr(startpos + 1, newBatch->_TextureName.size());

		newBatch->_HostImageResource = this;
		newBatch->_BatchID = mHostImageCenter->GenerateNewBatchID();
		newBatch->_ConvertU = _UsedData._ConvertU;
		newBatch->_ConvertV = _UsedData._ConvertV;
		newBatch->_SourceU = _UsedData._SourceU;
		newBatch->_SourceV = _UsedData._SourceV;
		newBatch->_SourceSizeX = _UsedData._SourceSizeX;
		newBatch->_SourceSizeY = _UsedData._SourceSizeY;
		newBatch->_OriginSizeX = _UsedData._OriginSizeX;
		newBatch->_OriginSizeY = _UsedData._OriginSizeY;
		mRenderBatchMap.push_back(newBatch);
		mHostImageCenter->AddRenderBatch(newBatch);
	}
	SAFE_DELETE_ARRAY( buffer );
	return true;
}

bool ImageResource::Load( const char * resName, ResourceType resType )
{
	if(mResourceState != eResourceState_UnInitialize)
		return false;

	char filename[256];
	sprintf(filename, "%s.bui", resName);
	bool isLoadBui = LoadBui(filename);

	// Load Texture
    // Reading Full Texture First
	if(resType == eResourceType_png_pvr)
	{
#if defined _WIN32 || defined WIN32
		sprintf(filename, "%s.png" ,resName );
#else
		sprintf(filename, "%s.pvr.ccz" ,resName );
#endif
	}
	else if(resType == eResourceType_jpg)
	{
		sprintf(filename, "%s.wen" ,resName );
	}
    CCTexture2D *newTex = CCTextureCache::sharedTextureCache()->addImage(filename);
    if(newTex == NULL)
    {
        
        int texIdx = 0;
        while(1)
        {
			if(resType == eResourceType_png_pvr)
			{
#if defined _WIN32 | WIN32
				sprintf(filename, "%s_%d.png", resName, texIdx++ );
#else
				sprintf(filename, "%s_%d.pvr.ccz", resName, texIdx++ );
#endif
			}
			else if(resType == eResourceType_jpg)
			{
				sprintf(filename, "%s_%d.wen", resName, texIdx++ );
			}
            //CCTexture2D::setUSED_ANTI_ALIAS( false );
            CCTexture2D* newTex = CCTextureCache::sharedTextureCache()->addImage(filename);
            //CCTexture2D::setUSED_ANTI_ALIAS( true );
            if(newTex)
            {
                mTextureContainer.push_back(newTex);
            }
            else
            {
                break;
            }
        }
    }
    
    if( newTex == NULL && mTextureContainer.empty() )
        return false;
    
    if(isLoadBui)
    {
		int n = 0;
        RenderBatchMap::iterator it = mRenderBatchMap.begin();
        RenderBatchMap::iterator end = mRenderBatchMap.end();
        while (it != end)
        {
            RenderBatchData* pRenderBatchData = *it;
            if(pRenderBatchData)
            {
				if(newTex)
				{
					pRenderBatchData->_FullTexture = newTex;
					if (n != 0)
						pRenderBatchData->_FullTexture->retain();
				}
				else
					pRenderBatchData->_HostImageResource = this;
            }
            ++it;
			++n;
        }
    }
    else
    {
        assert(mRenderBatchMap.empty());//应该没有元素
        RenderBatchData* pImageRes = new RenderBatchData;
        pImageRes->_BatchID = mHostImageCenter->GenerateNewBatchID();
        pImageRes->_ConvertU = 0;
        pImageRes->_ConvertV = 0;
        pImageRes->_FullTexture = newTex;
        pImageRes->_HostImageResource = NULL;
        pImageRes->_SourceU = 0;
        pImageRes->_SourceV = 0;
        pImageRes->_SourceSizeX = (short)newTex->getContentSize().width;
        pImageRes->_SourceSizeY = (short)newTex->getContentSize().height;
        pImageRes->_OriginSizeX = (short)newTex->getContentSize().width;
        pImageRes->_OriginSizeY = (short)newTex->getContentSize().height;
        pImageRes->_TextureName = filename;
        pImageRes->_TextureName = pImageRes->_TextureName.substr( pImageRes->_TextureName.find_last_of("/") + 1, pImageRes->_TextureName.size() );
        pImageRes->_TextureName = pImageRes->_TextureName.substr( 0, pImageRes->_TextureName.find(".") );
        mHostImageCenter->AddRenderBatch(pImageRes);
        mRenderBatchMap.push_back(pImageRes);
	}
    
	mResourceState = eResourceState_Loaded;
	return true;
}

bool ImageResource::UnLoad()
{
	{
		RenderBatchMap::iterator it = mRenderBatchMap.begin();
		RenderBatchMap::iterator end = mRenderBatchMap.end();
		while( it != end )
		{
			RenderBatchData* pRenderBatch = *it;
			pRenderBatch->_HostImageResource = NULL;
			//CCTextureCache::sharedTextureCache()->removeTexture(pRenderBatch->_FullTexture);
			//pRenderBatch->_FullTexture = NULL;
			CC_SAFE_RELEASE_NULL(pRenderBatch->_FullTexture);

			++it;
		}
	}

	{
		TextureContainer::iterator it = mTextureContainer.begin();
		TextureContainer::iterator end = mTextureContainer.end();
		while( it != end )
		{
			//CCTextureCache::sharedTextureCache()->removeTexture(*it);
			CC_SAFE_RELEASE(*it);
			++it;
		}
		mTextureContainer.clear();
	}
	mResourceState = eResourceState_UnLoaded;
	return true;
}

bool ImageResource::ReLoad(const char * resName, ResourceType resType)
{
	if(mResourceState != eResourceState_UnLoaded)
		return false;

	char filename[256];
	if(resType == eResourceType_png_pvr)
	{
#if defined _WIN32 || defined WIN32
		sprintf(filename, "%s.png" ,resName );
#else
		sprintf(filename, "%s.pvr.ccz" ,resName );
#endif
	}
	else if(resType == eResourceType_jpg)
	{
		sprintf(filename, "%s.wen" ,resName );
	}
	CCTexture2D *newTex = CCTextureCache::sharedTextureCache()->addImage(filename);
	int n = 0;
	if ( newTex )
	{
		RenderBatchMap::iterator it = mRenderBatchMap.begin();
		RenderBatchMap::iterator end = mRenderBatchMap.end();
		while (it != end)
		{
			RenderBatchData* pRenderBatchData = *it;
			if(pRenderBatchData)
			{
				if(newTex)
				{
					pRenderBatchData->_FullTexture = newTex;
					if (n != 0)
						pRenderBatchData->_FullTexture->retain();
				}
				else
					pRenderBatchData->_HostImageResource = this;
			}
			++it;
			++n;
		}
	}
	else
	{
		int texIdx = 0;
		while(1)
		{
			if(resType == eResourceType_png_pvr)
			{
#if defined _WIN32 | WIN32 
				sprintf(filename, "%s_%d.png", resName, texIdx++ );
#else
				sprintf(filename, "%s_%d.pvr.ccz", resName, texIdx++ );
#endif
			}
			else if(resType == eResourceType_jpg)
			{
				sprintf(filename, "%s_%d.wen", resName, texIdx++ );
			}
			//CCTexture2D::setUSED_ANTI_ALIAS( false );
			CCTexture2D* newTex = CCTextureCache::sharedTextureCache()->addImage(filename);
			//CCTexture2D::setUSED_ANTI_ALIAS( true );
			if(newTex)
			{
				mTextureContainer.push_back(newTex);
			}
			else
			{
				break;
			}
		}

		if(!mTextureContainer.empty())
		{
			RenderBatchMap::iterator it = mRenderBatchMap.begin();
			RenderBatchMap::iterator end = mRenderBatchMap.end();
			while( it != end )
			{
				RenderBatchData* pRenderBatch = *it;
				pRenderBatch->_HostImageResource = this;
				pRenderBatch->_FullTexture = NULL;
				++it;
			}
		}
	}

	if( newTex != NULL || !mTextureContainer.empty() )
		mResourceState = eResourceState_Loaded;
	return true;
}