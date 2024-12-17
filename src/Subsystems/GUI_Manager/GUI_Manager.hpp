#ifndef GUI_MANAGER_HPP
#define GUI_MANAGER_HPP

#include "Subsystems/TGUI/TGUI.hpp"

// Paths to Activities
#include "Activation.hpp"

enum GUI_Manager_Activity : uint8_t {
    ACTIVATION,
    GUI_Manager_Activity_size
};

Activity* GUI_Manager_Activities[] = {
    new Activation()
};

#endif // GUI_MANAGER_HPP