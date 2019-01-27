#pragma once

#include "UI.h"


const int ScrollType_X		= 1<<1;
const int ScrollType_Y		= 1<<2;
const int ScrollType_PAGE	= 1<<3;


class UIScrollView : public UI
{
CC_SYNTHESIZE(int, m_selectTag, SelectTag);

public:
	UIScrollView(void);
	virtual ~UIScrollView(void);

	virtual void update(float t);
	void visit(cocos2d::Renderer* renderer, const cocos2d::Mat4& transform, uint32_t flags) override;
	virtual void removeAllChildrenWithCleanup(bool cleanup);

	virtual void OnEvent( TEventType msg, CCTouch* touch );

	void SetScrollType(int type) { m_scrollType = type;}

	void SetSpace(int space) { m_space = space; }

	void resetStartPos( void ){ m_startPos = ccp(0,0); m_desPos = m_startPos; maxPt = m_startPos; }

	void	resetEndPos();

	void	setNoncePosX( int x ){m_startPos.x = (float)x; m_desPos = m_startPos; maxPt = m_startPos; }
	void	setNoncePosY( int y ){m_startPos.y = (float)y; m_desPos = m_startPos; maxPt = m_startPos; }

	const Vec2&	getStartPos( void )const	{ return m_startPos; }

	virtual UI*	getTouchUI(int beginX, int beginY, CCTouch* touch)
	{
		return NULL;
	}

	void setInitPos(Vec2 point);
private:
	Vec2 m_startPos;

	Vec2 m_desPos;

	Vec2 m_v;

	Vec2 maxPt;

	int m_scrollType;

	bool m_hasDrag;

	void GetScrollMax();
	void GetEndPos();

	int  m_endSpace;
	int  m_space;
	UI	*	m_pSelectUI;
};
