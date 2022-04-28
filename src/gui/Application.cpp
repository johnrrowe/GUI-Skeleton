#include "Application.hpp"

#include <gtkmm/application.h>
#include <gtkmm/builder.h>
#include <gtkmm/listbox.h>
#include <gtkmm/toolbutton.h>
#include <gtkmm/window.h>


Gtk::ToolButton* connect_toolbutton(
    Glib::RefPtr<Gtk::Builder>& builder, 
    GUI::Widgets widget,
    ThreadSafe::Queue<GUI::Event>& queue
)
{
    Gtk::ToolButton* toolbutton = nullptr; 
    builder->get_widget(GUI::widget_name(widget), toolbutton);

    if (toolbutton == nullptr)
        return nullptr;

    toolbutton->signal_clicked().connect(
        [&queue, widget] ()
        {
            queue.send(GUI::ButtonClicked{ widget });
        }
    );

    return toolbutton;
}


Gtk::ListBox* connect_listbox(
    Glib::RefPtr<Gtk::Builder>& builder, 
    GUI::Widgets widget,
    ThreadSafe::Queue<GUI::Event>& queue
)
{
    Gtk::ListBox* listbox = nullptr; 
    builder->get_widget(GUI::widget_name(widget), listbox);

    if (listbox == nullptr)
        return nullptr;

    listbox->signal_row_selected().connect(
        [&queue, widget] (Gtk::ListBoxRow* row)
        {
            if (row)
                queue.send(GUI::RowSelected{ widget, row->get_index() });
            else
                queue.send(GUI::RowSelectionCleared{ widget });
        }
    );

    return listbox;
}


Gtk::Window* connect_window(
    Glib::RefPtr<Gtk::Builder>& builder, 
    GUI::Widgets widget,
    ThreadSafe::Queue<GUI::Event>& gui_events
)
{
    Gtk::Window* window = nullptr; 
    builder->get_widget(GUI::widget_name(widget), window);

    if (window == nullptr)
        return nullptr;

    window->signal_delete_event().connect(
        [&gui_events, widget] (GdkEventAny*)
        {
            gui_events.send(GUI::WindowClosed{ widget });
            return false; // propogate the event to other handlers 
        }
    );

    return window;
}


std::variant<GUI::Application, std::string> GUI::Application::initialize(
    const std::string& glade_file_path,
    ThreadSafe::Queue<GUI::Event>& gui_events
)
{
    auto app = Gtk::Application::create();

    if (app.get() == nullptr)
    {
        return "Failed to initialize the GTK application";
    }

    Glib::RefPtr<Gtk::Builder> builder { nullptr };

    try 
    {
        builder = Gtk::Builder::create_from_file(glade_file_path);
    } 
    catch (...) 
    {
        return "Failed to load the GLADE file: " + glade_file_path;
    }

    Gtk::Window* main_window = connect_window(builder, GUI::Widgets::MAIN_WINDOW, gui_events); 

    bool connected_all_widgets = main_window
        && connect_toolbutton(builder, Widgets::KILL_SELECTED_PROCESS, gui_events)
        && connect_toolbutton(builder, Widgets::START_NEW_PROCESS, gui_events)
        && connect_listbox(builder, Widgets::PROCESS_LIST, gui_events);

    if (!connected_all_widgets)
    {
        return "Could not retrieve all widgets";
    }

    return Application{ 
        [app, main_window] ()
        {
            return app->run(*main_window);
        }
    };
}


int GUI::Application::run() const
{
    return main_loop(); 
}