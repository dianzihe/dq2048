#pragma once
#include "UI.h"


typedef void(*pDrawFun)(UI* ui);

class UIDrawable : public UI
{

public:
	UIDrawable();
	virtual ~UIDrawable();
	virtual void draw(cocos2d::Renderer* renderer, const cocos2d::Mat4& transform, uint32_t flags);
	void SetDrawCallBack(pDrawFun fun);
private:

	pDrawFun m_drawFun;

};