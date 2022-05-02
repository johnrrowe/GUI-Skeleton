#include "Controller.hpp"

#include <string>

#include <gtkmm/box.h>
#include <gtkmm/label.h>
#include <gtkmm/listbox.h>


void GUI::Controller::operator()(const GUI::AddRows& additional_rows)
{        
    for (int i = 0; i < additional_rows.count; i++)
    {
        auto row = Gtk::make_managed<Gtk::Box>();
        row->set_homogeneous();

        auto ID_column = Gtk::make_managed<Gtk::Box>();
        auto process_ID = Gtk::make_managed<Gtk::Label>(std::to_string(i)); 
        process_ID->set_hexpand();
        ID_column->add(*process_ID);
        row->add(*ID_column);

        auto status_column = Gtk::make_managed<Gtk::Box>();
        auto process_status = Gtk::make_managed<Gtk::Label>("GOOD"); 
        process_status->set_hexpand();
        status_column->add(*process_status);
        row->add(*status_column);

        process_list->append(*row);
    }

    process_list->show_all();
}


void GUI::Controller::operator()(const GUI::RemoveSelectedRows& removal)
{
    for (auto* row : process_list->get_selected_rows())
    {
        process_list->remove(*row);
    }
}


GUI::Controller::Controller(Gtk::ListBox* processes) :
    process_list(processes)
{
}
