#pragma once

#include <map>
#include <string>
#include <vector>

#include "cocos2d.h"
#include "UI.h"

using namespace std;
using namespace cocos2d;

class UIEditNew;

//add by maminghai	2013-11-20
//use loadchild load Ui file record
struct ChildUiLocateItem
{
	std::vector<std::string> parentNameArr;
};

class UIManager : public Layer
{
public:
	static UIManager*  getInstance();

	typedef std::map<std::string, UI*> UIMap;
	std::map<std::string, ChildUiLocateItem> m_mapChildLocatea;

	UI*			findUI(const string& name);
	UI*			findUIByName(const string name);
	string		findTopRootName(const string name);

	UI*			loadUI(const string& name, bool isTop = true, int zoder = 0);
	void		addUI(UI* pUI, bool isTop = true);
	void		addUI(UI* pUI, int id, bool isTop = true);
	UI*			loadUIWithIndex(const string& name, int iIndex, bool isTop = true, int zoder = 0);

	UI*			findUI(const string& name, int iIndex);
	void		removeUI(const string& name, int iIndex);
	void		removeUI(const string& name);
	UI*			findUsingUI(const string& uiFileName, const string& uiNodeName);

	UIManager();
	~UIManager();

	void			clear();

	UIMap&			GetUIMap() { return m_uiMap; }
	virtual void	update(float dt);

	//void			AddTouchDelegate( CCTouchDelegate* pHandler );
	//void			RemoveTouchDelegate( CCTouchDelegate* pHandler );

	bool onTouchBegan(Touch* touch, Event* event) override;
	void onTouchMoved(Touch* touch, Event* event) override;
	void onTouchEnded(Touch* touch, Event* event) override;

	void onTouchesBegan(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event  *event) override;
	void onTouchesMoved(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event  *event) override;
	void onTouchesEnded(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event  *event) override;

	UI*			getTouchUI(Touch* touch);

	Touch*			getCurTouch(void) { return m_curTouch; }
	static void		PushEvent(UI* p, const EventFunData& data);
	static void		RemoveEvent(UI* p);
	static void		PushOtherEvent(UI* p, const EventFunData& data);
	static void		RemoveOtherEvent(UI* p);
	void			setOnlyMessage(UI *p) { m_pOnlyMessage = p; };
	UI*				getOnlyMessage() { return m_pOnlyMessage; };

	void    topUI(UI* ui);
	void    onLoseTop(UI* ui);
	void    onGetTop(UI* ui);
	//在childmap表查找所需ui
	UI*		findUIInChildMap(const string& name);
	//在使用loadchild时添加新节点到childmap表,name为查找ui的名称,thisUi为使用loadchild的ui指针
	void	addItemToChildMap(const string name, UI* thisUi);
	//在删除节点时检测表中是否存在，存在则删除
	void	removeItemFromChildMap(const string name);
	//清理整个节点表
	void	clearChildMap();

public:
	vector<UI*>		m_selected;
	UIEditNew *		m_NonceEdit; //当前输入框
	float			m_holdTime;
	Point			m_curPos;
	Rect			m_nonceClipRect;
	Point			m_TranslatefPos;
	//std::set<CCTouchDelegate*> m_TouchHandlers;

	std::vector<UI*> m_stack;

#ifdef USED_JUMP
	static int m_draged;
#endif
private:
	UIMap			m_uiMap;
	list<UIEvent>	m_eventList;
	list<UIEvent>	m_otherEventList;
	Touch*			m_curTouch;
	UI*				m_pOnlyMessage;
	bool			m_bIsDrag;
	int				m_iTouchNum;
#ifdef USE_UI_DRAG
private:
	UI*				m_pDragUI;
	void			_CreateGragUI(UI* srcUI, CCTouch *touch);
	void			_MoveGragUI(CCTouch *touch);
	void			_HandleGragEvent(UI * destUI, CCTouch *touch);
#endif

};