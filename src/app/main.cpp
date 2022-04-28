#include "gui/Application.hpp"

#include <thread>


struct GuiEventHandler // inherit from boost::static_visitor<bool> if using boost::variant 
{
    bool operator()(const GUI::ButtonClicked& button_clicked)
    {
        printf("BUTTON CLICKED %s\n", GUI::widget_name(button_clicked.widget).c_str());
        return false;
    }

    bool operator()(const GUI::RowSelected& selected_row)
    {
        printf("ROW SELECTED %s\n", GUI::widget_name(selected_row.widget).c_str());
        return false;
    }

    bool operator()(const GUI::RowSelectionCleared& list_box)
    {
        printf("ROW SELECTION CLEARED %s\n", GUI::widget_name(list_box.widget).c_str());
        return false;
    }

    bool operator()(const GUI::WindowClosed& window_closed)
    {
        printf("MAIN WINDOW CLOSED\n");
        return window_closed.widget == GUI::Widgets::MAIN_WINDOW;
    }
};


void handle_gui_events(ThreadSafe::Queue<GUI::Event>& gui_events)
{
    bool main_window_closed = false;

    while (!main_window_closed)
    {
        if (const auto event = gui_events.receive())
        {
            // use boost::apply_visitor if using boost::variant
            main_window_closed = std::visit(GuiEventHandler{}, *event);
        }
    }
}


int main()
{
    ThreadSafe::Queue<GUI::Event> gui_events;

    std::jthread event_handler // joins on destruction
    {
        [&gui_events] ()
        {
            handle_gui_events(gui_events);        
        }
    };

    const char* glade_file = "../../../resources/Main.glade";

    const auto gui_init_attempt = GUI::Application::initialize(glade_file, gui_events);

    if (const auto* init_failure = std::get_if<std::string>(&gui_init_attempt)) 
    {
        printf("ERROR :: GUI :: %s\n", init_failure->c_str());
        return -1;
    }
    else
    {
        const auto& gui = std::get<GUI::Application>(gui_init_attempt);
        return gui.run();
    }
}
