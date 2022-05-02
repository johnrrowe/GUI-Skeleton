#pragma once

#include "Controller.hpp"
#include "Dispatcher.hpp"
#include "Event.hpp"
#include "Update.hpp"
#include "common/TSQueue.hpp"

#include <functional>
#include <string>
#include <variant>


namespace GUI
{
    class Application
    {
    public:

        // Initializes GTK and all the necessary widgets. It is a bug for more than one Application to be created.
        // If the Gtk::Application cannot be created, the GLADE file cannot be opened, or any widgets cannot be
        // constructed, an exception is thrown. 
        Application();
        ~Application() = default;

        Application(Application&&) = delete;
        Application(const Application&) = delete;
        Application& operator=(Application&&) = delete;
        Application& operator=(const Application&) = delete;

        // Executes the GTK::Application's main loop, taking control of the calling thread until the GUI exits.
        int run() const;

        // Creates a dispatcher that allows other threads to send GUI::Updates to the GUI thread.
        Dispatcher<GUI::Update> make_dispatcher();

        // Provides access to the queue that widgets send events on.
        ThreadSafe::Queue<GUI::Event>& event_queue(); 

        static constexpr const char* glade_file = "../../../resources/Main.glade";

    private:

        Application(std::function<int()> main_gtk_loop) :
            main_loop(std::move(main_gtk_loop))
        {}

        // Contains a lambda function that calls Gtk::Application::run.
        std::function<int()> main_loop; 

        // Runs for each GUI::Update that is sent using a Dispatcher<GUI::Update>.
        // Calls GTK functions that modify the GUI.
        Controller controller;

        // Executes the Controller each time a GUI::Update is sent.
        // Used to allows other threads to trigger GUI updates.
        DispatchManager<GUI::Update> update_handler; 

        // Widget signals create events that are sent on this queue.
        ThreadSafe::Queue<GUI::Event> events;
    };
};