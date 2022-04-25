#pragma once

#include <string>
#include <variant>


namespace GUI
{
    enum class Widgets
    {
        KILL_SELECTED,
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

    using Event = std::variant<
        ButtonClicked, 
        RowSelected,
        RowSelectionCleared
    >;
}