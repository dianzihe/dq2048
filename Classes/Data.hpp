#pragma once
#include "Constants.h"

NS_GAME_BEGIN
struct CellPos {
    explicit CellPos(int row = 0, int col = 0) : Row(row), Col(col) { };
    
    std::string& desc();
    int Row = 0, Col = 0;
};

struct Data {
    static int  getSavedData(Consts::ArrType&);
    static void saveData(const Consts::ArrType&, int score);
    static void clearSave();
    
    static int  getBest();
    static void saveBest(int);
};
NS_GAME_END
