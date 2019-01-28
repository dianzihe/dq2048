#include <string>
#include "GameScene.h"
#include "SimpleAudioEngine.h"
//#include "Gestures/GestureRecognizerUtils.h"
//#include "Gestures/SwipeGestureRecognizer.h"
#include "Colors.h"
#include "Board.h"
#include "GemUtils.h"
#include "UI.h"
#include "UIBatchRenderer.h"
#include "UIManager.h"
#include "ZhongGaoState.h"

USING_NS_CC;
NS_GAME_BEGIN

GameScene::GameScene(void){
	m_pRunState = NULL;
	m_uiNode = new Node();
	addChild(m_uiNode, GAME_LAYER_UI);
	UIBatchRenderer::instance()->initilize();
}
void GameScene::create()
{
	GameScene* p = new GameScene();

	if (p && p->init()) {
		p->autorelease();
		//TODO: CCDirector::sharedDirector()->replaceScene(p);
		Director::getInstance()->runWithScene(p);
	}
	Director::getInstance()->setProjection(kCCDirectorProjection3D);
}
GameScene* GameScene::GetScene()
{
	return (GameScene*)(Director::getInstance()->getRunningScene());
}
void GameScene::onEnter()
{
	log("------------GameScene::onEnter");
	//ChangeState(GAME_STATE_ZHONGGAO);
	initBG();
}
void GameScene::onExit()
{
	if (m_pRunState)
	{
		removeChild(m_pRunState, true);
	}
}
/*
bool GameScene::init() {

	log("------------GameScene::init");
    if (!Scene::init()) return false;

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
*/
void GameScene::ChangeState(GAME_STATE eState)
{
	if (eState == m_eGameState)
		return;

	if (eState == GAME_STATE_CUTSCENE ) {
		/*
		if (UpdateTool::AppUpdater::isEnabled()) {
			eState = GAME_STATE_UPDATE;
		}
		else {
#if defined UPDATE_START
			eState = GAME_STATE_UPDATE;
#else
			eState = GAME_STATE_LOGIN;
#endif
		}
		*/
	}

	log("ChangeState m_eGameState:%d eState:%d", m_eGameState, eState);

	m_eGameState = eState;
	if (m_pRunState) {
		removeChild(m_pRunState, true);
	}

	switch (eState)
	{
	case GAME_STATE_UPDATE:
		if (0) {
			//m_pRunState = new NewUpdateState();
		} else {
#ifdef UPDATE_START
			if (m_cps == NULL)
			{
				m_pRunState = new CUpdateState();
				m_cps = (CUpdateState*)m_pRunState;
				m_pRunState->retain();
			}
			else {
				m_pRunState = m_cps;
				m_pRunState->retain();
			}
#endif
		}
		break;
	case GAME_STATE_ZHONGGAO:
		m_pRunState = new CZhongGaoState();
		break;
	case GAME_STATE_CUTSCENE:
		//m_pRunState = new CCutSceneState();
		break;
	case GAME_STATE_LOGIN:
		LoadTask();
		//m_pRunState = new CLoginState();
		break;
	case GAME_STATE_GAMESERVERLIST:
		//m_pRunState = new CGameServerListState();
		break;
	case GAME_STATE_CHARLOBBY:
		//m_pRunState = new CCharLobbyState();
		break;
	case GAME_STATE_CREATEPLAYER:
		//m_pRunState = new CCreatePlayerState();
		break;

	case GAME_STATE_GAME:
		m_pRunState = new CGameState();
		break;

	case GAME_STATE_LOADING:
		m_pRunState = new CLoadingState();
		break;

	case GAME_STATE_BRANCHUPDATE:
		//m_pRunState = new CBranchUpdateState();
		break;

	case GAME_STATE_RECONNECT_LOGIN:
	{
		//m_pRunState = new CLoginState();
		//m_eGameState = GAME_STATE_LOGIN;
		break;
	}

	case GAME_STATE_DISCONNECTION_LOGIN:
	{
		/*
		if (g_lastUserID == 0 || g_lastRandIdentity == "" || g_lastServerID == "" || g_lastPlayerID == 0)
		{
			//露脧脧脽脰脴脕卢脢媒戮脻虏禄脮媒脠路拢卢陆酶脠毛loginstate
			LoadTask();
			m_pRunState = new CLoginState();
			m_eGameState = GAME_STATE_LOGIN;
		}
		else
		{
			//陆酶脠毛露脧脧脽脰脴脕卢state
			m_pRunState = new CDisconnectionLoginState();
		}
		*/
	}
	break;
	}

	if (m_pRunState) {
		m_pRunState->autorelease();
		addChild(m_pRunState);
		m_pRunState->EnterState();
	}

	switch (m_eGameState)
	{
	case GAME_STATE_LOGIN:
	{
		/*
		if (!m_is_connect_login_server) {
			m_is_connect_login_server = true;
		}
		*/
	}
	break;
	}
}
void GameScene::initBG()
{
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("battleMisc.plist");

	background = PH::GemUtils::GetSprite("gameui/background.jpg");
	//background = Sprite::createWithSpriteFrameName("gameui/background.jpg");
	//	PH::GemUtils::GetSprite("gameui/background.jpg");
	background->setPosition(winSize.width / 2, winSize.height / 2);
	this->addChild(background);
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
void GameScene::update(float dt)
{
	{
		UIManager::getInstance()->update(dt);
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
