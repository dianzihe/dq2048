#include "UIAnimation.h"
//#include "UIBatchRenderer.h"

UIAnimation::UIAnimation()
{
}

UIAnimation::~UIAnimation()
{
}

bool UIAnimation::init()
{
	/*
	m_pAnimation = new Animation();
	m_pAnimation->autorelease();
	m_pAnimation->setPosition(ccp(m_width/2, m_height/2));
	addChild(m_pAnimation);
	*/
	return true;
}

void UIAnimation::draw(cocos2d::Renderer* renderer, const cocos2d::Mat4& transform, uint32_t flags)
{
#ifdef  UI_BATCH_RENDERER
	//UIBatchRenderer::instance()->flush();
#endif
	//UI::draw();
}

void  UIAnimation::setAnimationInfo(int id, ACTORTYPE type, int index, float fScale, int loop, int sex, int equipLevel)
{
	/*
	m_pAnimation->LoadASprite(id, type, sex, equipLevel);
	if ( m_pAnimation->GetSprite() && index >= m_pAnimation->GetSprite()->GetAnimNumber())
	{
		return;
	}
	m_pAnimation->setAnim(index, 0, loop);
	m_pAnimation->setScale(fScale);
	*/
}
