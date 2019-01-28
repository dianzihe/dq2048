#ifndef _IMAGERESOURCE_H__
#define _IMAGERESOURCE_H__

#include <vector>
#include <cocos2d.h>
#include "Singleton.h"

typedef int				BatchID;

#define InvalidBatchID	-1

class ImageResource;
struct RenderBatchData
{
	std::string			  _TextureName;
	cocos2d::Texture2D*	  _FullTexture;
	BatchID				  _BatchID;
	short				  _ConvertU;
	short				  _ConvertV;
	short				  _SourceU;
	short				  _SourceV;
	short				  _SourceSizeX;
	short				  _SourceSizeY;
	short				  _OriginSizeX;
	short				  _OriginSizeY;
	ImageResource*		  _HostImageResource;

	RenderBatchData()
	: _FullTexture(NULL)
	, _HostImageResource(NULL)
	, _BatchID(0)
	, _ConvertU(0)
	, _ConvertV(0)
	, _SourceU(0)
	, _SourceV(0)
	, _SourceSizeX(0)
	, _SourceSizeY(0)
	, _OriginSizeX(0)
	, _OriginSizeY(0)
	{

	}

	RenderBatchData& operator=(const RenderBatchData& rhs)
	{
		if (this == &rhs)
			return *this;

		this->_TextureName = rhs._TextureName;;
		this->_FullTexture = rhs._FullTexture;
		CC_SAFE_RETAIN(this->_FullTexture);
		this->_BatchID = rhs._BatchID;
		this->_ConvertU = rhs._ConvertU;
		this->_ConvertV = rhs._ConvertV;
		this->_SourceU = rhs._SourceU;
		this->_SourceV = rhs._SourceV;
		this->_SourceSizeX = rhs._SourceSizeX;
		this->_SourceSizeY = rhs._SourceSizeY;
		this->_OriginSizeX = rhs._OriginSizeX;
		this->_OriginSizeY = rhs._OriginSizeY;
		this->_HostImageResource = rhs._HostImageResource;
		return *this;
	}

	~RenderBatchData()
	{
		CC_SAFE_RELEASE(_FullTexture);
	}
};

class ImageCenter;
enum ResourceState
{
	eResourceState_UnInitialize,
	eResourceState_Loaded,
	eResourceState_UnLoaded,
};

enum ResourceType
{
	eResourceType_png_pvr,
	eResourceType_jpg,
};

class ImageResource
{
public:
	typedef std::vector<RenderBatchData*> RenderBatchMap;
	typedef std::vector<cocos2d::Texture2D*> TextureContainer;
	ImageResource(ImageCenter* pImageCenter);
	virtual ~ImageResource();
public:
	bool Load( const char * resName, ResourceType resType = eResourceType_png_pvr );

	bool UnLoad();

	bool ReLoad( const char * resName, ResourceType resType = eResourceType_png_pvr );

	inline int GetSquareSize() { return mSquareSize; }
	inline int GetImageSizeX() { return mImageSizeX; }
	inline int GetImageSizeY() { return mImageSizeY; }

	TextureContainer& GetTextureContainer() { return mTextureContainer; }

	ResourceState GetResourceState() { return mResourceState; }

	void Retain() { ++mReference; }
	void Release() { --mReference; if( mReference <= 0 ) UnLoad(); }

protected:
	bool LoadBui( const char* filename );

protected:
	RenderBatchMap mRenderBatchMap;
	TextureContainer mTextureContainer;
	int mImageSizeX;
	int mImageSizeY;
	int mSquareSize;
	int mReference; //use in icon resource
	ImageCenter* mHostImageCenter;
	ResourceState mResourceState;
};

#endif
