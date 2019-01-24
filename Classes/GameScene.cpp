#include <string>
#include "GameScene.h"
#include "SimpleAudioEngine.h"
//#include "Gestures/GestureRecognizerUtils.h"
//#include "Gestures/SwipeGestureRecognizer.h"
#include "Colors.h"
#include "Board.h"
#include "GemUtils.h"

USING_NS_CC;

NS_GAME_BEGIN
Scene* GameScene::createScene() {
    return GameScene::create();
}


bool GameScene::init() {
    if (!Scene::init()) return false;

	/*
	auto sn = ShaderNode::shaderNodeWithVertex("", "efx_stream.fsh");
	auto s = Director::getInstance()->getWinSize();
	sn->setPosition(Vec2(s.width / 2, s.height / 2));
	addChild(sn);
	*/

	loadPersistentTextureCache();
	loadTextureCache();

	winSize = Director::getInstance()->getVisibleSize();

	initBG();


    //_fieldGUI = FieldGUI::create();
    //_fieldGUI->setField(&_field);
    //_field.init(_fieldGUI);
    
    initGui();
    //addGestureRecognizers();

    return true;
}

void GameScene::initBG()
{
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("battleMisc.plist");

	background = PH::GemUtils::GetSprite("gameui/background.jpg");
	background->setPosition(winSize.width / 2, winSize.height / 2);
	this->addChild(background);

	//flowersDrop = FlowerDrop::create();
	//flowersDrop->setPosition(ccp(-50, winSize.height + 50));
	//this->addChild(flowersDrop);

	//    CocosDenshion::SimpleAudioEngine::sharedEngine()->stopBackgroundMusic();
	//playBG("sound/bg/ui.mp3");

}

void GameScene::loadPersistentTextureCache()
{
	log("load persistent texture\n");
	for (int i = 0; i < 100; i++)
	{
		stringstream s;
		s << "icons" << i << ".plist";

		//log("load plist %s\n", s.str());
		if (PH::resourcePresentForLoading(s.str()))
			SpriteFrameCache::getInstance()->addSpriteFramesWithFile(s.str());
		else
			break;
	}

	log("load plist alert.plist\n");
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("alert.plist");
	log("load plist gems.plist\n");
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("gems.plist");
	log("load plist tutorialMisc.plist\n");
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("tutorialMisc.plist");
	//CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadBackgroundMusic("sound/bg/ui.mp3");
	log("done loading persistent texture\n");
}

void GameScene::loadTextureCache()
{
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("gameuiBase.plist");
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("gameui.plist");
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("gameui1.plist");
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("gameui2.plist");
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("gameui3.plist");
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("gameuiQiuAnim.plist");
}


void GameScene::initGui() {
    //auto background = LayerColor::create(Colors::MainBack);
    //this->addChild(background, -1);
    
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    auto centerX = origin.x + visibleSize.width /2;
    auto centerY = origin.y + visibleSize.height/2;
    
	PH::LevelInfo level;
	Vec2 layerPosition;
	PH::BoardLayer* layer = PH::BoardLayer::create(BOARD_WIDTH, BOARD_HEIGHT, level);
	//caculate the layer position
	//ȡ�м�ֵ
	/*ȥ�����߱߿򣬱߿�����Сֵ20�������ֵ*/
	/*����ֲ����߿� + BOARD_WIDTH * gem's width + (BOARD_WIDTH - 1) * gap + �߿�*/
	/*�߿�����ֵΪ10�� gem�����ֵΪ106*/
	/*����ֲ���*/
	/*TOP:20% ����ֵΪ500*/
	/*CON:70% ����ֵΪ*/
	/*BOT:10% */

	/*ȷ����Ϸ������Ҫ��ȷ���ڿɱ���Ļ�ߴ��µ���Ϸ�ߴ�
	��Сֵ�����ֵ
	*/
	
	float layerWidth = BOARD_FRAME * 2 + BOARD_WIDTH * 106 + (BOARD_WIDTH - 1) * 10;
	float layerHeight = BOARD_FRAME * 2 + BOARD_HEIGHT * 106 + (BOARD_HEIGHT - 1) * 10;
	if (visibleSize.width > layerWidth){
		layerPosition.x = visibleSize.width / 2 - layerWidth / 2;
	} else{
		layerPosition.x = visibleSize.width / 2 - layerWidth / 2;
	}

	if (visibleSize.width > layerWidth){
		layerPosition.y = visibleSize.height * (0.1 + 0.35) - layerHeight / 2;
	}
	else{
		layerPosition.y = visibleSize.height * (0.1 + 0.35) - layerHeight / 2;
	}
	float midX = visibleSize.width - 100 * 2;
	
	layer->setPosition(layerPosition);
	layer->setContentSize(Size(layerWidth, layerHeight));
	this->addChild(layer);

	//test create top 
	auto topLayer = LayerColor::create(Color4B(0xff, 0x00, 0x00, 0x80), layerWidth, 200);
	//topLayer->ignoreAnchorPointForPosition(false);
	topLayer->setPosition(visibleSize.width / 2 - layerWidth / 2,
		visibleSize.height * (0.1 + 0.7) + BOARD_FRAME);
	this->addChild(topLayer, 1, topLayer->getTag());

	//test create bottom 
	auto bottomLayer = LayerColor::create(Color4B(0xff, 0x00, 0x00, 0x80), layerWidth, 10);
	//topLayer->ignoreAnchorPointForPosition(false);
	bottomLayer->setPosition(visibleSize.width / 2 - layerWidth / 2, BOARD_FRAME);
	this->addChild(bottomLayer, 1, bottomLayer->getTag());

    //_fieldGUI->setPosition(Vec2(centerX, centerY));
    //this->addChild(_fieldGUI, 10);
}

void GameScene::addGestureRecognizers() {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    log("init for ios/android");
    //auto swipe = SwipeGestureRecognizer::create();
    //swipe->onSwipe = CC_CALLBACK_1(GameScene::onSwipe, this);
    //addChild(swipe);
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
