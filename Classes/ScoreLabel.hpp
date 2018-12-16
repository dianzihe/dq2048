#pragma once

#include "Constants.h"
#include "cocos2d.h"

NS_GAME_BEGIN

class ScoreLabel : public cocos2d::DrawNode {
public:
    void setTitle(const std::string& title);
    void setScore(int newVal);
            
    CREATE_FUNC(ScoreLabel);
private:
    virtual bool init();
    
    cocos2d::Label* _lblTitle = nullptr;
    cocos2d::Label* _lblScore = nullptr;
};

NS_GAME_END
