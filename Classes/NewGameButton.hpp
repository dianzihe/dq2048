#pragma once

#include "Constants.h"
#include "cocos2d.h"

NS_GAME_BEGIN

class NewGameButton : public cocos2d::DrawNode {

public:
    CREATE_FUNC(NewGameButton);
    
private:
    virtual bool init();
};

NS_GAME_END
