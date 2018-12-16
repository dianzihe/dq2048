#pragma once

#include "Constants.h"
#include "Data.hpp"
#include "Cell.hpp"
#include "ScoreLabel.hpp"

NS_GAME_BEGIN
class Field;

class FieldGUI: public cocos2d::Node
{
public:
    virtual bool init();
    
    void addCell(int value, CellPos position);
    void removeCell(CellPos position, CellPos to);
    void moveCell(CellPos from, CellPos to);
    void joinCell(int value, CellPos pos);
    
    void setScore(int score);
    void setBest(int score);
    
    void onWin(CellPos position);
    void onLoose();

    void clearAll();
    void setField(Field* field){ _field = field; };
    
    CREATE_FUNC(FieldGUI);
    
    void debug1();
    void debug2();
private:
    int getTagValue(CellPos position);
    Vec2 posCoords(CellPos pos);
    void showMess(const std::string& mes);
    void menuCallback(cocos2d::Ref* pSender);
    
    void update(float dt);
    
    Node* _cellLayer = nullptr;
    Field* _field = nullptr;
    
    ScoreLabel* _scoreCurrent = nullptr;
    ScoreLabel* _scoreBest = nullptr;
    
    Label* _lblDebug = nullptr;
    
    int _score = 0;
    int _best = 0;
    
    static constexpr float startX = - Consts::FIELD_WIDTH  / 2 + Consts::CELL_WIDTH  / 2;
    static constexpr float startY = + Consts::FIELD_HEIGHT / 2 - Consts::CELL_HEIGHT / 2;
    
    
};
NS_GAME_END
