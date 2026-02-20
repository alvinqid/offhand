#include "Event/input.hpp"
#include "mod/InputManager.hpp"
#include "features/zoom.hpp"

void EventInput(InputManager& inputManager) {
    InputAction& zoomKeybind =
        inputManager.RegisterNewInput("zoom", {'C'}, true, KeybindContext::Gameplay);

    zoomKeybind.addButtonDownHandler([](FocusImpact focus, ClientInstance& client) {
        setEnabled(true);
        return InputPassthrough::Consume;
    });

    zoomKeybind.addButtonUpHandler([](FocusImpact focus, ClientInstance& client) {
        setEnabled(false);
        return InputPassthrough::Consume;
    });
}