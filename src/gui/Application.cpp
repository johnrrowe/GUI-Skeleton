#include "Application.hpp"

#include <gtkmm/application.h>
#include <gtkmm/builder.h>
#include <gtkmm/toolbutton.h>
#include <gtkmm/listbox.h>
#include <gtkmm/window.h>


template <typename WidgetT>
bool connect_widget(
    Gtk::Builder& builder, 
    GUI::Widgets widget,
    ThreadSafe::Queue<GUI::Event>& queue 
);


template <>
bool connect_widget<Gtk::ToolButton>(
    Gtk::Builder& builder, 
    GUI::Widgets widget,
    ThreadSafe::Queue<GUI::Event>& queue
)
{
    Gtk::ToolButton* toolbutton = nullptr; 
    builder.get_widget(GUI::widget_name(widget), toolbutton);

    if (toolbutton == nullptr)
        return false;

    toolbutton->signal_clicked().connect(
        [&queue, widget] ()
        {
            queue.send(GUI::ButtonClicked{ widget });
        }
    );

    return true;
}


template <>
bool connect_widget<Gtk::ListBox>(
    Gtk::Builder& builder, 
    GUI::Widgets widget,
    ThreadSafe::Queue<GUI::Event>& queue
)
{
    Gtk::ListBox* list_box = nullptr; 
    builder.get_widget(GUI::widget_name(widget), list_box);

    if (list_box == nullptr)
        return false;

    list_box->signal_row_selected().connect(
        [&queue, widget] (Gtk::ListBoxRow* row)
        {
            if (row)
                queue.send(GUI::RowSelected{ widget, row->get_index() });
            else
                queue.send(GUI::RowSelectionCleared{ widget });
        }
    );

    return true;
}


template <>
bool connect_widget<Gtk::Window>(
    Gtk::Builder& builder, 
    GUI::Widgets widget,
    ThreadSafe::Queue<GUI::Event>&
)
{
    Gtk::Window* window = nullptr; 
    builder.get_widget(GUI::widget_name(widget), window);
    return window != nullptr;
}


std::variant<GUI::Application, GUI::InitError> GUI::Application::initialize(
    const std::string& glade_file_path,
    ThreadSafe::Queue<GUI::Event>& gui_events
)
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

    bool connected_all_widgets = true
        && connect_widget<Gtk::ToolButton>(*builder.get(), Widgets::KILL_SELECTED, gui_events)
        && connect_widget<Gtk::ToolButton>(*builder.get(), Widgets::START_NEW_PROCESS, gui_events)
        && connect_widget<Gtk::ListBox>(*builder.get(), Widgets::PROCESS_LIST, gui_events);

    if (!connected_all_widgets)
    {
        return InitError{ "Could not retrieve all widgets" };
    }

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