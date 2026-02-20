#pragma once

#include <memory>

#include "ll/api/mod/NativeMod.h"
#include "ll/api/memory/Hook.h"
#include "ll/api/mod/RegisterHelper.h"

#include "mc/client/input/VanillaClientInputMappingFactory.h"
#include "MiniAPI/input/InputManager.hpp"
#include "mc/client/renderer/game/LevelRendererPlayer.h"
#include "mc/deps/input/InputMode.h"
#include "mc/client/options/BaseOptions.h"

#include "Event/input.hpp"

namespace alvinqid {

class MiniAPIMOD {

public:
    static MiniAPIMOD& getInstance();

    MiniAPIMOD() : mSelf(*ll::mod::NativeMod::current()) {}

    [[nodiscard]] ll::mod::NativeMod& getSelf() const { return mSelf; }

    bool load();
    bool enable();
    bool disable();
    
    MiniAPI::InputManager* getInputManager() const {
        return mInputManager.get();
    }

    void setInputManager(std::unique_ptr<MiniAPI::InputManager> mgr) {
        mInputManager = std::move(mgr);
    }
    //bool unload();

private:
    ll::mod::NativeMod& mSelf;
    std::unique_ptr<MiniAPI::InputManager> mInputManager;
};

} // namespace alvinqid