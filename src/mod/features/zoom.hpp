#pragma once
#include "mod/api/InputManager.hpp"

#include <chrono>

namespace alvinqid {

    class Features {
    public:
    
        static void setEnabled(bool state);
        
        static float getSensitivity(float currentSensitivity);
        
        static float getFov(float currentFov, bool applyEffects);
        
        static void RegisterCustomInputs(InputManager inputManager);
    
    private:
        static bool enabled;
    }
} // namespace alvinqid