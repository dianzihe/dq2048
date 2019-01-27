#pragma once
#include "UI.h"

//**********************
//UIScroll  2012.8.28 by yqf
//**********************

enum SCROLL_TYPE
{
	SCROLL_TYPE_X,
	SCROLL_TYPE_Y,
};

class UIScroll : public UI
{
	CC_SYNTHESIZE( SCROLL_TYPE, m_ScrollType, ScrollType );//滚动方向
	CC_SYNTHESIZE( int, m_iMaxNum, MaxNum );//最大数
	CC_SYNTHESIZE( int, m_iCurrentNum, CurrentNum );//当前数
public:
	UIScroll(void);
	virtual ~UIScroll(void);

	bool init() override;

	void Add();
	void Reduce();

	virtual void draw(cocos2d::Renderer* renderer, const cocos2d::Mat4& transform, uint32_t flags);

	void onBarDrag();

	bool isShowAddAndReduce(){ return m_bIsShowAddAndReduce;};
	void setShowAddAndReduce( bool b );

	bool m_isDrag;

private:
	UI   *m_pBarBtn;
	UI   *m_pAddBtn;
	UI   *m_pReduceBtn;
	float m_fAddBtnTime;
	float m_fReduceBtnTime;
	bool m_bIsShowAddAndReduce;
};
