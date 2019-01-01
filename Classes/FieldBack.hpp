#pragma once

#include "Constants.h"
#include "RoundedRect.hpp"

NS_GAME_BEGIN
using namespace std;

class FieldBack;
typedef shared_ptr<FieldBack> FieldBackPtr;

class FieldBack : public RoundedRect {
public:
	Vec2 _cellSize;
	virtual bool init(Vec2 size, int rows, int col, float width, Color4F color);
    //CREATE_FUNC(FieldBack)

	static FieldBackPtr make(Vec2 size, int rows, int col, float width, Color4F color)
	{
		FieldBackPtr ret(new FieldBack);
		if (ret && !ret->init(size, rows, col, width, color))
			ret.reset();
		return ret;
	}

	static FieldBack * make(Vec2 size, int rows, int col, float width, Color4F color, int)
	{
		FieldBack * ret = new FieldBack();
		if (ret != NULL && ret->init(size, rows, col, width, color)){
			ret->autorelease();
			return ret;
		}
		CC_SAFE_DELETE(ret);
		return NULL;

	}

};
NS_GAME_END
