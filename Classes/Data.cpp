#include <stdio.h>
#include <fstream>

#include "Data.hpp"
#include "Constants.h"
#include "cocos2d.h"

USING_NS_CC;

NS_GAME_BEGIN

static std::string getPath() {
    static std::string path = FileUtils::getInstance()->getWritablePath() + Consts::SAVE_PATH;
    return path;
}

void Data::clearSave() {
    auto fileUtils = FileUtils::getInstance();
    fileUtils->removeFile(getPath());
    log("Clear save!");
}

int Data::getSavedData(Consts::ArrType& data) {
    if (Consts::USE_TEST_DATA) {
        Consts::ArrType t = {
            {0, 2, 2, 4},
            {2, 2, 2, 2},
            {0, 0, 0, 1024},
            {0, 0, 1024, 0}
        };

        for (int c = 0; c < Consts::COLS; ++c)
            for (int r = 0; r < Consts::ROWS; ++r)
                data[r][c] = t[r][c];

        return 100;
    }
    
    auto fileUtils = FileUtils::getInstance();
    if (!fileUtils->isFileExist(getPath())) return 0;
    
    auto d = fileUtils->getDataFromFile(getPath());
    auto buf = d.takeBuffer(nullptr);
    for (int i = 0; i < Consts::ROWSCOLS; ++i) {
        int r = i / Consts::COLS;
        int c = i % Consts::COLS;
        int val = ((int*)buf)[i];
        data[r][c] = val;
    }
    free(buf); // так по документаии надо
    log("Read success!");

    auto def = UserDefault::getInstance();
    return def->getIntegerForKey("curScore", 0);
}

void Data::saveData(const Consts::ArrType& arr, int score) {
    auto fileUtils = FileUtils::getInstance();
    std::string path = fileUtils->getWritablePath() + Consts::SAVE_PATH;

    cocos2d::Data savedData;
    savedData.copy((const unsigned char*)&arr, Consts::ROWSCOLS * sizeof(int));
    fileUtils->writeDataToFile(savedData, path);
    log("Write success!");
    
    auto def = UserDefault::getInstance(); //todo: хранить отдельно не очень, одно может сохраниться а другое нет, надо бы переделать
    def->setIntegerForKey("curScore", score);
    def->flush();
}

int Data::getBest() {
    auto def = UserDefault::getInstance();
    return def->getIntegerForKey("bestScore", 0);
}

void Data::saveBest(int score) {
    auto def = UserDefault::getInstance();
    def->setIntegerForKey("bestScore", score);
    def->flush();
}

std::string& CellPos::desc(){
    return std::string("[").append(std::to_string(Row)).append(",").append(std::to_string(Col)).append("]");
}

NS_GAME_END
