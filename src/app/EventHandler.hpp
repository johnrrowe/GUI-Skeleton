#pragma once

#include "gui/Event.hpp"
#include "gui/Update.hpp"

#include <optional>
#include <unordered_set>


class EventHandler 
{
public:

    // Event handling functions. You must have one for each type of event. 
    // Each handler must return the same type. If the GUI needs to be updated,
    // a GUI::Update will be returned, otherwise std::nullopt is returned.

    // Pre C++17, boost::optional can be in place of std::optional, and boost::variant
    // instead of std::variant. In this case, EventHandler would also need to inherit
    // from boost::static_visitor<boost::optional<GUI::Update>>  
    std::optional<GUI::Update> operator()(const GUI::ButtonClicked& button_clicked);
    std::optional<GUI::Update> operator()(const GUI::RowsSelected& selection);
    std::optional<GUI::Update> operator()(const GUI::WindowClosed& window_closed);

    bool is_shutdown() const
    {
        return shutdown;
    }

private:

    std::unordered_set<int> selected_rows;
    bool shutdown = false;
};