#include "FieldGUI.hpp"
#include "Constants.h"
#include "cocos2d.h"
#include "Field.hpp"
#include "FieldBack.hpp"
#include "NewGameButton.hpp"
#include <iostream>
#include "Colors.h"

USING_NS_CC;

NS_GAME_BEGIN
bool FieldGUI::init(){
    if (!Node::init()){
        return false;
    }
    // back
    auto back = FieldBack::create();
    back->setPosition(Vec2(-Consts::FIELD_WIDTH / 2 - Consts::CELL_OFFSET / 2,
                           -Consts::FIELD_HEIGHT/ 2 - Consts::CELL_OFFSET / 2));

    //layer for cells
    _cellLayer = Node::create();
    
    //NewGameButton
    int menuOffset = 50;
    auto btn  = NewGameButton::create();
    auto butW = btn->getContentSize().width;
    auto butH = btn->getContentSize().height;
    auto newGameMenuItem = MenuItemSprite::create(btn, nullptr, CC_CALLBACK_1(FieldGUI::menuCallback, this));
    newGameMenuItem->setPosition(Vec2(Consts::FIELD_WIDTH/2  + butW/2 + menuOffset,
                                      Consts::FIELD_HEIGHT/2 - butH/2));
    auto menu = Menu::create(newGameMenuItem, NULL);
    menu->setPosition(Vec2::ZERO);
    
    //score labels
    _scoreCurrent = ScoreLabel::create();
    auto scoreW = _scoreCurrent->getContentSize().width;
    auto scoreH = _scoreCurrent->getContentSize().height;
    _scoreCurrent->setTitle(Consts::scoreText);
    _scoreCurrent->setPosition(Vec2(-Consts::FIELD_WIDTH/2  - scoreW - menuOffset,
                                     Consts::FIELD_HEIGHT/2 - scoreH));
    _scoreBest = ScoreLabel::create();
    _scoreBest->setPosition(Vec2(-Consts::FIELD_WIDTH/2 - scoreW - menuOffset,
                                  Consts::FIELD_HEIGHT/2 - scoreH*2 - menuOffset/2));
    _scoreBest->setTitle(Consts::bestText);
    
    //lblDebug
    _lblDebug = Label::createWithTTF(Consts::helpText, Consts::gameFont, 25);
    _lblDebug->setPosition(Vec2(0, -Consts::FIELD_HEIGHT/2 - _lblDebug->getContentSize().height));
    _lblDebug->setTextColor(Color4B::GRAY);
    
    addChild(back, -1);
    addChild(menu, 0);
    addChild(_scoreCurrent);
    addChild(_scoreBest);
    addChild(_cellLayer, 1);
    addChild(_lblDebug, 1);
    
//    this->scheduleUpdate();
    
    return true;
}

void FieldGUI::showMess(const std::string& mes) {
    auto lbl = Label::createWithTTF(mes, Consts::guiFont, 70);
    lbl->enableShadow();
    lbl->enableGlow(Colors::calcColor4B(0x776e65));
    lbl->setPosition(Vec2::ZERO);
    _cellLayer->addChild(lbl, 2);
    lbl->setScale(0);
    
    auto move_ease = EaseBackOut::create(ScaleTo::create(1.0f, 1.0f));
    lbl->runAction(move_ease);
}

void FieldGUI::onWin(CellPos position){
    showMess(Consts::congtarText);
}

void FieldGUI::onLoose() {
    showMess(Consts::looseText);
}

void FieldGUI::setScore(int score){
    _score = score;
    _scoreCurrent->setScore(_score);
}

void FieldGUI::setBest(int best){
    _best = best;
    _scoreBest->setScore(_best);
}

void FieldGUI::addCell(int value, CellPos position) {
    auto ex_cell = _cellLayer->getChildByTag(getTagValue(position));
    if (ex_cell) {
        log("warning: add but exists");
        ex_cell->removeFromParentAndCleanup(true);
    }
    
    auto cell = Cell::create();
    cell->setTag(getTagValue(position));
    cell->setPosition(posCoords(position));
    cell->show(value);
    cell->setHistory(std::string("cr(").append(position.desc()).append("=").append(std::to_string(value)).append(")"));
    _cellLayer->addChild(cell, 1);
}

void FieldGUI::moveCell(CellPos from, CellPos to) {
    auto ex_cell = _cellLayer->getChildByTag(getTagValue(to));
    if (ex_cell) {
        log("warning: move but exists!");
        ex_cell->removeFromParentAndCleanup(true);
    }
    
    auto cell = static_cast<Cell*>(_cellLayer->getChildByTag(getTagValue(from)));
    if (!cell) {
        log("warning: no cell moveCell from %s to %s [tag=%d]", from.desc().c_str(), to.desc().c_str(), getTagValue(from));
        return;
    }
    cell->setHistory(std::string("mv(").append(from.desc()).append("=>").append(to.desc()).append(")"));
    log("move cell %s to %s [tag=%d -> %d]", from.desc().c_str(), to.desc().c_str(), getTagValue(from), getTagValue(to));
    cell->setTag(getTagValue(to));
    cell->stopAllActionsByTag((int)Consts::ActionTag::Move);
    
    auto moveTo = MoveTo::create(Consts::MOVE_TIME, posCoords(to));
    moveTo->setTag((int)Consts::ActionTag::Move);
    cell->runAction(moveTo);
}

void FieldGUI::joinCell(int value, CellPos position) {
    auto cell = static_cast<Cell*>(_cellLayer->getChildByTag(getTagValue(position)));
    if (!cell) {
        log("warning: no cell joinCell position %s value=%d [tag=%d]", position.desc().c_str(), value, getTagValue(position));
        return;
    }
    cell->setHistory(std::string("jn(").append(position.desc()).append("=>").append(std::to_string(value)).append(")"));
    log("join cell %s value=%d [tag=%d]", position.desc().c_str(), value, getTagValue(position));
    cell->up(value);
}

void FieldGUI::removeCell(CellPos pos, CellPos to) {
    auto cell = static_cast<Cell*>(_cellLayer->getChildByTag(getTagValue(pos)));
    if (!cell) {
        log("warning: no cell removeCell pos %s to %s [tag=%d]", pos.desc().c_str(), to.desc().c_str(), getTagValue(pos));
        return;
    }
    cell->setHistory(std::string("rm(").append(pos.desc()).append("=>").append(to.desc()).append(")"));
    log("removeCell pos %s to %s [tag=%d]", pos.desc().c_str(), to.desc().c_str(), getTagValue(pos));
    cell->setTag(-getTagValue(pos) - 1);
    cell->stopAllActions();
    cell->setLocalZOrder(0);
    
    auto rem = CallFunc::create([cell](){
        cell->removeFromParentAndCleanup(true);
    });
    auto moveTo = MoveTo::create(Consts::MOVE_TIME, posCoords(to));
    auto seq = Sequence::create(moveTo, rem, nullptr);
    cell->runAction(seq);
}

void FieldGUI::clearAll() {
    _cellLayer->removeAllChildrenWithCleanup(true);
}

void FieldGUI::menuCallback(Ref* pSender){
    if (_field) _field->restartGame();
}

int FieldGUI::getTagValue(CellPos position) {
    return position.Row * Consts::COLS + position.Col;
}

Vec2 FieldGUI::posCoords(CellPos pos) {
    float toX = startX + pos.Col * Consts::CELL_WIDTH;
    float toY = startY - pos.Row * Consts::CELL_HEIGHT;
    return Vec2(toX, toY);
}

void FieldGUI::debug1() {
    auto childrens = _cellLayer->getChildren();
    int i = 0;
    std::cout << "cells info. cnt " << childrens.size() << std::endl;
    for (auto ch: childrens) {
        Cell* cell = dynamic_cast<Cell*>(ch);
        if (cell){
            auto pos = cell->getPosition();
            auto hist = cell->getHistory();
            std::cout << "tag [" << cell->getTag() << "] coords: " << pos.x << "," << pos.y << " hist=" << hist << std::endl;
        }
        ++i;
    }
}

void FieldGUI::debug2() {
    auto childrens = _cellLayer->getChildren();
    int i = 0;
    for (auto& ch: childrens) {
        auto move = MoveBy::create(1, Vec2(-30,30));
        auto seq = Sequence::create(DelayTime::create(0.5+i*0.5), move, nullptr);
        ch->runAction(seq);
        ++i;
    }
}

// for debug only
void FieldGUI::update(float delta){
    auto size = _cellLayer->getChildren().size();
    auto mes = std::to_string(size);
    if (size > 17) mes.append(" Failed size!");
    bool correct = true;
    for (int r = 0; r < Consts::ROWS && correct; ++r)
        for (int c = 0; c < Consts::COLS; ++c){
            auto dataVal = _field->getDataAt(r, c);
            if (dataVal != 0){
                auto pos = CellPos(r, c);
                auto cell = static_cast<Cell*>(_cellLayer->getChildByTag(getTagValue(pos)));
                if (!cell || cell->val() != dataVal) {
                    log("warning: incorrect: %d %s %d %s %s", cell->val(), "!=", dataVal, " on ", pos.desc().c_str());
                    correct = false;
                    break;
                }
            }
        }
    
    if (!correct) mes.append(" -- inclorrect");
    _lblDebug->setString(mes);
}

NS_GAME_END
