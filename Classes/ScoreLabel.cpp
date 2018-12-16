#include "ScoreLabel.hpp"
#include "RoundedRect.hpp"
#include "Colors.h"

USING_NS_CC;

NS_GAME_BEGIN

bool ScoreLabel::init()
{
    if (!DrawNode::init()){
        return false;
    }
    
    auto btnW = 150, btnH = 90, fntSize = 35;
    auto back = RoundedRect::create();
    back->setPosition(Vec2::ZERO);
    back->setSizeAndColor(Vec2(btnW, btnH), Colors::Back1);
    addChild(back);
    
    _lblTitle = Label::createWithTTF("", Consts::guiFont, fntSize);
    _lblTitle->setTextColor(Colors::ScoreLableText);
    _lblTitle->setPosition(Vec2(btnW/2, btnH/2 + (fntSize + 5)/2));
    addChild(_lblTitle);
    
    _lblScore = Label::createWithTTF("0", Consts::guiFont, fntSize);
    _lblScore->setTextColor(Color4B::WHITE);
    _lblScore->setPosition(Vec2(btnW/2, btnH/2 - (fntSize + 5)/2));
    addChild(_lblScore);
    
    setContentSize(Size(btnW, btnH));
    return true;
}

void ScoreLabel::setTitle(const std::string& title) {
    _lblTitle->setString(title);
}

void ScoreLabel::setScore(int newVal) {
    _lblScore->setString(std::to_string(newVal));
}

NS_GAME_END
