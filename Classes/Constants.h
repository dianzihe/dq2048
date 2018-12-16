#pragma once
#include <array>
#include <string>

#define NS_GAME_BEGIN    namespace Game {
#define NS_GAME_END      }

namespace Consts {
    const int ROWS = 4;
    const int COLS = ROWS;
    const int ROWSCOLS = ROWS * COLS;
    
    typedef int ArrType[ROWS][COLS];
    
    const int WIN_VALUE = 2048;

    const int CELL_WIDTH  = 100;
    const int CELL_HEIGHT = CELL_WIDTH;
    const int CELL_OFFSET = 10;
    const int FIELD_WIDTH = CELL_WIDTH * ROWS;
    const int FIELD_HEIGHT = CELL_HEIGHT * COLS;
    
    enum class Move { Left, Right, Up, Down };
    enum class ActionTag { Move, Scale };
    
    const int START_CELLS = 2;
    
    const std::array<std::pair<int, int>, 2> VAL_PROBS {
        std::make_pair(2, 90), // probability for 2 is 90%
        std::make_pair(4, 10)  // probability for 4 is 10%
    };
    
    const std::string SAVE_PATH = std::string("save_").append(std::to_string(ROWS)).append("_").append(std::to_string(COLS));
    
    const float MOVE_TIME = 0.15f;
    const float APEAR_TIME = 0.15f;
    
    const std::string guiFont = "fonts/Marker Felt.ttf";
    const std::string gameFont = "fonts/arial.ttf";
    
    const std::string newGameText = "NEW GAME";
    const std::string congtarText = "Congratulations!";
    const std::string looseText = "Wasted!";
    const std::string scoreText = "Score";
    const std::string bestText = "Best";
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    const std::string helpText = "swipe to move cells";
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_MAC)
    const std::string helpText = "use arrows to move cells";
#else
    const std::string helpText = "";
#endif
    
    const bool USE_TEST_DATA = false;
}
