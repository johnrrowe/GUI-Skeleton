#include "Application.hpp"

#include <gtkmm/application.h>
#include <gtkmm/builder.h>
#include <gtkmm/toolbutton.h>
#include <gtkmm/listbox.h>
#include <gtkmm/window.h>


std::variant<GUI::Application, GUI::InitError> GUI::Application::initialize(const std::string& glade_file_path)
{
    auto app = Gtk::Application::create();

    if (app.get() == nullptr)
    {
        return InitError{ "Failed to initialize the GTK application" };
    }

    Glib::RefPtr<Gtk::Builder> builder { nullptr };

    try 
    {
        builder = Gtk::Builder::create_from_file(glade_file_path);
    } 
    catch (...) 
    {
        return InitError{ "Failed to load the GLADE file: " + glade_file_path };
    }

    Gtk::Window* main_window = nullptr; 
    const std::string main_window_name = "window_main";
    builder->get_widget(main_window_name, main_window);

    if (!main_window)
    {
        return InitError{ "Could not retrieve the widget: " + main_window_name };
    }

    Gtk::ToolButton* kill_button = nullptr; 
    const std::string kill_button_name = "toolbutton_kill_selected";
    builder->get_widget(kill_button_name, kill_button);

    if (!kill_button)
    {
        return InitError{ "Could not retrieve the widget: " + kill_button_name };
    }


    Gtk::ListBox* process_list = nullptr; 
    const std::string process_list_name = "listbox_processes";
    builder->get_widget(process_list_name, process_list);

    if (!process_list)
    {
        return InitError{ "Could not retrieve the widget: " + process_list_name };
    }

    process_list->signal_row_selected().connect(
        [kill_button] (Gtk::ListBoxRow* row)
        {
            if (row == nullptr)
                kill_button->set_sensitive(false);
            else
                kill_button->set_sensitive(true);
        }
    );

    return std::variant<Application, InitError>{ std::in_place_type<Application>, app.release(), main_window };
}


GUI::Application::Application(
    Gtk::Application* a, 
    Gtk::Window* w
) :
    app{ a },
    main_window{ w }
{}


GUI::Application::~Application() = default;


int GUI::Application::run()
{
    return app->run(*main_window); 
}