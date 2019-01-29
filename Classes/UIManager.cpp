#include "UIManager.h"
#include "UIEditNew.h"
#include "GameScene.h"

UIManager*  UIManager::getInstance()
{
	static UIManager  m_pinstance;
	return &m_pinstance;
}

UIManager::UIManager()
{
	//支持多点触控
	//CCDirector::sharedDirector()->getTouchDispatcher()->addStandardDelegate(this, 0);	
	//CCTouchDispatcher::sharedDispatcher()->addTargetedDelegate(this, 0, true);	

	m_curTouch = NULL;
	m_NonceEdit = NULL;
	m_pOnlyMessage = NULL;
	m_bIsDrag = false;

	m_iTouchNum = 0;

	m_TranslatefPos = Vec2(0, 0);

	// Adds Touch Event Listener
	auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);

	listener->onTouchBegan = CC_CALLBACK_2(UIManager::onTouchBegan, this);
	listener->onTouchMoved = CC_CALLBACK_2(UIManager::onTouchMoved, this);
	listener->onTouchEnded = CC_CALLBACK_2(UIManager::onTouchEnded, this);

	_eventDispatcher->addEventListenerWithFixedPriority(listener, -10);
	_touchListener = listener;

#ifdef USE_UI_DRAG
	m_pDragUI = NULL;
#endif
}

UIManager::~UIManager()
{
	//for ( map<string,UI*>::iterator i=m_uiMap.begin(); i!=m_uiMap.end(); i++ )
	//{
	//	i->second->release();		
	//}
}

bool g_DoTouch = true;
bool UIManager::onTouchBegan(Touch* touch, Event* event)
{
#ifdef USED_JUMP
	m_draged = 0;
#endif

	if (m_NonceEdit) {
		m_NonceEdit->detachWithIME();
		m_NonceEdit = NULL;
		g_DoTouch = false;
		return true;
	}

#ifdef USE_UI_DRAG
	if (m_pDragUI)
	{
		m_pDragUI->removeFromParentAndCleanup(true);
		m_pDragUI = NULL;
	}
#endif

	g_DoTouch = true;

	m_curPos = touch->getLocationInView();
	m_holdTime = 0;
	m_curTouch = touch;

	bool isHaveMonopolizeTouchUI = false;
	if (m_pOnlyMessage && !m_pOnlyMessage->isVisible())
		setOnlyMessage(NULL);
	if (m_pOnlyMessage) {
		UI *p = m_pOnlyMessage->getTouchUI(touch);
		if (p) {
			m_selected.push_back(p);
		}
		else {
			m_selected.push_back(m_pOnlyMessage);
		}
		if (m_NonceEdit)
			m_NonceEdit->detachWithIME();
		m_NonceEdit = NULL;
	}
	else {
		GameScene* scene = GameScene::GetScene();
		if (scene == NULL)
			return false;
		Ref* layer;
		UI * pMonopolizeTouch = NULL;

		for (const auto &layer : scene->m_uiNode->getChildren()) {
			UI* layer1 = (UI*)layer;
			UI* pUI = NULL;

			if (!layer1->isVisible() || layer1->isIgnoreTouch())
				continue;

			if (layer1->isMonopolizeTouch())
				isHaveMonopolizeTouchUI = true;

			if (isHaveMonopolizeTouchUI && !layer1->isMonopolizeTouch())
				continue;
			if (!layer1->isInClipRect(touch))
				continue;

			pUI = layer1->getTouchUI(touch);
			if (pUI == NULL) {
				// 如果当前是独占，单无任何控件响应事件，则忽略本次触摸事件
				if (isHaveMonopolizeTouchUI)
					break;
				else
					continue;
			}

			if (pUI->m_type == UI_UIEDIT) {
				if (m_NonceEdit) m_NonceEdit->detachWithIME();

				m_NonceEdit = (UIEditNew*)pUI;
				m_NonceEdit->attachWithIME();
			}

			if (((pUI->m_tag == 0) || (!pUI->m_down)) && pUI->isEnable()) {
				m_selected.push_back(pUI);
				if (pUI->m_tag != 0) {
					Node *pParent = pUI->getParent();
					if (pParent) {
						for (const auto &child : pParent->getChildren()) {
							UI* p = (UI*)child;
							if ((p != pUI) && (p->m_tag == pUI->m_tag)) {
								p->m_down = false;
							}
						}
						pUI->m_down = true;
					}
					break;
				}
			}

			if (pUI->isPenetrateTouch()) {
				continue;
			}
			else {
				break;
			}
		}
	}
	if (m_selected.size()) {
		for (int i = 0; i < (int)m_selected.size(); ++i) {
			m_selected[i]->m_down = true;
			m_selected[i]->OnEvent(EVENT_DOWN, touch);
		}
		return true;
	}

	/*
	//窗口独占情况下，地图不能点击
	if (!isHaveMonopolizeTouchUI) {
		if (GameScene::GetScene()->ccTouchBegan(touch, event)) {
			return true;
		}
	}
	*/
	return true;
}

void UIManager::onTouchMoved(Touch* touch, Event* event)
{
#ifdef USED_JUMP
	m_draged++;
#endif

	m_holdTime = -1;
	if (!g_DoTouch) return;

#ifdef USE_UI_DRAG
	if (m_pDragUI)
	{
		_MoveGragUI(touch);
		return;
	}
#endif

	Vec2 pos = touch->getLocationInView();
	if (!(fabs(pos.x - m_curPos.x) >= 5 || fabs(pos.y - m_curPos.y) >= 5)) {
		return;
	}

	m_curPos = pos;
	m_curTouch = touch;
	m_bIsDrag = true;
	bool isHaveMonopolizeTouchUI = false;

	if (m_pOnlyMessage && !m_pOnlyMessage->isVisible()) setOnlyMessage(NULL);
	if (m_pOnlyMessage) {
		UI *p = m_pOnlyMessage->getTouchUI(touch);
		if (p) {
			p->OnEvent(EVENT_DRAG, touch);
		}
		else {
			m_pOnlyMessage->OnEvent(EVENT_DRAG, touch);
		}
	}
	else {
		bool	isHold = false;
		for (int i = 0; i < (int)m_selected.size(); ++i) {
			UI* p = m_selected[i];
			UI* pUI = NULL;

			if (!p->isVisible() || p->isIgnoreTouch())
				continue;

			if (p->isMonopolizeTouch())
				isHaveMonopolizeTouchUI = true;

			if (isHaveMonopolizeTouchUI && !p->isMonopolizeTouch())
				continue;

			if (!p->isInClipRect(touch))
				continue;

			p->OnEvent(EVENT_DRAG, touch);

			Rect rect;
			if (p->isEnlarge()) {
				rect = CCRectMake(p->m_width / 4, p->m_height / 4, p->m_width + p->m_width / 2, p->m_height + p->m_height / 2);
			}
			else {
				rect = CCRectMake(0, 0, p->m_width, p->m_height);
			}

			/*
			if (Rect::RectContainsPoint(rect, p->convertTouchToNodeSpaceAR(touch)))
			{
				isHold = true;
			}
			*/
		}

		if (!isHold) {
			m_holdTime = -1;
		}
		if (!m_selected.size()) {
			if (!isHaveMonopolizeTouchUI) {
				//GameScene::GetScene()->ccTouchMoved(touch, event);
			}
		}
	}
}

void UIManager::onTouchEnded(CCTouch* touch, CCEvent* event)
{
	if (!g_DoTouch) return;
	m_curPos = touch->getLocationInView();
	m_curTouch = touch;

	if (m_pOnlyMessage && !m_pOnlyMessage->isVisible()) setOnlyMessage(NULL);
	if (m_pOnlyMessage)
	{
		UI *p = m_pOnlyMessage->getTouchUI(touch);
		if (p)
		{
#ifdef USE_UI_DRAG
			_HandleGragEvent(p, touch);
#endif
			p->m_down = false;
			p->OnEvent(EVENT_UP, touch);
		}
		else if (m_pOnlyMessage)
		{
#ifdef USE_UI_DRAG
			_HandleGragEvent(m_pOnlyMessage, touch);
#endif
			m_pOnlyMessage->m_down = false;
			m_pOnlyMessage->OnEvent(EVENT_UP, touch);
		}
		/*
		if(!GuideModule::instance().IsInGuide())
		{
		for( int i=0; i<(int)m_selected.size(); ++i )
		{
		if( m_selected[i]->m_tag == 0 )
		m_selected[i]->m_down = false;
		}
		m_selected.clear();
		#ifdef USE_UI_DRAG
		_HandleGragEvent( NULL, touch );
		#endif
		return;
		}
		*/
		m_bIsDrag = false;
	}

	vector<UI*> upUIList;
	GameScene* scene = GameScene::GetScene();
	bool isHaveMonopolizeTouchUI = false;
	for (const auto &layer : scene->m_uiNode->getChildren())
	{
		UI* layer1 = (UI*)layer;
		UI* pUI = NULL;
		if (!layer1->isVisible() || layer1->isIgnoreTouch())
			continue;

		if (layer1->isMonopolizeTouch())
			isHaveMonopolizeTouchUI = true;

		if (isHaveMonopolizeTouchUI && !layer1->isMonopolizeTouch())
			continue;

		if (!layer1->isInClipRect(touch))
			continue;

		pUI = layer1->getTouchUI(touch);

		if (pUI != NULL) {
#ifdef USE_UI_DRAG
			_HandleGragEvent(pUI, touch);
#endif
			for (int i = 0; i < (int)m_selected.size(); ++i) {
				if (pUI == m_selected[i]) {
					upUIList.push_back(pUI);
					break;
				}
			}
			if (pUI->isPenetrateTouch())
				continue;
			break;
		}
	}

#ifdef USE_UI_DRAG
	_HandleGragEvent(NULL, touch);
#endif

	for (int i = 0; i < (int)upUIList.size(); ++i) {
		upUIList[i]->OnEvent(EVENT_UP, touch);
	}

	if (m_selected.size()) {
		for (int i = 0; i < (int)m_selected.size(); ++i) {
			if (m_selected[i]->m_tag == 0)
				m_selected[i]->m_down = false;
			if (m_bIsDrag) {
				m_selected[i]->OnEvent(EVETN_DRAG_END, touch);
			}
		}
		m_selected.clear();
	}
	else {
		/*
		if( !isHaveMonopolizeTouchUI )
		{
		GameScene::GetScene()->ccTouchEnd(touch, event);
		}
		*/
	}
	m_bIsDrag = false;
}

void UIManager::PushEvent(UI* p, const EventFunData& data)
{
	UIEvent event;

	event.self = p;
	event.data = data;

	UIManager::getInstance()->m_eventList.push_back(event);
}

void UIManager::PushOtherEvent(UI* p, const EventFunData& data)
{
	UIEvent event;

	event.self = p;
	event.data = data;

	UIManager::getInstance()->m_otherEventList.push_back(event);
}

void UIManager::RemoveEvent(UI* p)
{
	list<UIEvent>::iterator it = UIManager::getInstance()->m_eventList.begin();
	while (it != UIManager::getInstance()->m_eventList.end())
	{
		if (it->self == p)
		{
			it->self = NULL;
		}

		it++;
	}

	vector<UI*>::iterator it2 = UIManager::getInstance()->m_selected.begin();
	for (; it2 != UIManager::getInstance()->m_selected.end(); it2++)
	{
		if (*it2 == p)
		{
			UIManager::getInstance()->m_selected.erase(it2);
			break;
		}
	}

}

void UIManager::RemoveOtherEvent(UI* p)
{
	list<UIEvent>::iterator it = UIManager::getInstance()->m_otherEventList.begin();
	while (it != UIManager::getInstance()->m_otherEventList.end())
	{
		if (it->self == p)
		{
			it->self = NULL;
		}

		it++;
	}

	vector<UI*>::iterator it2 = UIManager::getInstance()->m_selected.begin();
	for (; it2 != UIManager::getInstance()->m_selected.end(); it2++)
	{
		if (*it2 == p)
		{
			UIManager::getInstance()->m_selected.erase(it2);
			break;
		}
	}
}

UI*	UIManager::getTouchUI(CCTouch* touch)
{
	GameScene* scene = GameScene::GetScene();
	if (scene == NULL)
		return NULL;
	CCObject* layer;
	UI * pMonopolizeTouch = NULL;
	auto& children = scene->m_uiNode->getChildren();
	for (const auto &layer : children) {

		UI* layer1 = (UI*)layer;
		UI* pUI = NULL;

		if (!layer1->isVisible() || layer1->isIgnoreTouch())
			continue;

		if (!layer1->isInClipRect(touch))
			continue;

		pUI = layer1->getTouchUI(touch);
		if (pUI)
		{
			return pUI;
		}
	}
	return NULL;
}

static bool sCanTouch = true;
static unsigned long sFrontTouchTime = 0;

void UIManager::onTouchesBegan(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event  *event)
{
	log("UIManager::onTouchesBegan");
	/*
	cc_timeval now;
	CCTime::gettimeofdayCocos2d(&now, NULL);
	unsigned long nowTime = now.tv_sec * 1000 + now.tv_usec / 1000;
	if (nowTime - sFrontTouchTime >= 300)
	{
		sFrontTouchTime = nowTime;
		sCanTouch = true;
	}
	else
	{
		sCanTouch = false;
		return;
	}
	Vec2 location[2];
	CCSetIterator iter = pTouches->begin();
	bool	isUseDirectCMD = false;

	for (int i = 0; i < 2 && iter != pTouches->end(); iter++, ++i)
	{
		CCTouch* pTouch = (CCTouch*)(*iter);
		location[i] = pTouch->locationInView();

		//如果本次点击被摇杆获取，跳过本次点击
		if (GameScene::GetScene()->m_eGameState == GAME_STATE_GAME)
		{
			if (CDirectHandleUI::instance().onTouchBegin(pTouch))
			{
				isUseDirectCMD = true;
				continue;
			}
			if (CShortPage::instance().getShortCut()
				&& CShortPage::instance().getShortCut()->isCanBegin(pTouch))
			{
				CShortPage::instance().getShortCut()->ccTouchBegan(pTouch, NULL);
				isUseDirectCMD = true;
				continue;
			}

			GuideModule::instance().ccTouchBegan(pTouch, NULL);

		}


		if (i == 0 && pTouches->count() == 1)
		{
			ccTouchBegan(pTouch, pEvent);

			std::set<CCTouchDelegate*>::iterator it = m_TouchHandlers.begin();
			std::set<CCTouchDelegate*>::iterator end = m_TouchHandlers.end();
			while (it != end)
			{
				(*it)->ccTouchBegan(pTouch, pEvent);
				++it;
			}
		}

		UI *pUI = UIManager::Instance()->findUI("main_superskill.ui");
		if (pUI)
		{
			if (pUI->isVisible())
			{
				UIScrollView *pCheatsScrollView = dynamic_cast<UIScrollView*>(pUI->findUI("pageequipmentz1"));;
				if (pCheatsScrollView)
				{
					Rect rect = pCheatsScrollView->getGlobalRect();
					if (!rect.containsPoint(pTouch->getLocation())) //&& !rect1.containsPoint(dynamic_cast< CCTouch *>(pTouches).getLocation()))
					{
						CGameMainUI::instance()->CloseCheatsList();
					}

				}
			}
		}
	}//遍历取出每个触摸点坐标
	//wcc
	if (!isUseDirectCMD && !Vec2::Vec2EqualToPoint(location[0], Vec2Zero) && !Vec2::Vec2EqualToPoint(location[1], Vec2Zero) && gCanTouchForZoom)
	{
		sIsFinishChange = false;

		float x = location[0].x - location[1].x;
		float y = location[0].y - location[1].y;
		sDistance = sqrtf(x*x + y*y);
	}

	//UI *pUI = this->findUI("main_superskill.ui");
	//if (pUI)
	//{
	//	UI* pUISroll = pUI->findUI("pageequipmentz1");
	//	if (pUISroll)
	//	{
	//		 if(pUISroll->m_rect9->intersectsRect(location[0]));
	//		 {
	//			 CGameMainUI::instance()->CloseCheatsList();
	//		 }
	//	}
	//}
	*/
}

void UIManager::onTouchesMoved(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event  *event)
{
	log("UIManager::onTouchesMoved");
	/*
	Vec2 location[2];
	CCSetIterator iter = pTouches->begin();
	bool	isUseDirectCMD = false;


	for (int i = 0; i < 2 && iter != pTouches->end(); iter++, ++i)
	{
		CCTouch* pTouch = (CCTouch*)(*iter);
		location[i] = pTouch->locationInView();

		//如果本次点击被摇杆获取，跳过本次点击
		if (GameScene::GetScene()->m_eGameState == GAME_STATE_GAME)
		{
			if (CDirectHandleUI::instance().onTouchMoved(pTouch))
			{
				isUseDirectCMD = true;
				continue;
			}
			if (CShortPage::instance().getShortCut()
				&& CShortPage::instance().getShortCut()->isBegining())
			{
				CShortPage::instance().getShortCut()->ccTouchMoved(pTouch, NULL);
				isUseDirectCMD = true;
				continue;
			}

			GuideModule::instance().ccTouchMoved(pTouch, NULL);
		}

		if (i == 0 && pTouches->count() == 1)
		{
			if (sCanTouch)
			{
				ccTouchMoved(pTouch, pEvent);

				std::set<CCTouchDelegate*>::iterator it = m_TouchHandlers.begin();
				std::set<CCTouchDelegate*>::iterator end = m_TouchHandlers.end();
				while (it != end)
				{
					(*it)->ccTouchMoved(pTouch, pEvent);
					++it;
				}
			}
		}
	}//遍历取出每个触摸点坐标

	if (!isUseDirectCMD && !Vec2::Vec2EqualToPoint(location[0], Vec2Zero) && !Vec2::Vec2EqualToPoint(location[1], Vec2Zero) && sDistance == 0.0f && gCanTouchForZoom)
	{
		float x = location[0].x - location[1].x;
		float y = location[0].y - location[1].y;
		sDistance = sqrtf(x*x + y*y);
	}


	if (!isUseDirectCMD && !m_NonceEdit && !Vec2::Vec2EqualToPoint(location[0], Vec2Zero) && !Vec2::Vec2EqualToPoint(location[1], Vec2Zero) && !m_selected.size() && gCanTouchForZoom)
	{
		sIsFinishChange = false;

		float x = location[0].x - location[1].x;
		float y = location[0].y - location[1].y;
		float dis = sqrtf(x*x + y*y);
		float ddt = dis - sDistance;

		float scale = ddt / _SPEED_PER_TOUCHES;

		sNowScale += scale;

		if (sNowScale > _LIMITED_MAX_SCREEN_SCALE) {
			sNowScale = _LIMITED_MAX_SCREEN_SCALE;
		}
		if (sNowScale < _LIMITED_MIN_SCREEN_SCALE) {
			sNowScale = _LIMITED_MIN_SCREEN_SCALE;
		}

		Map* pMap = GameScene::GetScene()->GetCurMap();
		CCamera* pCamera = pMap ? pMap->GetCamera() : 0;
		if (pCamera && ddt)
		{
			gAntoScale = sNowScale;
			pCamera->setSceneScale(sNowScale);
		}
	}
	*/
}

void UIManager::onTouchesEnded(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event  *event)
{
	/*
	Vec2 location[2];
	CCSetIterator iter = pTouches->begin();
	bool	isUseDirectCMD = false;

	if (sDistance != 0.0f) {
		sDistance = 0.0f;
		sIsFinishChange = true;
	}

	for (int i = 0; i < 2 && iter != pTouches->end(); iter++, ++i)
	{
		CCTouch* pTouch = (CCTouch*)(*iter);
		location[i] = pTouch->getLocationInView();

		//如果本次点击事件被摇杆获取，跳过本次点击
		if (GameScene::GetScene()->m_eGameState == GAME_STATE_GAME)
		{
			if (CDirectHandleUI::instance().onTouchEnd(pTouch))
			{
				isUseDirectCMD = true;
				continue;
			}
			if (CShortPage::instance().getShortCut()
				&& CShortPage::instance().getShortCut()->isBegining())
			{
				CShortPage::instance().getShortCut()->ccTouchEnded(pTouch, NULL);
				isUseDirectCMD = true;
				continue;
			}

			GuideModule::instance().ccTouchEnd(pTouch, NULL);
		}

		if (m_iTouchNum <= 0)
		{
			if (sCanTouch)
			{
				ccTouchEnded(pTouch, pEvent);

				std::set<CCTouchDelegate*>::iterator it = m_TouchHandlers.begin();
				std::set<CCTouchDelegate*>::iterator end = m_TouchHandlers.end();
				while (it != end)
				{
					(*it)->ccTouchEnded(pTouch, pEvent);
					++it;
				}
			}
		}
		m_iTouchNum--;
	}//遍历取出每个触摸点坐标

	GameScene::GetMap()->restHoldTime();
	*/
}

UI*	UIManager::findUsingUI(const string& uiFileName, const string& uiNodeName)
{
	UI *pUI = NULL;
	pUI = findUI(uiFileName);
	if (pUI && pUI->isVisible())
	{
		UI *p = pUI->findUsingUI("", uiNodeName);
		if (p) return p;
	}
	UIMap::iterator it = m_uiMap.begin();
	for (; it != m_uiMap.end(); ++it)
	{
		UI *p = it->second->findUsingUI(uiFileName, uiNodeName);
		if (p) return p;
	}
	return NULL;
}

////////////////////////////////////////////////////////////////////////////////////
void UIManager::topUI(UI *ui)
{
	if (!ui->m_bisRoot)
		return;

	for (UIMap::iterator mit = m_uiMap.begin(); mit != m_uiMap.end(); ++mit) {
		if (mit->second == ui) {
			if (m_stack.size() > 0) {
				// find ui
				for (std::vector<UI*>::iterator it = m_stack.begin();
					it != m_stack.end();
					++it)
				{
					if (*it == ui) {
						m_stack.erase(it);
						break;
					}
				}
			}

			// push
			UI* old_top = m_stack.size() > 0 ? (*(m_stack.rbegin())) : NULL;

			if (old_top && old_top != ui) {
				old_top->onLoseTop();
				//CCLOG("ON LOSE TOP:%s\n", old_top->m_name.c_str());
			}

			// push
			m_stack.push_back(ui);

			// new top
			ui->onGetTop();
			//CCLOG("ON GET TOP:%s\n", ui->m_name.c_str());

			break;
		}
	}
}

void UIManager::onLoseTop(UI *ui)
{
	if (!ui->m_bisRoot)
		return;

	// lose
	UI* old_top = m_stack.size() > 0 ? (*(m_stack.rbegin())) : NULL;

	if (old_top && old_top == ui) {
		old_top->onLoseTop();
		//CCLOG("ON LOSE TOP:%s\n", old_top->m_name.c_str());
	}

	// remove
	for (std::vector<UI*>::iterator it = m_stack.begin();
		it != m_stack.end();
		++it)
	{
		if (*it == ui) {
			m_stack.erase(it);
			break;
		}
	}

	// cur
	UI* cur_top = m_stack.size() > 0 ? (*(m_stack.rbegin())) : NULL;
	if (cur_top) {
		cur_top->onGetTop();
		//CCLOG("nON GET TOP:%s\n", cur_top->m_name.c_str());
	}

}

void UIManager::onGetTop(UI *ui)
{
	// lose
	Node* p = ui->getParent();

	if (p != NULL) {
		ui->retain();
		p->removeChild(ui, false);
		p->addChild(ui);
		ui->release();
	}

	this->topUI(ui);
}

void UIManager::clear()
{
	clearChildMap();
	m_uiMap.clear();
	m_iTouchNum = 0;
	m_pOnlyMessage = NULL;
	m_NonceEdit = NULL;
	m_selected.clear();
	m_curTouch = NULL;

	m_stack.clear();
}

UI* UIManager::findUI(const string& name)
{
	map<string, UI*>::iterator it = m_uiMap.find(name);

	if (it != m_uiMap.end()) {
		return it->second;
	}

	return NULL;
}

string	UIManager::findTopRootName(const string name)
{
	map<string, UI*>::iterator it1 = m_uiMap.find(name);
	if (it1 != m_uiMap.end()) {
		return name;
	}
	std::map<std::string, ChildUiLocateItem>::iterator it2 = m_mapChildLocatea.find(name);
	if (it2 != m_mapChildLocatea.end()) {
		return it2->second.parentNameArr[0];
	}
	return "";
}

UI*	UIManager::findUIByName(const string name)
{
	UI*	findTarget = findUI(name);
	if (findTarget == NULL) {
		findTarget = findUIInChildMap(name);
	}
	return findTarget;
}
//在childmap表查找所需ui
UI* UIManager::findUIInChildMap(const string& name)
{
	std::map<std::string, ChildUiLocateItem>::iterator it = m_mapChildLocatea.find(name);
	if (it != m_mapChildLocatea.end()) {
		if (!it->second.parentNameArr.empty()) {
			if (it->second.parentNameArr[0] != "") {
				UI*	findTarget = findUI(it->second.parentNameArr[0]);
				if (findTarget) {
					int layerNum = it->second.parentNameArr.size();
					for (int i = 1; i < layerNum; i++) {
						if (it->second.parentNameArr[i] != "") {
							findTarget = findTarget->findUI(it->second.parentNameArr[i]);
							if (findTarget == NULL) {
								return NULL;
							}
						}
						else {
							return NULL;
						}
					}
					return findTarget->findUI(name);
				}
			}
		}
	}
	return NULL;
}

//在使用loadchild时添加新节点到childmap表
void UIManager::addItemToChildMap(const string name, UI* thisUi)
{
	GameScene* scene = GameScene::GetScene();
	vector<string> tempNameVec;
	if (thisUi)
	{
		tempNameVec.push_back(thisUi->m_name);
		if (thisUi != scene->m_uiNode)
		{
			UI*	findParents = (UI*)(thisUi->getParent());
			while (findParents && findParents != scene->m_uiNode)
			{
				tempNameVec.push_back(findParents->m_name);
				findParents = (UI*)(findParents->getParent());
			}
		}
		ChildUiLocateItem locateItem;
		int layerNum = tempNameVec.size() - 1;
		locateItem.parentNameArr.resize(tempNameVec.size());
		for (int i = layerNum; i >= 0; i--)
		{
			locateItem.parentNameArr[i] = tempNameVec[layerNum - i];
		}
		m_mapChildLocatea[name] = locateItem;
	}
}

//在删除节点时检测表中是否存在，存在则删除
void UIManager::removeItemFromChildMap(const string name)
{
	std::map<std::string, ChildUiLocateItem>::iterator it = m_mapChildLocatea.find(name);
	if (it != m_mapChildLocatea.end())
	{
		m_mapChildLocatea.erase(it);
	}
}

//清理整个节点表
void UIManager::clearChildMap()
{
	m_mapChildLocatea.clear();

}

void UIManager::update(float dt)
{
	if (m_holdTime >= 0) m_holdTime += dt;

#ifdef USE_UI_DRAG
	if (m_holdTime >= 1.0f)
	{
		for (size_t i = 0; i < m_selected.size(); ++i)
		{
			if (!m_pDragUI)
			{
				_CreateGragUI(m_selected[i], m_curTouch);
			}
		}
	}
#endif

	Layer::update(dt);

	if (m_pOnlyMessage) {
		if (!m_pOnlyMessage->isVisible()) {
			setOnlyMessage(NULL);
		}
	}
	for (map<string, UI*>::iterator i = m_uiMap.begin(); i != m_uiMap.end(); i++)
	{
		if (i->second->isVisible())
		{
			i->second->update(dt);
		}
	}

	/*
	for (map<string, UI*>::iterator i = m_uiMap.begin(); i != m_uiMap.end(); i++)
	{
		if (i->second->isRemove())
		{
			CMainUIOpenCloseHandle::instance().DelHandleUI(i->second);
			i->second->removeFromParentAndCleanup(true);
			m_uiMap.erase(i);
			break;
		}
	}

	//////////////////////////////update for zoom in/out////////////////////////////////////////////

	if (sIsFinishChange) {
		Map* pMap = GameScene::GetScene()->GetCurMap();
		CCamera* pCamera = pMap ? pMap->GetCamera() : 0;

		if (sNowScale > _STANDARD_MAX_SCREEN_SCALE)
		{
			sNowScale -= 0.5f*dt;
			if (sNowScale < _STANDARD_MAX_SCREEN_SCALE) {
				sNowScale = _STANDARD_MAX_SCREEN_SCALE;
			}

			if (pCamera)
			{
				gAntoScale = sNowScale;
				pCamera->setSceneScale(sNowScale);
			}
		}
		else if (sNowScale < _STANDARD_MIN_SCREEN_SCALE)
		{
			sNowScale += 0.5f*dt;
			if (sNowScale > _STANDARD_MIN_SCREEN_SCALE) {
				sNowScale = _STANDARD_MIN_SCREEN_SCALE;
			}

			if (pCamera)
			{
				gAntoScale = sNowScale;
				pCamera->setSceneScale(sNowScale);
			}
		}
	}


	if (sNowScale != gAntoScale)
	{
		if (sNowScale < gAntoScale)
		{
			sNowScale += 0.5f*dt;
			if (sNowScale >= gAntoScale)
			{
				sNowScale = gAntoScale;
			}
		}
		else
		{
			sNowScale -= 0.5f*dt;
			if (sNowScale <= gAntoScale)
			{
				sNowScale = gAntoScale;
			}
		}

		Map* pMap = GameScene::GetScene()->GetCurMap();
		CCamera* pCamera = pMap ? pMap->GetCamera() : 0;
		if (pCamera)
		{
			pCamera->setSceneScale(sNowScale);
		}
	}
	*/
	//////////////////////////////////////////////////////////////////////////

	list<UIEvent>::iterator it = m_eventList.begin();
	while (it != m_eventList.end())
	{
		if (it->self)
		{
			it->data.funcEvent(it->self, it->data.pData);
		}

		it++;
	}

	m_eventList.clear();

	it = m_otherEventList.begin();
	while (it != m_otherEventList.end())
	{
		if (it->self)
		{
			it->data.funcEvent(it->self, it->data.pData);
		}

		it++;
	}

	m_otherEventList.clear();
}

UI* UIManager::loadUI(const string& name, bool isTop, int zoder)
{
	UI* rt = findUI(name);
	if (rt != NULL) return rt;

	UIDataGroup group;

	group.Load(name.c_str());

	rt = group.createUI(zoder);

	int x = SCREEN_SIZE.width / 2;
	if ((group.align & ALIGN_LEFT) != 0) {
		x = 0;
	}
	if ((group.align & ALIGN_HCENTER) != 0) {
		x = SCREEN_SIZE.width / 2;
	}
	if ((group.align & ALIGN_RIGHT) != 0) {
		x = SCREEN_SIZE.width;
	}

	int y = SCREEN_SIZE.height / 2;
	if ((group.align & ALIGN_TOP) != 0) {
		y = SCREEN_SIZE.height;
	}
	if ((group.align & ALIGN_VCENTER) != 0) {
		y = SCREEN_SIZE.height / 2;
	}
	if ((group.align & ALIGN_BOTTOM) != 0) {
		y = 0;
	}
	rt->setPosition(x, y);

	m_uiMap[name] = rt;

	if (isTop) {
		rt->m_bisRoot = true;
		rt->Top();
	}

	return rt;
}


void UIManager::addUI(UI* pUI, bool isTop)
{
	if (pUI) {
		if (m_uiMap[pUI->m_name] != NULL) {
			return;
		}
		GameScene::GetScene()->m_uiNode->addChild(pUI, 10000);
		m_uiMap[pUI->m_name] = pUI;
	}

	if (isTop)
	{
		pUI->m_bisRoot = true;
		pUI->Top();
	}
}

void UIManager::addUI(UI* pUI, int id, bool isTop)
{
	if (pUI)
	{
		if (m_uiMap[pUI->m_name] != NULL)
		{
			return;
		}
		GameScene::GetScene()->m_uiNode->addChild(pUI, 0);
		m_uiMap[pUI->m_name] = pUI;
	}

	if (isTop)
	{
		pUI->m_bisRoot = true;
		pUI->Top();
	}
}


//*************************
//2012.8.22 by yaf 实现UI多实例
UI* UIManager::loadUIWithIndex(const string& name, int iIndex, bool isTop, int zoder)
{
	char buff[256];
	sprintf(buff, "%s_%d", name.c_str(), iIndex);
	string key = buff;

	UI* rt = findUI(key);
	if (rt != NULL) return rt;

	UIDataGroup group;
	group.Load(name.c_str());

	rt = group.createUI(zoder);

	int x = SCREEN_SIZE.width / 2;
	if ((group.align & ALIGN_LEFT) != 0)
	{
		x = 0;
	}
	if ((group.align & ALIGN_HCENTER) != 0)
	{
		x = SCREEN_SIZE.width / 2;
	}
	if ((group.align & ALIGN_RIGHT) != 0)
	{
		x = SCREEN_SIZE.width;
	}

	int y = SCREEN_SIZE.height / 2;
	if ((group.align & ALIGN_TOP) != 0)
	{
		y = SCREEN_SIZE.height;
	}
	if ((group.align & ALIGN_VCENTER) != 0)
	{
		y = SCREEN_SIZE.height / 2;
	}
	if ((group.align & ALIGN_BOTTOM) != 0)
	{
		y = 0;
	}
	rt->setPosition(x, y);

	m_uiMap[key] = rt;

	if (isTop)
	{
		rt->m_bisRoot = true;
		rt->Top();
	}

	return rt;
}

UI* UIManager::findUI(const string& name, int iIndex)
{
	char buff[256];
	sprintf(buff, "%s_%d", name.c_str(), iIndex);
	string key = buff;

	map<string, UI*>::iterator it = m_uiMap.find(key);
	if (it != m_uiMap.end())
	{
		return it->second;
	}
	return NULL;
}


void UIManager::removeUI(const string& name, int iIndex)
{
	char buff[256];
	sprintf(buff, "%s_%d", name.c_str(), iIndex);
	string key = buff;

	map<string, UI*>::iterator it = m_uiMap.find(key);
	if (it != m_uiMap.end())
	{
		it->second->remove();

		UIManager::getInstance()->onLoseTop(it->second);
	}
}

void UIManager::removeUI(const string& name)
{
	map<string, UI*>::iterator it = m_uiMap.find(name);
	if (it != m_uiMap.end())
	{
		it->second->remove();
		UIManager::getInstance()->onLoseTop(it->second);
	}
}
/*
void UIManager::RemoveTouchDelegate(CCTouchDelegate* pHandler)
{
	std::set<CCTouchDelegate*>::iterator it = m_TouchHandlers.find(pHandler);
	if (it != m_TouchHandlers.end())
	{
		m_TouchHandlers.erase(it);
	}
}
void UIManager::AddTouchDelegate(CCTouchDelegate* pHandler)
{
	m_TouchHandlers.insert(pHandler);
}
*/

#ifdef USE_UI_DRAG

void	UIManager::_CreateGragUI(UI* srcUI, CCTouch * touch)
{
	if (m_pDragUI)
	{
		return;
	}

	if (!srcUI)
		return;
	if (!touch)
		return;
	GameScene* scene = GameScene::GetScene();
	if (!scene)
		return;
	if (!scene->m_uiNode)
		return;

	UIScrollView *pScroll = dynamic_cast<UIScrollView*>(srcUI);
	if (pScroll)
	{
		Vec2 pos = pScroll->getStartPos();
		UI* pUI = pScroll->UI::getTouchUI(pos.x, pos.y, touch);
		if (pUI) srcUI = pUI;
	}

	if (srcUI->m_pCreateDragFunc)
	{
		m_pDragUI = srcUI->m_pCreateDragFunc(srcUI);
	}

	if (!m_pDragUI)
		return;

	scene->m_uiNode->addChild(m_pDragUI);
	m_pDragUI->m_iDragSrcTag = srcUI->m_iDragSrcTag;
	m_pDragUI->Top();
	Vec2 pos = scene->m_uiNode->convertTouchToNodeSpaceAR(touch);
	m_pDragUI->setPosition(pos.x - m_pDragUI->m_width / 2, pos.y - m_pDragUI->m_height / 2);
}

void UIManager::_MoveGragUI(CCTouch *touch)
{
	if (!touch)
		return;
	if (!m_pDragUI)
		return;
	GameScene* scene = GameScene::GetScene();
	if (!scene)
		return;
	if (!scene->m_uiNode)
		return;
	Vec2 pos = scene->m_uiNode->convertTouchToNodeSpaceAR(touch);
	m_pDragUI->setPosition(pos.x - m_pDragUI->m_width / 2, pos.y - m_pDragUI->m_height / 2);
}


void	UIManager::_HandleGragEvent(UI* destUI, CCTouch *touch)
{
	if (!m_pDragUI)
		return;
	if (!destUI)
	{
		m_pDragUI->removeFromParentAndCleanup(true);
		m_pDragUI = NULL;
		return;
	}

	UIScrollView *pScroll = dynamic_cast<UIScrollView*>(destUI);
	if (pScroll)
	{
		Vec2 pos = pScroll->getStartPos();
		UI* pUI = pScroll->UI::getTouchUI(pos.x, pos.y, touch);
		if (pUI) destUI = pUI;
	}

	if (destUI->m_iDragDestTag == m_pDragUI->m_iDragSrcTag &&  destUI->m_pDragFunc)
	{
		destUI->m_pDragFunc(destUI, m_pDragUI, m_pDragUI->m_pDragSrcData);
	}

	m_pDragUI->removeFromParentAndCleanup(true);
	m_pDragUI = NULL;
}

#endif