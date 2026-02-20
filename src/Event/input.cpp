#include "Event/input.hpp"
#include "input/InputManager.hpp"
#include "features/zoom.hpp"

void EventInput(Amethyst::InputManager& inputManager) {
    Amethyst::InputAction& zoomKeybind =
        inputManager.RegisterNewInput("zoom", {'C'}, true, Amethyst::KeybindContext::Gameplay);

    zoomKeybind.addButtonDownHandler([](FocusImpact focus, ClientInstance& client) {
        setEnabled(true);
        return Amethyst::InputPassthrough::Consume;
    });

    zoomKeybind.addButtonUpHandler([](FocusImpact focus, ClientInstance& client) {
        setEnabled(false);
        return Amethyst::InputPassthrough::Consume;
    });
}