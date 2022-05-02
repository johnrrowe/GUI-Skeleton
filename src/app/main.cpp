#include "gui/Application.hpp"

#include "EventHandler.hpp"

#include <thread>


void run_event_handler(
    ThreadSafe::Queue<GUI::Event>& gui_events,
    GUI::Dispatcher<GUI::Update> dispatcher
)
{
    EventHandler handler;

    while (!handler.is_shutdown())
    {
        while (const auto event = gui_events.receive())
        {
            // you can use boost::apply_visitor if before C++17
            if (auto update = std::visit(handler, *event))
                dispatcher.send(std::move(*update));
        }
    }
}


int main()
{
    GUI::Application gui;

    std::jthread event_loop // joins on destruction
    {
        [&gui] () 
        {
            run_event_handler(gui.event_queue(), gui.make_dispatcher());
        }
    };

    return gui.run();
}
