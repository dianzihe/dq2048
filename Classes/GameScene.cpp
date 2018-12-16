#include <string>
#include "GameScene.h"
#include "SimpleAudioEngine.h"
#include "Gestures/GestureRecognizerUtils.h"
#include "Gestures/SwipeGestureRecognizer.h"
#include "Colors.h"

USING_NS_CC;

NS_GAME_BEGIN
Scene* GameScene::createScene() {
    return GameScene::create();
}


bool GameScene::init() {
    if (!Scene::init()) return false;

    _fieldGUI = FieldGUI::create();
    _fieldGUI->setField(&_field);
    _field.init(_fieldGUI);
    
    initGui();
    addGestureRecognizers();

    return true;
}

void GameScene::initGui() {
    auto background = LayerColor::create(Colors::MainBack);
    this->addChild(background, -1);
    
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    auto centerX = origin.x + visibleSize.width /2;
    auto centerY = origin.y + visibleSize.height/2;
    
    _fieldGUI->setPosition(Vec2(centerX, centerY));
    this->addChild(_fieldGUI, 10);
}

void GameScene::addGestureRecognizers() {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    log("init for ios/android");
    auto swipe = SwipeGestureRecognizer::create();
    swipe->onSwipe = CC_CALLBACK_1(GameScene::onSwipe, this);
    addChild(swipe);
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_MAC)
    log("init for mac");
    auto listener = EventListenerKeyboard::create();
    listener->onKeyPressed = CC_CALLBACK_2(GameScene::onKeyPressed, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
#else
    //unsupported os!
#endif
}

void GameScene::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event) {
    switch (keyCode) {
        case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
            _field.onSwipe(Consts::Move::Left);
            break;
            
        case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
            _field.onSwipe(Consts::Move::Right);
            break;
            
        case EventKeyboard::KeyCode::KEY_UP_ARROW:
            _field.onSwipe(Consts::Move::Up);
            break;
            
        case EventKeyboard::KeyCode::KEY_DOWN_ARROW:
            _field.onSwipe(Consts::Move::Down);
            break;
            
        case EventKeyboard::KeyCode::KEY_1:
            _fieldGUI->debug1();
            break;
            
        case EventKeyboard::KeyCode::KEY_2:
            _fieldGUI->debug2();
            break;
            
        default:
            break;
    }
}

void GameScene::onSwipe(SwipeGestureRecognizer* recognizer) {
    auto stato = recognizer->getStatus();
    if (stato != GestureStatus::RECOGNIZED) return;
    
    auto direction = recognizer->getSwipeDirection();
    switch (direction) {
        case SwipeGestureRecognizer::SwipeStatus::SWIPE_UP:
            _field.onSwipe(Consts::Move::Up);
            break;
            
        case SwipeGestureRecognizer::SwipeStatus::SWIPE_RIGHT:
            _field.onSwipe(Consts::Move::Right);
            break;
            
        case SwipeGestureRecognizer::SwipeStatus::SWIPE_DOWN:
            _field.onSwipe(Consts::Move::Down);
            break;
            
        case SwipeGestureRecognizer::SwipeStatus::SWIPE_LEFT:
            _field.onSwipe(Consts::Move::Left);
            break;
            
        default:
            break;
    }
}

NS_GAME_END
