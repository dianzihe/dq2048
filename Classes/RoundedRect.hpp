#pragma once

#include "cocos2d.h"

USING_NS_CC;

NS_GAME_BEGIN
class RoundedRect: public DrawNode {

public:
    void setSizeAndColor(const Vec2& size, const Color4F& color);
    CREATE_FUNC(RoundedRect);
    virtual bool init();

protected:
    void drawSolidRoundedRect(const Vec2 &origin, const Vec2 &destination,
                              float radius, unsigned int segments, const Color4F &color);
};
NS_GAME_END
