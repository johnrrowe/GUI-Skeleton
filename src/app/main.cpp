#include "gui/Application.hpp"

#include <thread>


struct EventHandler
{
    bool operator()(const GUI::ButtonClicked& button_clicked)
    {
        printf("BUTTON CLICKED %s\n", GUI::widget_name(button_clicked.widget).c_str());
        return true;
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
};


void handle_gui_events(ThreadSafe::Queue<GUI::Event>& gui_events);
void handle_gui_events(ThreadSafe::Queue<GUI::Event>& gui_events)
{
    bool button_was_clicked = false;

    while (!button_was_clicked)
    {
        if (const auto event = gui_events.receive())
        {
            button_was_clicked = std::visit(EventHandler{}, *event);
        }
    }
}


int main(int, char**)
{
    ThreadSafe::Queue<GUI::Event> gui_events;

    std::jthread event_handler {
        [&gui_events] ()
        {
            handle_gui_events(gui_events);        
        }
    };

    const char* glade_file = "../../../resources/Main.glade";

    auto gui_init_attempt = GUI::Application::initialize(glade_file, gui_events);

    if (const auto* init_failure = std::get_if<GUI::InitError>(&gui_init_attempt)) 
    {
        printf("ERROR :: GUI :: %s\n", init_failure->reason.c_str());
        return -1;
    }

    auto& gui = std::get<GUI::Application>(gui_init_attempt);

    return gui.run();
}
