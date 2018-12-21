//
//  AnimPreset.cpp
//  HelloCpp
//
//  Created by Zeyang Li on 12/4/12.
//
//
/*
#include "Board/AnimPreset.h"
#include "Animation/CCCallLambda.h"
#include "Board/Utils.h"
#include "Board/Common.h"
#include "Fonts.h"
#include <boost/bind.hpp>
#include <extensions/utils.h>
*/
#include "AnimPreset.h"
#include "CCCallLambda.h"

namespace PH
{
    function<void()> removeGen(Node* n)
    {
        assert(n != NULL && "parameter is null");
        return [n]() { n->removeFromParentAndCleanup(true); };
    }
    
    static function<void()> showGen(Node* n)
    {
        assert(n != NULL && "parameter is null");
        return [n]() { n->setVisible(true); };
    }
    
    float calOrbDelay(const Vec2& src, const Vec2& dst)
    {
        //        const float speed = 1/1800.0f;
        const float speed = 1/1800.0f;
        
        Vec2 p0(src.x, src.y);
        Vec2 p2(dst.x, dst.y);
        Vec2 dir = p2 - p0;
        
        float degree = M_PI*0.5f;
        
        Vec2 perp(cosf(degree) * dir.x + -sinf(degree) * dir.y,
                   sinf(degree) * dir.x +  cosf(degree) * dir.y);
        if(perp.y < 0.0f)
            perp *= -1.0f;
        perp *= 0.5f;
        
        // compute curve length
        float dis = perp.getAngle() * degree * 2;
        float time = dis * speed;
        
        return time;
    }
    
    TaskPtr createOrb(Node* parent, Vec2 src, Vec2 dst, bool delay)
    {
        Sprite* orb = GemUtils::GetSprite("effects/generic_orb.png");
        orb->setVisible(false);
        orb->setPosition(src);
        
//        Vec2articleSystemQuad* orb = Vec2articleSystemQuad::create("particle/fire_ball.plist");
//        orb->setPosition(src);
//        orb->setVisible(false);
        
        parent->addChild(orb, ORDER_ORB);
        
//        const float speed = 1/1800.0f;
        const float speed = 1/1800.0f;
        
        Vec2 p0(src.x, src.y);
        Vec2 p2(dst.x, dst.y);
        Vec2 dir = p2 - p0;
        
        float degree = M_PI*0.5f;
        
        Vec2 perp(cosf(degree) * dir.x + -sinf(degree) * dir.y,
                   sinf(degree) * dir.x +  cosf(degree) * dir.y);
        if(perp.y < 0.0f)
            perp *= -1.0f;
        perp *= 0.5f;
        
        Vec2 p1 = perp + (p0 + p2)*0.5f;
		perp.normalize();
		perp.getAngle();
        // compute curve length
        float dis = perp.getAngle() * degree * 2;
        float time = dis * speed;
        
        ccBezierConfig bz = ccbz(Vec2(p0.x, p0.y), Vec2(p1.x, p1.y), Vec2(p2.x, p2.y));
        BezierBy* bzMove =  BezierTo::create(time, bz);

//        CCCatmullRomBy* bzMove = CCCatmullRomBy::create(time, cr);
        
        FiniteTimeAction * orbSeq = Sequence::create(CCCallLambda::create([orb](){orb->setVisible(true);}),
                                                         bzMove,
                                                         CCCallLambda::create([orb](){orb->removeFromParentAndCleanup(false);}),
//                                                         CCCallLambda::create([=](){ orb->stopSystem(); }),
                                                         NULL);
        return TaskSequence::make()
        << TaskAnim::make(orb, orbSeq, delay);
    }
    
    TaskPtr createFloatWordText(Node* parent,
                                const std::string & str,
                                Vec2 src,
                                float delay)
    {
        // damage counter animation
        LabelBMFont* text = LabelBMFont::create(str.c_str(), "bmfont/FloatWord_32.fnt");
        text->setOpacity(0);
        // use content scale. BM font is not automatically scaled
        text->setScale(Director::getInstance()->getContentScaleFactor()*1.05f);
        text->setPosition(src);
        
        parent->addChild(text, ORDER_FLOAT);
        
        // random roam destinations
        
        auto prev = Spawn::create(Sequence::create(ScaleBy::create(0.2, Director::getInstance()->getContentScaleFactor()*1.35f), ScaleBy::create(0.2, Director::getInstance()->getContentScaleFactor()*1.35f)->reverse(), NULL),
                                    FadeIn::create(0.5f), NULL);
        auto act = Spawn::create(CCMoveBy::create(1.f, Vec2(0, 100)),
                                   ScaleTo::create(0.3f, Director::getInstance()->getContentScaleFactor()*1.05),
                                   FadeOut::create(1.3f),
                                   NULL);
        
        FiniteTimeAction* floatSeq = Sequence::create(DelayTime::create(delay),
                                                          prev,
                                                          act,
                                                          CCCallLambda::create(removeGen(text)),
                                                          NULL);
        
        
        return TaskSequence::make()
        << createDelay(parent, delay)
        << TaskAnim::make(text, floatSeq, false);
    }
    
    TaskPtr createFloatText(Node* parent,
                            int val,
                            Vec2 src,
                            GemUtils::GemColor color,
                            float delay)
    {
        // damage counter animation
		std::stringstream stream;
		stream << val;
        LabelBMFont* text = LabelBMFont::create(stream.str(), GemUtils::numberFont(color));
        text->setOpacity(0);
        // use content scale. BM font is not automatically scaled
        text->setScale(Director::getInstance()->getContentScaleFactor());
        text->setPosition(src);
        
        parent->addChild(text, ORDER_FLOAT);
        
        // random roam destinations
        Vec2 randomBand = ccRandomBand(30, 40);
        Vec2 roamDst = Vec2(src.x + randomBand.x, src.y + randomBand.y);
        
        FiniteTimeAction* floatSeq = Sequence::create(DelayTime::create(delay),
                                                          FadeIn::create(0.3),
                                                          MoveTo::create(1.0f, roamDst),
                                                          FadeOut::create(0.15),
                                                          CCCallLambda::create(removeGen(text)),
                                                          NULL);
        return TaskAnim::make(text, floatSeq, false);
    }
    
    TaskPtr createEnergyText(Node* parent, int val, Vec2 src)
    {
		std::stringstream stream;
		stream << val;
        std::string s = val > 0.0f ? "+" + stream.str() : stream.str();
        // damage counter animation
        CCLabelBMFont* text = CCLabelBMFont::create(s.c_str(),
                                                    "bmfont/Molot_32_Energy.fnt");
        text->setOpacity(0);
        // use content scale. BM font is not automatically scaled
        text->setScale(Director::getInstance()->getContentScaleFactor()*0.8f);
        text->setPosition(src);
        text->setAnchorPoint(Vec2(1.0, 0.0f));
        
        parent->addChild(text, 999);
        
        FiniteTimeAction* floatSeq = Sequence::create(FadeIn::create(0.1f),
                                                          DelayTime::create(0.8f),
                                                          FadeOut::create(0.1f),
                                                          CCCallLambda::create(removeGen(text)),
                                                          NULL);
        return TaskAnim::make(text, floatSeq, false);
    }
    
    TaskPtr createTwoPhaseFloatText(Node* parent,
                                    const int phase1Val,
                                    const int phase2Val,
                                    const Vec2 src,
                                    const GemUtils::GemColor color,
                                    const float delay)
    {
        // damage counter animation
		std::stringstream stream;
		stream << phase1Val;

        LabelBMFont* text = LabelBMFont::create(stream.str(),
                                                    GemUtils::numberFont(color));
        text->setOpacity(0);
        // use content scale. BM font is not automatically scaled
        text->setScale(Director::getInstance()->getContentScaleFactor());
        text->setPosition(src);
        
        parent->addChild(text, ORDER_FLOAT);
        
        // random roam destinations
		
        Vec2 rd1 = ccRandomBand(25, 35);
        rd1.y = std::abs(rd1.y);
        Vec2 roamDst1 = Vec2(src.x + rd1.x, src.y + rd1.y);
        
        Vec2 rd2 = ccRandomBand(20, 21);
        rd2.y = phase1Val*0.5 > phase2Val ? -std::abs(rd2.y) : std::abs(rd2.y);
        Vec2 roamDst2 = Vec2(roamDst1.x + rd2.x, roamDst1.y + rd2.y);
        
        float scale = 1.0f;
        if(phase2Val > phase1Val)
            scale = 1.35f;
        else if(phase2Val < phase1Val)
            scale = std::max((float)phase2Val / phase1Val, 0.6f);
        
        FiniteTimeAction* floatSeq =
            Sequence::create(DelayTime::create(delay),
                               // phase1 animation
                               FadeIn::create(0.2),
                               MoveTo::create(0.7f, roamDst1),
                               // phase2 animation
                               Spawn::create(MoveTo::create(0.8f, roamDst2),
                                               ScaleTo::create(0.8f, Director::getInstance()->getContentScaleFactor()*scale),
                                               CCCountBMTextNumber::create(phase1Val, phase2Val, 0.4),
                                               NULL),
                               FadeOut::create(0.15),
                               CCCallLambda::create([text](){text->removeFromParentAndCleanup(true);}),
                               NULL);
        
        return TaskAnim::make(text, floatSeq, false);
    }
    
    Sprite* createComboTextLabel(const int val, const Vec2 src)
    {
        Sprite* comboBg = GemUtils::GetSprite("ui/combo_80.png");
        comboBg->setPosition(src);
        comboBg->setAnchorPoint(Vec2(0.5f, 0.8f));
        comboBg->setVisible(false);
		std::stringstream s;
		s << val;
        CCLabelBMFont* comboText = CCLabelBMFont::create(s.str(),
                                                         "bmfont/Combo_36.fnt");
        comboText->setTag(1);
        comboText->setPosition(Vec2(comboBg->getContentSize().width * 0.5f, 34));
        comboText->setAnchorPoint(Vec2(0.5, 0.0));
        comboBg->addChild(comboText);
        
        return comboBg;
    }
        
//    CFiniteTimeAction* createFadeThenRemove(Gem* gem, float time)
//    {
//        FadeOut* fadeOut = FadeOut::create(time);
//        CCCallLambda* remove = CCCallLambda::create([gem](){ gem->root->removeFromParentAndCleanup(true); });
//        FiniteTimeAction* seq = Sequence::create(fadeOut, remove, NULL);
//        return seq;
//    }
    
    FiniteTimeAction* createIconJumpSequence(Vec2 p, float delay)
    {
        FiniteTimeAction* jump = Sequence::create(DelayTime::create(delay),
//                                                      CCEaseElasticInOut::create(CCJumpTo::create(1.0f, p, 50, 1), 1.0f),
//                                                      CCEaseBackInOut::create(CCJumpTo::create(0.4f, p, 60, 1)),
                                                      JumpTo::create(0.4f, p, 60, 1),
                                                      NULL);
        return jump;
    }
        
    Sprite* createShadowGem(GemUtils::GemColor c)
    {
        Sprite* gem = GemUtils::GetSprite(GemUtils::res(c));
        gem->setAnchorPoint(Vec2(0.5f, 0.5f));
        gem->setOpacity(125);
        
        return gem;
    }
    
    TaskPtr createAttackAnim(CCLayer* parent, Vec2 p, GemUtils::GemColor c)
    {
        Animation* anim = AnimationCache::getInstance()->animationByName(GemUtils::attackAnim(c));
        
        assert(anim != NULL && "anim is NULL, animations are not loaded?");
        
        // MUST use Sprite. Node does not work with renderable animations
        Sprite* effect = Sprite::create();
        effect->retain();
        
        p.x += 10;
        
        if(c == GemUtils::Dark)
            p.y -= 20;
        else if(c == GemUtils::Light)
        {
            p.y += 10;
        }
        else if(c == GemUtils::Water)
        {
            p.x += 5;
        }
        
        effect->setPosition(p);
        
        if(c == GemUtils::Fire)
            effect->setScale(1.15);
        else
            effect->setScale(1.3);
        
        TaskSequencePtr seq = TaskSequence::make();
        
        seq->add(TaskLambda::make([=]()
        {
            parent->addChild(effect);
            //ring(GemUtils::attackSound(c));
        }));
        seq->add(TaskAnim::make(effect, CCAnimate::create(anim)));
        seq->add(TaskLambda::make([=]()
        {
            effect->removeFromParentAndCleanup(true);
            effect->release();
        }));
        
        return seq;
    }
    
    FiniteTimeAction* createNumberAnimSeq(float from, float to, float duration)
    {
        duration = std::max(0.1f, duration);
        
        FiniteTimeAction* count = CCCountBMTextNumber::create(from, to, duration);
        
        float scale = Director::getInstance()->getContentScaleFactor() * 1.0f;
        float scaleBig = Director::getInstance()->getContentScaleFactor() * 1.8f;
    
        FiniteTimeAction* scaleUp = ScaleTo::create(duration*0.5, scaleBig);
        FiniteTimeAction* scaleDown = ScaleTo::create(duration*0.5, scale);
        FiniteTimeAction* scaleSeq = Sequence::create(scaleUp,
                                                          scaleDown,
                                                          NULL);
        
        FiniteTimeAction* jump = JumpBy::create(duration, Vec2(0, 0), 15, 1);
        
        return Spawn::create(count, scaleSeq, jump, NULL);
    }
    
    TaskPtr createDelay(Node* parent, float t)
    {
        Node* node = Node::create();
        DelayTime* delay = DelayTime::create(t);
        
        parent->addChild(node, -1000);
        
        return TaskSequence::make(TaskAnim::make(node, delay),
                                  TaskLambda::make([node](){ node->removeFromParentAndCleanup(true); }));
    }
    /*
    TaskPtr createPropertyAnim(StatusLabelPtr propertyLabel)
    {
        Node* l = propertyLabel->getRoot();
        // NOTE: property anim must because it's really easy to overwrite earlier anims
        // in different batches and we get a lost action blocking everything.
        l->stopAllActions();
        l->setScale(1.f);
        
        
        Sprite * medium = GemUtils::GetSprite("fadong_01.png");
        
        l->addChild(medium, -100);
        medium->setOpacity(0);
        
        auto seq = Sequence::create(ScaleTo::create(0.35f, 1.3f),
                                      ScaleTo::create(0.35f, 1.0f),
                                      NULL);
        

        return
        TaskSequence::make()
        //<< TaskAnim::make(medium, act, false)
        << TaskLambda::make([=]()
        {
            auto anim = CCAnimate::create(AnimationCache::sharedAnimationCache()->animationByName("skill_trigger"));
            medium->setOpacity(175);
            medium->runAction(Sequence::create(anim,
                                                 CCCallLambda::create([=]()
                                                 {
                                                     medium->removeFromParent();
                                                 }), NULL));
        })
        << TaskAnim::make(l, seq, false);
    }
    */
    TaskPtr createComboTextFadeRemove(Sprite* comboText, int index)
    {
        /*
        auto batch = TaskBatch::make();
        
        float base = 1.3f + ((float)index)*0.12f;
        base = std::min(1.6f, base);
        
        auto prev = Spawn::create(CCEaseElasticOut::create(ScaleTo::create(0.5f, base+0.2f)),
                                    FadeIn::create(0.5f), NULL);
        auto mid = Spawn::create(CCMoveBy::create(0.5f, Vec2(0, 50)),
                                   ScaleTo::create(0.3f, 1.3f),
                                   FadeOut::create(0.8f),
                                   NULL);
        
        FiniteTimeAction* act = Sequence::create(prev,
                                                     mid,
                                                     CCCallLambda::create([comboText]()
                                                     {
                                                        comboText->removeFromParentAndCleanup(true);
                                                     }),
                                                     NULL);
        batch << TaskAnim::make(comboText, act, false);
        
        
        Node * parent = comboText->getParent();
        Sprite * num = (Sprite*)comboText->getChildByTag(1);
        if( parent != NULL && num != NULL )
        {
            auto seq = TaskSequence::make();
            
            auto num_act = Sequence::create(DelayTime::create(0.5f),
                                              FadeOut::create(1.3f),
                                              NULL);
            seq << TaskAnim::make(parent, num_act, false);
            
            batch << seq;
        }
        
        
        return batch;
        */
        auto act = Sequence::create(CCCallLambda::create(
                                      [=]()
                                      {
                                          comboText->getParent()->reorderChild(comboText, ORDER_FLOAT);
                                      }),
										Sequence::create(ScaleBy::create(0.2, 1.3), ScaleBy::create(0.2, 1.3)->reverse(), NULL),
                                      Spawn::create(ScaleTo::create(0.15, 1.5),
                                                      FadeOut::create(0.15),
                                                      NULL),
                                      CCCallLambda::create([comboText]()
                                      {
                                          comboText->removeFromParentAndCleanup(true);
                                      }),
                                      NULL);
        return TaskAnim::make(comboText, act);
    }
    
    TaskPtr moveGemAnimSlow(Sprite* sprite, Vec2 gridP)
    {
        const int GEM_MOVE_TAG = 0x1011;
        
        TaskSequencePtr seq = TaskSequence::make();
        FiniteTimeAction* moveTo = CCEaseElasticOut::create(MoveTo::create(0.6f,
                                                                               g2w_center(gridP)),
                                                              1.2f);
    
        FiniteTimeAction* breath = Sequence::create(CCFadeTo::create(0.2, 190),
                                                        CCFadeTo::create(0.2, 255),
                                                        NULL);
        FiniteTimeAction* both = Spawn::create(moveTo, breath, NULL);

        both->setTag(GEM_MOVE_TAG);
        
        seq << TaskLambda::make([=]()
        {
            CCAction* x = sprite->getActionByTag(GEM_MOVE_TAG);
            if(x)
                sprite->stopAction(x);
        });
        seq << TaskAnim::make(sprite, both, false);
        return seq;
    }
    
    TaskPtr moveGemAnim(Sprite* sprite, Vec2 gridP)
    {
        const int GEM_MOVE_TAG = 0x1011;
        
        TaskSequencePtr seq = TaskSequence::make();
        FiniteTimeAction* moveTo = CCEaseElasticOut::create(MoveTo::create(0.2f,
                                                                               g2w_center(gridP)),
                                                              1.2f);
        
        moveTo->setTag(GEM_MOVE_TAG);
        seq << TaskLambda::make([=]()
                                {
                                    CCAction* x = sprite->getActionByTag(GEM_MOVE_TAG);
                                    if(x)
                                        sprite->stopAction(x);
                                });
        seq << TaskAnim::make(sprite, moveTo, false);
        return seq;
    }
    
    FiniteTimeAction* createShake(float t, Vec2 p, float rmin, float rmax)
    {
		/*
        auto rx = bind(randf(), rmin, rmax);
        
        return Sequence::create(MoveTo::create(t*0.5f, Vec2(p.x+rx.x, p.y+rx())),
                                                    MoveTo::create(t, Vec2(p.x-rx(), p.y-rx())),
                                                    MoveTo::create(t, Vec2(p.x+rx(), p.y+rx())),
                                                    MoveTo::create(t, Vec2(p.x-rx(), p.y-rx())),
                                                    MoveTo::create(t, Vec2(p.x+rx(), p.y+rx())),
                                                    MoveTo::create(t, Vec2(p.x-rx(), p.y-rx())),
                                                    MoveTo::create(t, Vec2(p.x+rx(), p.y+rx())),
                                                    MoveTo::create(t, Vec2(p.x-rx(), p.y-rx())),
                                                    MoveTo::create(t*0.5f, Vec2(p.x, p.y)),
                                                    NULL);
        */
		return NULL;
    }
    
    Animation* createLoadingCharacter()
    {
        Animation* anim = AnimationCache::getInstance()->getAnimation("loading_anim");
        if(anim == NULL)
        {
            anim = Animation::create();
            
            int numFrames = 4;
            for(int i=1; i<=numFrames; ++i)
            {
                char name[128];
                sprintf(name, "loading_anim%d.png", i);
                SpriteFrame* frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(name);
                
                assert(frame != NULL && "cannot find frames");
                
                anim->addSpriteFrame(frame);
            }
            
            anim->setDelayPerUnit(0.18);
            
            AnimationCache::getInstance()->addAnimation(anim, "loading_anim");
        }
        
        return anim;
    }
    
    
    TaskPtr createVictoryAnim(Node* parent, const Vec2& p)
    {
        Sprite* brass = GemUtils::GetSprite("ui/shengli_wenzi.png");
        brass->setPosition(Vec2(220, 100));
        
        Sprite* swordLeft = GemUtils::GetSprite("ui/baojian_faguang.png");
        swordLeft->setAnchorPoint(Vec2(0.5, 0.01));
        swordLeft->setPosition(Vec2(88, 0));
        swordLeft->setOpacity(0);
        
        Sprite* swordRight = GemUtils::GetSprite("ui/baojian_faguang.png");
        swordRight->setAnchorPoint(Vec2(0.5, 0.01));
        swordRight->setPosition(Vec2(440-88, 0));
        swordRight->setOpacity(0);
        
        Sprite* ribbon = GemUtils::GetSprite("ui/shengli_sidai.png");
        //    ribbon->setPosition(Vec2(320, 800));
        ribbon->setPosition(p);
        
        ribbon->addChild(swordLeft);
        ribbon->addChild(swordRight);
        ribbon->addChild(brass);
        
        ribbon->setVisible(false);
        ribbon->setScale(0.0f);
        
        parent->addChild(ribbon, ORDER_VICTORY);
        
        TaskSequencePtr seq = TaskSequence::make();
        
        seq << TaskSound::make("sound/v/victory.mp3");
        seq << TaskAnim::make(ribbon, Sequence::create(Show::create(),
                                                         EaseExponentialOut::create(ScaleTo::create(0.5, 1.0)),
                                                         NULL));
        
        TaskBatchPtr batch = TaskBatch::make();
        batch << TaskAnim::make(swordLeft, Spawn::create(FadeIn::create(0.1),
                                                           RotateBy::create(0.3, 45),
                                                           NULL));
        batch << TaskAnim::make(swordRight, Spawn::create(FadeIn::create(0.1),
                                                            RotateBy::create(0.3, -45),
                                                            NULL));
        seq << batch;
        return seq;
    }
    
    
    TaskPtr createDefeatAnim(Node* parent, const Vec2& p)
    {
        Sprite* brass = GemUtils::GetSprite("ui/baibei_wenzi.png");
        brass->setPosition(Vec2(220, 100));
        
        Sprite* swordLeft = GemUtils::GetSprite("ui/baojian_faguang.png");
        swordLeft->setAnchorPoint(Vec2(0.5, 0.99));
        swordLeft->setPosition(Vec2(88, 0));
        swordLeft->setScaleY(-1);
        swordLeft->setRotation(45);
        swordLeft->setOpacity(0);
        
        Sprite* swordRight = GemUtils::GetSprite("ui/baojian_faguang.png");
        swordRight->setAnchorPoint(Vec2(0.5, 0.99));
        swordRight->setPosition(Vec2(440-88, 0));
        swordRight->setScaleY(-1);
        swordRight->setRotation(-45);
        swordRight->setOpacity(0);
        
        Sprite* ribbon = GemUtils::GetSprite("ui/baibei_sidai.png");
//        ribbon->setPosition(Vec2(320, 800));
        ribbon->setPosition(p);
        
        ribbon->addChild(swordLeft);
        ribbon->addChild(swordRight);
        ribbon->addChild(brass);
        
        ribbon->setVisible(false);
        
        ribbon->setScale(0.0f);
        ribbon->setTag(1008);
        
        parent->addChild(ribbon, ORDER_VICTORY);
        
        TaskSequencePtr seq = TaskSequence::make();
        
        seq << TaskSound::make("sound/v/defeat.mp3");
        seq << TaskAnim::make(ribbon, Sequence::create(Show::create(),
                                                         EaseExponentialOut::create(ScaleTo::create(0.5, 1.0)),
                                                         NULL));
        
        TaskBatchPtr batch = TaskBatch::make();
        batch << TaskAnim::make(swordLeft, Spawn::create(FadeIn::create(0.2),
                                                           RotateBy::create(0.3, 35),
                                                           NULL));
        batch << TaskAnim::make(swordRight, Spawn::create(FadeIn::create(0.3),
                                                            RotateBy::create(0.3, -35),
                                                            NULL));
        seq << batch;
        return seq;    
    }
    
    TaskPtr createFlyin(Node* node, float duration)
    {
        Vec2 src(640 + node->getContentSize().width * node->getAnchorPoint().x, 480);
        Vec2 dst(  0 - node->getContentSize().width * node->getAnchorPoint().x, 480);
        
        node->setPosition(src);
        
        FiniteTimeAction* flyin =
            Sequence::create(CCEaseElasticOut::create(MoveTo::create(duration, Vec2(320, 480)), 0.8f),
                               CCEaseElasticIn::create(MoveTo::create(duration, dst), 0.8f),
                               NULL);
        return TaskAnim::make(node, flyin);
    }
}