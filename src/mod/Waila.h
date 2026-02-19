#pragma once

#include "ll/api/mod/NativeMod.h"

#include "ll/api/memory/Hook.h"
#include "ll/api/mod/RegisterHelper.h"

//InGamePackChanger
#include "mc/client/gui/screens/controllers/GeneralSettingsScreenController.h"

//Waila
#include "header/NinesliceHelper.hpp"
#include "header/VisualTree.h"
#include "mc/client/gui/screens/ScreenView.h"
#include "mc/client/gui/controls/UIRenderContext.h"
#include "mc/client/game/ClientInstanceArguments.h"
#include "mc/client/game/ClientInstance.h"
#include "mc/client/gui/ScreenSizeData.h"
#include "mc/client/gui/GuiData.h"
#include "mc/client/gui/screens/GamepadCursorData.h"
#include "mc/locale/I18n.h"
#include "src/mc/world/phys/HitResult.h"
#include "src/mc/world/level/ILevel.h"
#include "src/mc/world/level/BlockSource.h"
#include "mc/client/renderer/screen/MinecraftUIRenderContext.h"
#include "src/mc/world/level/block/Block.h"
#include "mc/deps/core/string/HashedString.h"
#include "mc/deps/core/string/StringHash.h"

#include <cctype>
#include <algorithm>
#include <memory>
#include <string>

namespace alvinqid {

class Waila {

public:
    static Waila& getInstance();

    Waila() : mSelf(*ll::mod::NativeMod::current()) {}

    [[nodiscard]] ll::mod::NativeMod& getSelf() const { return mSelf; }

    /// @return True if the mod is loaded successfully.
    bool load();

    /// @return True if the mod is enabled successfully.
    bool enable();

    /// @return True if the mod is disabled successfully.
    bool disable();

    // TODO: Implement this method if you need to unload the mod.
    // /// @return True if the mod is unloaded successfully.
    // bool unload();

private:
    ll::mod::NativeMod& mSelf;
    ScreenView* screenView;
    ClientInstance* clientInstance;
    MinecraftUIRenderContext* mcuirc;
};

} // namespace alvinqid