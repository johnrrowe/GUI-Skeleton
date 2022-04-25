#include "Widgets.hpp"


std::string GUI::widget_name(Widgets widget)
{
    switch (widget)
    {
    case Widgets::KILL_SELECTED:
        return "toolbutton_kill_selected";

    case Widgets::PROCESS_LIST:
        return "listbox_processes";

    case Widgets::START_NEW_PROCESS:
        return "toolbutton_start_new";

    default:
        return "unknown_widget";
    }
}