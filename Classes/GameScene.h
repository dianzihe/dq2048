#pragma once

#include <string>
#include "cocos2d.h"
#include "Gestures/SwipeGestureRecognizer.h"
#include "Field.hpp"

NS_GAME_BEGIN
class GameScene : public cocos2d::Scene
{
public:
	Sprite* background;
	Size winSize;
	Node*			m_uiNode;
    static cocos2d::Scene* createScene();
	static GameScene* GetScene();
	void loadPersistentTextureCache();
	void loadTextureCache();
	void initBG();
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

