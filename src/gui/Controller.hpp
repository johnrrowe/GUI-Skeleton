#pragma once

#include "Update.hpp"


namespace Gtk
{
    class ListBox;
}


namespace GUI
{
    class Controller
    {
    public:

        Controller(Gtk::ListBox* processes);

        Controller() = default;
        Controller(Controller&&) = default;
        Controller(const Controller&) = default;
        Controller& operator=(Controller&&) = default;
        Controller& operator=(const Controller&) = default;

        // Update handling functions. You must have one for each type of update,
        // and each handler must return the same type. Pre C++17, UpdateHandler
        // would need to inherit from boost::static_visitor<void> in order to
        // work with boost::variant.
        void operator()(const GUI::AddRows& additional_rows);
        void operator()(const GUI::RemoveSelectedRows& removal);

    private:

        Gtk::ListBox* process_list { nullptr };
    };
} 