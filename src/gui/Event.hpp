#pragma once

#include "Widgets.hpp"

#include <unordered_set>
#include <variant>


namespace GUI
{
    struct ButtonClicked
    {
        Widgets widget { Widgets::NONE };
    };


    struct RowsSelected
    {
        Widgets widget { Widgets::NONE };
        std::unordered_set<int> rows;
    };


    struct WindowClosed
    {
        Widgets widget { Widgets::NONE };
    };


    using Event = std::variant<
        ButtonClicked, 
        RowsSelected,
        WindowClosed
    >;
}