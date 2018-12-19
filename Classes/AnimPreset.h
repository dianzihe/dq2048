//
//  AnimPreset.h
//  HelloCpp
//
//  Created by Zeyang Li on 12/4/12.
//
//

#pragma once

#include <cocos2d.h>

#include "Task.h"
#include "GemUtils.h"
//#include "Board/Task.h"
//#include "Board/StatusLabel.h"

namespace PH
{
    // helper functions for creating animation presets.
    function<void()> removeGen(CCNode* n);
    
    float calOrbDelay(const Vec2& src, const Vec2& dst);
    TaskPtr createOrb(CCNode* parent, Vec2 src, Vec2 dst, bool delay = true);
    
    TaskPtr createFloatWordText(CCNode* parent,
                                const std::string & str,
                                Vec2 src,
                                float delayLaunch);
    
    TaskPtr createFloatText(CCNode* parent,
                            int val,
                            Vec2 src,
                            GemUtils::GemColor color,
                            float delayLaunch);
    
    TaskPtr createTwoPhaseFloatText(CCNode* parent,
                                    const int phase1Val,
                                    const int phase2Val,
                                    const Vec2 src,
                                    const GemUtils::GemColor color,
                                    const float delay);
    
    TaskPtr createEnergyText(CCNode* parent, int val, Vec2 src);
    
    CCSprite* createComboTextLabel(const int val, const Vec2 src);
    
    //CCFiniteTimeAction* createFadeThenRemove(GemPtr gem, float time);
    CCFiniteTimeAction* createIconJumpSequence(Vec2 p, float delayLaunch);
    
    CCSprite* createShadowGem(GemUtils::GemColor c);
    
    TaskPtr createAttackAnim(CCLayer* parent, Vec2 p, GemUtils::GemColor c);
    
    CCFiniteTimeAction* createNumberAnimSeq(float from,
                                            float to,
                                            float duration);
    
    TaskPtr createDelay(CCNode* parent, float t);
    
    //TaskPtr createPropertyAnim(StatusLabelPtr propertyLabel);
    
    TaskPtr createComboTextFadeRemove(CCSprite* comboText, int index);
    
    TaskPtr moveGemAnim(CCSprite* gem, Vec2 gridP);
    TaskPtr moveGemAnimSlow(CCSprite* gem, Vec2 gridP);
    
    CCFiniteTimeAction* createShake(float t, Vec2 p, float rmin, float rmax);
    
    CCAnimation* createLoadingCharacter();

    TaskPtr createVictoryAnim(CCNode* parent, const Vec2& center);
#define DEFEAT_ANIM_TAG   0x1008
    TaskPtr createDefeatAnim(CCNode* parent, const Vec2& center);
    TaskPtr createFlyin(CCNode* node, float duration);
}
