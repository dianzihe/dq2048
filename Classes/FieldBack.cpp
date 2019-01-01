#include "FieldBack.hpp"
#include "cocos2d.h"
#include "Constants.h"
#include "RoundedRect.hpp"
#include "Colors.h"

USING_NS_CC;

NS_GAME_BEGIN
bool FieldBack::init(Vec2 size, int rows, int col, float width, Color4F color)  {
    if (!RoundedRect::init()){
        return false;
    }
	Size tmpSize = Size(rows * (width + Consts::CELL_OFFSET) + Consts::CELL_OFFSET,
		col * (width + Consts::CELL_OFFSET) + Consts::CELL_OFFSET);
	drawSolidRoundedRect(Vec2::ZERO, tmpSize, 5.0f, 16, color);

	int w = width;
	int h = width;
	for (int r = 0; r < rows; ++r)
		for (int c = 0; c < col; ++c) {
			auto pos = Vec2(Consts::CELL_OFFSET + r * (width + Consts::CELL_OFFSET), Consts::CELL_OFFSET + c * (width + Consts::CELL_OFFSET));
			drawSolidRoundedRect(pos, Vec2(pos.x + w, pos.y + h), 5.0f, 16, Colors::CellBack);
		}

    return true;
}
NS_GAME_END
