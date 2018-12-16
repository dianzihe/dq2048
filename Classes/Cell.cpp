#include "Cell.hpp"
#include "Constants.h"
#include "cocos2d.h"
#include "RoundedRect.hpp"
#include "Colors.h"

USING_NS_CC;

NS_GAME_BEGIN
bool Cell::init(){
    if (!DrawNode::init()){
        return false;
    }

    _back = RoundedRect::create();
    _back->setPosition(Vec2(-WIDTH/2, -HEIGHT/2));
    _lblVal = Label::createWithTTF("", Consts::gameFont, getFontSize());
    
    addChild(_back, -1);
    addChild(_lblVal, 1);
    
    setScale(0);
    return true;
}

float Cell::getFontSize() {
    return _val < 100 ? 60 : (_val < 1000 ? 50 : 40); // todo: move to consts or calculate
}

const Color4F Cell::getFillColor() {
    return Colors::Cells.at(_val);
}

Color4B Cell::getTextColor() {
    return _val > 4 ? Colors::CellText1 : Colors::CellText2;
}

void Cell::show(int val){
    _val = val;
    
    _lblVal->setString(std::to_string(_val));
    _lblVal->setTextColor(getTextColor());
    _lblVal->setSystemFontSize(getFontSize());
    
    auto sacelAction = ScaleTo::create(Consts::APEAR_TIME, 1.0f);
    auto seq = Sequence::create(DelayTime::create(Consts::APEAR_TIME), sacelAction, nullptr);
    seq->setTag((int)Consts::ActionTag::Scale);
    runAction(seq);
    
    _back->setSizeAndColor(Vec2(WIDTH, HEIGHT), getFillColor());
}

void Cell::up(int newVal){
    _val = newVal;
    
    _lblVal->setString(std::to_string(_val));
    _lblVal->setTextColor(getTextColor());
    _lblVal->setSystemFontSize(getFontSize());
    
    stopAllActionsByTag((int)Consts::ActionTag::Scale);
    auto scaleMax = ScaleTo::create(Consts::MOVE_TIME, 1.2f);
    auto scaleNorm = ScaleTo::create(Consts::MOVE_TIME, 1.0f);
    auto seq = Sequence::create(scaleMax, scaleNorm, nullptr);
    seq->setTag((int)Consts::ActionTag::Scale);
    runAction(seq);
    
    _back->setSizeAndColor(Vec2(WIDTH, HEIGHT), getFillColor());
}

void Cell::win(){
    //todo: some beautifull
}

void Cell::setHistory(std::string& mes) {
    _hist.append(mes);
}

std::string& Cell::getHistory(){
    return _hist;
}

int Cell::val(){
    return _val;
}

NS_GAME_END
