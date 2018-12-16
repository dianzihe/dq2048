#include "cocos2d.h"
#include "Constants.h"
#include "RoundedRect.hpp"

USING_NS_CC;

NS_GAME_BEGIN
bool RoundedRect::init() {
    if (!DrawNode::init()){
        return false;
    }
    return true;
}

void RoundedRect::setSizeAndColor(const Vec2& size, const Color4F& color) {
    this->clear();
    drawSolidRoundedRect(Vec2::ZERO, size, 5.0f, 20, color);
}

#if false
void RoundedRect::drawSolidRoundedRect(const Vec2 &origin, const Vec2 &destination, float radius,
                                       unsigned int segments, const Color4F &color)
{
    drawSolidRect(origin, destination, color); // пока просто SolidRect
}
#else
// take from https://gist.github.com/dontangg/a7c3acec4bf05e61dd58
void RoundedRect::drawSolidRoundedRect(const Vec2 &origin, const Vec2 &destination, float radius,
                                       unsigned int segments, const Color4F &color)
{
    const float coef = 2.0f * (float)M_PI/(segments - 8);

    Vec2 *vertices = new (std::nothrow) Vec2[segments + 1];
    if (!vertices)
        return;

    // Draw right edge
    vertices[0].x = destination.x;
    vertices[0].y = origin.y + radius;
    vertices[1].x = destination.x;
    vertices[1].y = destination.y - radius;

    unsigned int quadrant = 1;
    unsigned int radsI = 0;
    for(unsigned int i = 2; i <= segments; i++) {
        float rads = radsI*coef;
        GLfloat j = radius * cosf(rads);
        GLfloat k = radius * sinf(rads);

        if (rads < M_PI_2 || rads > M_PI + M_PI_2) {
            if (quadrant == 3) {
                // Draw bottom edge
                vertices[i].x = origin.x + radius;
                vertices[i].y = origin.y;
                ++i;
                vertices[i].x = destination.x - radius;
                vertices[i].y = origin.y;

                ++quadrant;
                continue;
            }
            j += destination.x - radius;
        } else {
            if (quadrant == 1) {
                // Draw top edge
                vertices[i].x = destination.x - radius;
                vertices[i].y = destination.y;
                ++i;
                vertices[i].x = origin.x + radius;
                vertices[i].y = destination.y;

                quadrant++;
                continue;
            }
            j += origin.x + radius;
        }

        if (rads < M_PI) {
            k += destination.y - radius;
        } else {
            if (quadrant == 2) {
                // Draw left edge
                vertices[i].x = origin.x;
                vertices[i].y = destination.y - radius;
                ++i;
                vertices[i].x = origin.x;
                vertices[i].y = origin.y + radius;

                ++quadrant;
                continue;
            }
            k += origin.y + radius;
        }

        vertices[i].x = j;
        vertices[i].y = k;

        ++radsI;
    }

    drawSolidPoly(vertices, segments, color);

    CC_SAFE_DELETE_ARRAY(vertices);
}
#endif

NS_GAME_END
