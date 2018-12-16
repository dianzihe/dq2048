#pragma once

#include "Constants.h"
#include "RoundedRect.hpp"

NS_GAME_BEGIN
struct FieldBack : public RoundedRect {
    virtual bool init();
    CREATE_FUNC(FieldBack);
};
NS_GAME_END
