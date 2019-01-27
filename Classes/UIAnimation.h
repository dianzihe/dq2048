#pragma once
#include "UI.h"
//#include "Animation.h"

#define  Item_Animation_Start 25 
class UIAnimation : public UI
{
public:
	UIAnimation();
	virtual ~UIAnimation();

	bool init() override;

	//virtual void visit(cocos2d::Renderer *renderer, const cocos2d::Mat4& parentTransform, uint32_t parentFlags) override;
	virtual void draw(cocos2d::Renderer* renderer, const cocos2d::Mat4& transform, uint32_t flags) override;


	void  setAnimationInfo(int id, ACTORTYPE type, int index, float fScale=1, int loop=-1, int sex=0, int equipLevel=1);//等比缩放

	//void  setAnimationInfo(int id, ACTORTYPE type, int index, float fScaleX=1, float fScaleY=1, int loop=-1, int sex=0, int equipLevel=1);//自由缩放

	Animation *getAnimation() { return m_pAnimation; }
	void  setAnimScaleX(float scaleX) { /*m_pAnimation->setScaleX(scaleX); */}
	void  setAnimScaleY(float scaleY) { /*m_pAnimation->setScaleY(scaleY); */}
private:
	Animation *m_pAnimation;
};