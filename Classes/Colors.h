#pragma once

namespace Colors {
    static cocos2d::Color3B calcColor3B(int color) { // color like 0xFF00CC
        float blue = color & 0xFF;
        float green = (color >> 8) & 0xFF;
        float red = (color >> 16) & 0xFF;
        return cocos2d::Color3B(red, green, blue);
    }
    
    static cocos2d::Color4F calcColor4F(int color) { // color like 0xFF00CC
        return cocos2d::Color4F(calcColor3B(color));
    }
    
    static cocos2d::Color4B calcColor4B(int color) { // color like 0xFF00CC
        return cocos2d::Color4B(calcColor3B(color));
    }
    
    const auto ScoreLableText   = calcColor4B(0xe4dace);
    const auto MainBack         = calcColor4B(0xfaf8ee);
    const auto Back1            = calcColor4F(0xbaada0);
    const auto Back2            = calcColor4F(0x8f7a66);
    const auto ButtonText       = calcColor4B(0xf9f6f2);
    const auto CellBack         = calcColor4F(0xccc1b4);
    const auto CellText1        = calcColor4B(0xf9f6f2);
    const auto CellText2        = calcColor4B(0x776e65);
    
    const std::unordered_map<int, cocos2d::Color4F> Cells = {
        {2,     calcColor4F(0xeee4da)},
        {4,     calcColor4F(0xede0c8)},
        {8,     calcColor4F(0xf2b179)},
        {16,    calcColor4F(0xf59563)},
        {32,    calcColor4F(0xf67c5f)},
        {64,    calcColor4F(0xf65e3b)},
        {128,   calcColor4F(0xedcf72)},
        {256,   calcColor4F(0xedcc61)},
        {512,   calcColor4F(0xedc850)},
        {1024,  calcColor4F(0xedc53f)},
        {2048,  calcColor4F(0xedc22e)}
    };
    
}
