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
    function<void()> removeGen(Node* n);
    
    float calOrbDelay(const Vec2& src, const Vec2& dst);
    TaskPtr createOrb(Node* parent, Vec2 src, Vec2 dst, bool delay = true);
    
    TaskPtr createFloatWordText(Node* parent,
                                const std::string & str,
                                Vec2 src,
                                float delayLaunch);
    
    TaskPtr createFloatText(Node* parent,
                            int val,
                            Vec2 src,
                            GemUtils::GemColor color,
                            float delayLaunch);
    
    TaskPtr createTwoPhaseFloatText(Node* parent,
                                    const int phase1Val,
                                    const int phase2Val,
                                    const Vec2 src,
                                    const GemUtils::GemColor color,
                                    const float delay);
    
    TaskPtr createEnergyText(Node* parent, int val, Vec2 src);
    
    Sprite* createComboTextLabel(const int val, const Vec2 src);
    
    //CCFiniteTimeAction* createFadeThenRemove(GemPtr gem, float time);
    FiniteTimeAction* createIconJumpSequence(Vec2 p, float delayLaunch);
    
    Sprite* createShadowGem(GemUtils::GemColor c);
    
    TaskPtr createAttackAnim(Layer* parent, Vec2 p, GemUtils::GemColor c);
    
    FiniteTimeAction* createNumberAnimSeq(float from,
                                            float to,
                                            float duration);
    
    TaskPtr createDelay(Node* parent, float t);
    
    //TaskPtr createPropertyAnim(StatusLabelPtr propertyLabel);
    
    TaskPtr createComboTextFadeRemove(Sprite* comboText, int index);
    
    TaskPtr moveGemAnim(Sprite* gem, Vec2 gridP);
    TaskPtr moveGemAnimSlow(Sprite* gem, Vec2 gridP);
    
    FiniteTimeAction* createShake(float t, Vec2 p, float rmin, float rmax);
    
    Animation* createLoadingCharacter();

    TaskPtr createVictoryAnim(Node* parent, const Vec2& center);
//#define DEFEAT_ANIM_TAG   0x1008
    TaskPtr createDefeatAnim(Node* parent, const Vec2& center);
    TaskPtr createFlyin(Node* node, float duration);
}
