#pragma once

#include "mc/client/gui/controls/UIControl.h"

class VisualTree {
public:
    std::byte padding[8];
    UIControl* mRootControlName;
};