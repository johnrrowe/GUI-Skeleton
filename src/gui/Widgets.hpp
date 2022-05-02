#pragma once

#include <string>


namespace GUI
{
    enum class Widgets
    {
        MAIN_WINDOW,
        KILL_SELECTED_PROCESS,
        PROCESS_LIST,
        START_NEW_PROCESS,
        NONE,
    };

    // Gets the name of specific widgets, and these names must match the names
    // in the glade file.
    std::string widget_name(Widgets widget);
}