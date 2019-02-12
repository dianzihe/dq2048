#pragma once

#include <string>
#include "cocos2d.h"
#include "Gestures/SwipeGestureRecognizer.h"
#include "Field.hpp"
#include "GameState.h"

NS_GAME_BEGIN

enum DefaultLanguageType
{
	DLT_None = 0,
	DLT_CHS = 1,
	DLT_CHT = 2
};
enum GAME_STATE
{
	GAME_STATE_NONE,
	GAME_STATE_UPDATE,
	GAME_STATE_ZHONGGAO,
	GAME_STATE_CUTSCENE,
	GAME_STATE_LOGIN,
	GAME_STATE_GAMESERVERLIST,
	GAME_STATE_CHARLOBBY,
	GAME_STATE_CREATEPLAYER,
	GAME_STATE_GAME,
	GAME_STATE_LOADING,
	GAME_STATE_DISCONNECTION_LOGIN,
	GAME_STATE_RECONNECT_LOGIN,
	GAME_STATE_BRANCHUPDATE,
};
enum GAME_LAYER
{
	GAME_LAYER_MAP,
	GAME_LAYER_UI,
};

class GameScene : public cocos2d::Scene
{
public:
	Sprite*			background;
	Size			winSize;
	Node*			m_uiNode;

	GAME_STATE		m_eGameState;
	CRunState*		m_pRunState;

	
	
	GameScene(void);
	~GameScene(void){};

	static GameScene* inst;
	static GameScene* create();
    //static cocos2d::Scene* createScene();
	static GameScene* GetScene();
	static Node* GetUI();

	//virtual void onEnter();
	//virtual void onExit();
	virtual void update(float dt);

	virtual void visit(cocos2d::Renderer *renderer, const cocos2d::Mat4& parentTransform, uint32_t parentFlags) override;
	virtual void draw(cocos2d::Renderer* renderer, const cocos2d::Mat4& transform, uint32_t flags) override;

	void loadPersistentTextureCache();
	void loadTextureCache();
	void initBG();
   // CREATE_FUNC(GameScene);
	
	void ChangeState(GAME_STATE eState);
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

