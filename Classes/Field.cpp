#include "Field.hpp"
#include "FieldGUI.hpp"
#include "Data.hpp"
#include <iostream>

USING_NS_CC;

NS_GAME_BEGIN
void Field::onWin(CellPos position){
    _gameOver = true;
    _fieldGui->onWin(position);
    Data::clearSave();
}

void Field::onLoose(){
    _gameOver = true;
    _fieldGui->onLoose();
    Data::clearSave();
}

void Field::init(FieldGUI* fieldGui) {
    _fieldGui = fieldGui;
    
    auto savedScore = Data::getSavedData(_data);
    if (savedScore > 0) {
        for (int r = 0; r < Consts::ROWS; ++r)
            for (int c = 0; c < Consts::COLS; ++c)
                if (_data[r][c] > 0) _fieldGui->addCell(_data[r][c], CellPos(r, c));
        
        _score = savedScore;
        _fieldGui->setScore(savedScore);
        printData();
    }
    else {
        restartGame();
    }
    
    _best = Data::getBest();
    _fieldGui->setBest(_best);
}

void Field::restartGame() {
    _gameOver = false;
    _fieldGui->clearAll();
    Data::clearSave();
    _score = 0;
    _fieldGui->setScore(_score);
    
    for (int c = 0; c < Consts::COLS; ++c)
        for (int r = 0; r < Consts::ROWS; ++r)
            _data[r][c] = 0;
    
    addRandom(Consts::START_CELLS);
}

bool Field::addRandom(int cnt){
    std::vector<int> idxs; // массив доступных индексов
    std::cout << "avail cell: ";
    for (int r = 0; r < Consts::ROWS; ++r)
        for (int c = 0; c < Consts::COLS; ++c)
            if (_data[r][c] == 0) {
                idxs.push_back(r * Consts::COLS + c);
                std::cout << "[" << r << "," << c << "]=" << (r * Consts::COLS + c) << " ";
            }
    std::cout << std::endl;
    
    for (int i = 0; i < cnt; ++i) {
        if (idxs.size() == 0) break;
        
        int rnd = RandomHelper::random_int(0, (int)idxs.size() - 1);
        int rndInd = idxs[rnd];
        idxs.erase(idxs.begin() + rnd); // чтобы не выбрать этот индекс еще раз, удаляем его из массива доступных индексов
        
        int rndVal = RandomHelper::random_int(1, 100);
        int curSum = 0;
        for (auto& cur: Consts::VAL_PROBS) {
            curSum += curSum + cur.second;
            if (rndVal <= curSum) {
                int r = rndInd / Consts::COLS;
                int c = rndInd % Consts::COLS;
                log("add random to: [%d,%d] = %d", r, c, cur.first);
                _data[r][c] = cur.first;
                _fieldGui->addCell(_data[r][c], CellPos(r, c));
                break;
            }
        }
    }
    
    printData();
    return idxs.size() > 0;
}

void Field::moveCell(CellPos from, CellPos to) {
    _data[to.Row][to.Col] = _data[from.Row][from.Col];
    _data[from.Row][from.Col] = 0;
    _fieldGui->moveCell(from, to);
}

bool Field::checkJoin(CellPos from, CellPos to, std::vector<int>& merged) {
    if (_data[from.Row][from.Col] != _data[to.Row][to.Col]) return false;
    if (merged[to.Row * Consts::COLS + to.Col] != 0) return false;
    
    _data[from.Row][from.Col] = 0;
    _data[to.Row][to.Col] *= 2;
    merged[to.Row * Consts::COLS + to.Col] = 1;
    
    _fieldGui->removeCell(from, to);
    _fieldGui->joinCell(_data[to.Row][to.Col], to);
    
    addScore(_data[to.Row][to.Col]);
    
    if (_data[to.Row][to.Col] == Consts::WIN_VALUE) {
        onWin(to);
    }
    return true;
}

void Field::addScore(int score){
    _score += score;
    if (_score > _best) {
        _best = _score;
        _fieldGui->setBest(_best);
        Data::saveBest(_best);
    }
    _fieldGui->setScore(_score);
}

void Field::onSwipe(Consts::Move dir) {
    if (_gameOver) return;
    
    bool revers = (dir == Consts::Move::Right || dir == Consts::Move::Down);
    bool horizontalMove = (dir == Consts::Move::Right || dir == Consts::Move::Left);
    auto convertRow = [revers, horizontalMove](int i, int r) {
        if (horizontalMove) return r;
        return revers ? Consts::ROWS - i - 1 : i;
    };
    auto convertCol = [revers, horizontalMove](int i, int c) {
        if (!horizontalMove) return c;
        return revers ? Consts::COLS - i - 1 : i;
    };
    int prev = revers ? 1 : -1;
    auto strDir = horizontalMove ? (revers ? "right": "left") : (revers ? "down": "up");
    log("move %s%s", strDir , revers ? " (revers)" : "");
    
    std::vector<int> merged(Consts::ROWSCOLS);
    
    auto jMax = horizontalMove ? Consts::ROWS : Consts::COLS;
    auto iMax = horizontalMove ? Consts::COLS : Consts::ROWS;

    bool wasAction = false;
    for (int j = 0; j < jMax; ++j) {
        int idxEmpty = -1;
        for (int i = 0; i < iMax; ++i) {
            int r = convertRow(i, j);
            int c = convertCol(i, j);
            if (_data[r][c] > 0) { // в ячейке есть число
                if (idxEmpty >= 0) { //если есть пустая ячейка, то двигаемся туда
                    int newRow = convertRow(idxEmpty, j);
                    int newCol = convertCol(idxEmpty, j);
                    moveCell(CellPos(r, c), CellPos(newRow, newCol));
                    i = idxEmpty;
                    r = newRow;
                    c = newCol;
                    idxEmpty = -1;
                    wasAction = true;
                }
                if (i > 0){ // если это не крайняя ячейка пробуем ее склеить
                    auto prevCellPos = horizontalMove ? CellPos(r, c + prev) : CellPos(r + prev, c);
                    bool joined = checkJoin(CellPos(r, c), prevCellPos, merged);
                    if (joined) {
                        --i;
                        wasAction = true;
                    }
                }
            }
            else if (idxEmpty == -1) { // сохраняем первую пустую ячейку
                idxEmpty = i;
            }
        }
    }
    
    if (wasAction) {
        bool hasEmptyCells = addRandom();
        if (!hasEmptyCells) checkLoose();
        if (!_gameOver) {
            Data::saveData(_data, _score); // todo: save once on app close
        }
    }
}

void Field::checkLoose() {
    for (int c = 0; c < Consts::COLS; ++c)
        for (int r = 0; r < Consts::ROWS; ++r) {
            if ((r + 1) < Consts::ROWS && _data[r][c] == _data[r + 1][c]) return;
            if ((c + 1) < Consts::COLS && _data[r][c] == _data[r][c + 1]) return;
        }
    
    onLoose();
}

void Field::printData(){
    std::cout << "-----------------------" << std::endl;
    for (int r = 0; r < Consts::ROWS; ++r) {
        for (int c = 0; c < Consts::COLS; ++c)
            std::cout << _data[r][c] << " ";

        std::cout << std::endl;
    }
}

int Field::getDataAt(int r, int c) {
    return _data[r][c];
}
NS_GAME_END
