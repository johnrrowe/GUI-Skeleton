#include "Application.hpp"

#include <exception>
#include <unordered_set>

#include <gtkmm/application.h>
#include <gtkmm/builder.h>
#include <gtkmm/listbox.h>
#include <gtkmm/toolbutton.h>
#include <gtkmm/window.h>


Gtk::ToolButton* connect_toolbutton(
    ThreadSafe::Queue<GUI::Event>& gui_events,
    Glib::RefPtr<Gtk::Builder>& builder, 
    GUI::Widgets widget
)
{
    Gtk::ToolButton* toolbutton = nullptr; 
    builder->get_widget(GUI::widget_name(widget), toolbutton);

    if (toolbutton == nullptr)
        throw std::runtime_error("Could not retrieve " + widget_name(widget) + " from the glade file.");

    toolbutton->signal_clicked().connect(
        [&gui_events, widget] ()
        {
            gui_events.send(GUI::ButtonClicked{ widget });
        }
    );

    return toolbutton;
}


Gtk::ListBox* connect_listbox(
    ThreadSafe::Queue<GUI::Event>& gui_events,
    Glib::RefPtr<Gtk::Builder>& builder, 
    GUI::Widgets widget
)
{
    Gtk::ListBox* listbox = nullptr; 
    builder->get_widget(GUI::widget_name(widget), listbox);

    if (listbox == nullptr)
        throw std::runtime_error("Could not retrieve " + widget_name(widget) + " from the glade file.");

    listbox->signal_selected_rows_changed().connect(
        [&gui_events, listbox, widget] ()
        {
            std::unordered_set<int> selected_rows;

            for (Gtk::ListBoxRow* row : listbox->get_selected_rows())
            {
                selected_rows.insert(row->get_index());
            }

            gui_events.send(GUI::RowsSelected{ widget, std::move(selected_rows) });
        }
    );

    return listbox;
}


Gtk::Window* connect_window(
    ThreadSafe::Queue<GUI::Event>& gui_events,
    Glib::RefPtr<Gtk::Builder>& builder, 
    GUI::Widgets widget
)
{
    Gtk::Window* window = nullptr; 
    builder->get_widget(GUI::widget_name(widget), window);

    if (window == nullptr)
        throw std::runtime_error("Could not retrieve " + widget_name(widget) + " from the glade file.");

    window->signal_delete_event().connect(
        [&gui_events, widget] (GdkEventAny*)
        {
            gui_events.send(GUI::WindowClosed{ widget });
            return false; // propogate the event to other handlers 
        }
    );

    return window;
}


GUI::Application::Application() :
    update_handler([this] (const Update& update) { std::visit(controller, update); })
{
    auto app = Gtk::Application::create();

    if (app.get() == nullptr)
    {
        throw std::runtime_error("Failed to initialize the GTK application");
    }

    Glib::RefPtr<Gtk::Builder> builder { nullptr };

    try 
    {
        builder = Gtk::Builder::create_from_file(glade_file);
    } 
    catch (...) 
    {
        throw std::runtime_error("Failed to load the glade file: " + std::string(glade_file));
    }

    Gtk::Window* main_window = connect_window(events, builder, GUI::Widgets::MAIN_WINDOW); 
    Gtk::ListBox* process_list = connect_listbox(events, builder, Widgets::PROCESS_LIST); 

    connect_toolbutton(events, builder, Widgets::KILL_SELECTED_PROCESS);
    connect_toolbutton(events, builder, Widgets::START_NEW_PROCESS);

    controller = { process_list }; 

    main_loop = [app, main_window] ()
    {
        return app->run(*main_window);
    };
}


int GUI::Application::run() const
{
    return main_loop(); 
}


GUI::Dispatcher<GUI::Update> GUI::Application::make_dispatcher()
{
    return update_handler.make_dispatcher();
}


ThreadSafe::Queue<GUI::Event>& GUI::Application::event_queue() 
{
    return events;
}

