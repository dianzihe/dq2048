#pragma once

#include "Constants.h"
#include "cocos2d.h"
#include "RoundedRect.hpp"

USING_NS_CC;

NS_GAME_BEGIN
class Cell: public DrawNode {
    
public:
    void show(int val);
    void up(int newVal);
    void win();
    
    void setHistory(std::string& mes);
    std::string& getHistory();
    
    int val();
    
    CREATE_FUNC(Cell);
    
private:
    virtual bool init();
    
    float getFontSize();
    const Color4F getFillColor();
    Color4B getTextColor();
    
    Label* _lblVal = nullptr;
    RoundedRect* _back = nullptr;
    
    int _val = 0;
    
    std::string _hist;
    
    static constexpr int WIDTH = Consts::CELL_WIDTH - Consts::CELL_OFFSET;
    static constexpr int HEIGHT = Consts::CELL_HEIGHT - Consts::CELL_OFFSET;
};
NS_GAME_END
