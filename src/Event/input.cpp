#include "Event/input.hpp"

void EventInput(Amethyst::InputManager& inputManager) {
    Amethyst::InputAction& zoomKeybind = inputManager.RegisterNewInput("zoom", {'C'}, true, Amethyst::KeybindContext::Gameplay);
    zoomKeybind.addButtonDownHandler([](FocusImpact focus, ClientInstance& client) {
        //enabledStart = std::chrono::system_clock::now();
        setEnabled(true);
        return Amethyst::InputPassthrough::Consume;
    });

    zoomKeybind.addButtonUpHandler([](FocusImpact focus, ClientInstance& client) {
        //enabledStart = std::chrono::system_clock::now();
        setEnabled(false);
        return Amethyst::InputPassthrough::Consume;
    });
};