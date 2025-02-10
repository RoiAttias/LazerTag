#ifndef GUI_MANAGER_HPP
#define GUI_MANAGER_HPP

#define LUMINAUI_USE_TFT_ESPI
#include "Components/LuminaUI/LuminaUI.hpp"

// Paths to Activities
#include "Activation.hpp"
#include "Dashboard.hpp"
#include "HaveAGoodWeek.hpp"
#include "Story.hpp"

enum GUI_Manager_Activity : uint8_t {
    ACTIVATION,
    DASHBOARD,
    HAVE_A_GOOD_WEEK,
    STORY,
    GUI_Manager_Activity_size
};

Activity * GUI_Manager_Activities[] = {
    activation,
    new Dashboard(),
    new HaveAGoodWeek(),
    new Story()
};

#endif // GUI_MANAGER_HPP