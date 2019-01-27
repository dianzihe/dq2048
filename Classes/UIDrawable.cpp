#include "UIDrawable.h"

void UIDrawable::draw(cocos2d::Renderer* renderer, const cocos2d::Mat4& transform, uint32_t flags)
{
	UI::draw(renderer, transform, flags);
	if( m_drawFun )
		m_drawFun(this);
}

void UIDrawable::SetDrawCallBack( pDrawFun fun )
{
	m_drawFun = fun;
}

UIDrawable::UIDrawable():
m_drawFun(NULL)
{

}

UIDrawable::~UIDrawable()
{

}