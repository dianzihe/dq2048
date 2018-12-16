#include "NewGameButton.hpp"
#include "RoundedRect.hpp"
#include "Colors.h"

USING_NS_CC;

NS_GAME_BEGIN
//todo: сделать универсальный класс
bool NewGameButton::init()
{
    if (!DrawNode::init()){
        return false;
    }
    
    auto btnW = 200, btnH = 60, fntSize = 35;

    auto back = RoundedRect::create();
    back->setPosition(Vec2::ZERO);
    back->setSizeAndColor(Vec2(btnW, btnH), Colors::Back2);
    
    auto btnText = Label::createWithTTF(Consts::newGameText, Consts::guiFont, fntSize);
    btnText->setTextColor(Colors::ButtonText);
    btnText->setPosition(Vec2(btnW/2, btnH/2));
    
    addChild(back);
    addChild(btnText);
    
    setContentSize(Size(btnW, btnH));
    return true;
}

NS_GAME_END
