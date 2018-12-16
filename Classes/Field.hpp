#pragma once

#include "Constants.h"
#include "Data.hpp"
#include "FieldGUI.hpp"

NS_GAME_BEGIN
class Field
{
public:
    Field() = default;
    Field(const Field&) = delete;
    Field operator=(const Field&) = delete;
    
    void init(FieldGUI* fieldGui);
    void restartGame();
    
    void onSwipe(Consts::Move direction);
    
    int getDataAt(int r, int c);

private:
    bool addRandom(int cnt = 1);
    
    void moveCell(CellPos from, CellPos to);
    bool checkJoin(CellPos from, CellPos to, std::vector<int>& merged);
    void addScore(int score);
    
    void onWin(CellPos position);
    void checkLoose();
    void onLoose();
    
    void printData();
    
    Consts::ArrType _data;
    FieldGUI* _fieldGui = nullptr;
    bool _gameOver = false;
    
    int _score = 0;
    int _best = 0;
};
NS_GAME_END
