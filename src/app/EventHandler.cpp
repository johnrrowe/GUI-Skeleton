#include "EventHandler.hpp"


std::optional<GUI::Update> EventHandler::operator()(const GUI::ButtonClicked& button_clicked)
{
    switch (button_clicked.widget)
    {
    case GUI::Widgets::START_NEW_PROCESS:
        return GUI::AddRows{ .count = 1 }; 

    case GUI::Widgets::KILL_SELECTED_PROCESS:
        return GUI::RemoveSelectedRows{}; 

    default:
        return std::nullopt; // if using boost::optional, return boost::none 
    }
}


std::optional<GUI::Update> EventHandler::operator()(const GUI::RowsSelected& selection)
{
    selected_rows = selection.rows;
    return std::nullopt;
}


std::optional<GUI::Update> EventHandler::operator()(const GUI::WindowClosed& window_closed)
{
    shutdown = window_closed.widget == GUI::Widgets::MAIN_WINDOW;
    return std::nullopt;
}

