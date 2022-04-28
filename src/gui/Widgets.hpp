#pragma once

#include <string>
#include <variant>


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

    std::string widget_name(Widgets widget);

    struct ButtonClicked
    {
        Widgets widget { Widgets::NONE };
    };

    struct RowSelected
    {
        Widgets widget { Widgets::NONE };
        int index { 0 };
    };

    struct RowSelectionCleared 
    {
        Widgets widget { Widgets::NONE };
    };

    struct WindowClosed
    {
        Widgets widget { Widgets::NONE };
    };

    using Event = std::variant<
        ButtonClicked, 
        RowSelected,
        RowSelectionCleared,
        WindowClosed
    >;
}