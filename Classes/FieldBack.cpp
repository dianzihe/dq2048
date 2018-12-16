#include "FieldBack.hpp"
#include "cocos2d.h"
#include "Constants.h"
#include "RoundedRect.hpp"
#include "Colors.h"

USING_NS_CC;

NS_GAME_BEGIN
bool FieldBack::init() {
    if (!RoundedRect::init()){
        return false;
    }
    
    // back
    auto size = Vec2(Consts::FIELD_WIDTH + Consts::CELL_OFFSET, Consts::FIELD_HEIGHT + Consts::CELL_OFFSET);
    drawSolidRoundedRect(Vec2::ZERO, size, 5.0f, 16, Colors::Back1);
    
    //tiles
    int w = Consts::CELL_WIDTH - Consts::CELL_OFFSET;
    int h = Consts::CELL_HEIGHT - Consts::CELL_OFFSET;
    for (int r = 0; r < Consts::ROWS; ++r)
        for (int c = 0; c < Consts::COLS; ++c) {
            auto pos = Vec2(Consts::CELL_OFFSET + c * Consts::CELL_WIDTH,
                            Consts::CELL_OFFSET + r * Consts::CELL_HEIGHT);
            drawSolidRoundedRect(pos, Vec2(pos.x + w, pos.y + h), 5.0f, 16, Colors::CellBack);
        }
    return true;
}
NS_GAME_END
