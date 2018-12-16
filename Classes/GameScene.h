#pragma once

#include <string>
#include "cocos2d.h"
#include "Gestures/SwipeGestureRecognizer.h"
#include "Field.hpp"

NS_GAME_BEGIN
class GameScene : public cocos2d::Scene
{
public:
    static cocos2d::Scene* createScene();
    
    CREATE_FUNC(GameScene);
    
private:
    virtual bool init();
    
    void initGui();
    void addGestureRecognizers();
    void onSwipe(cocos2d::SwipeGestureRecognizer* recognizer);
    void onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event);
    
    Field _field;
    FieldGUI* _fieldGUI = nullptr;
};
NS_GAME_END

