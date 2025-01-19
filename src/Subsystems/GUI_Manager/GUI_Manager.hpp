#ifndef GUI_MANAGER_HPP
#define GUI_MANAGER_HPP

#define LUMINAUI_USE_TFT_ESPI
#include "Subsystems/TGUI/TGUI.hpp"

// Paths to Activities
#include "Activation.hpp"
#include "Dashboard.hpp"
#include "HaveAGoodWeek.hpp"

enum GUI_Manager_Activity : uint8_t {
    ACTIVATION,
    DASHBOARD,
    HAVE_A_GOOD_WEEK,
    GUI_Manager_Activity_size
};

Activity * GUI_Manager_Activities[] = {
    new Activation(),
    new Dashboard(),
    new HaveAGoodWeek()
};

#endif // GUI_MANAGER_HPP