#pragma once

#include "Widgets.hpp"
#include "common/TSQueue.hpp"

#include <functional>
#include <string>
#include <variant>


namespace GUI
{
    class Application
    {
    public:

        // Initializes GTK and all the necessary widgets. This should only be called once at the
        // beginning of the program. Then either returns a GUI::Application object or fails with
        // an std::string containing the reason for failure.
        // May fail if the Gtk::Application cannot be created, the GLADE file cannot be opened, or
        // any needed widgets cannot be constructed. 
        // When GTK signals are triggered, a GUI::Event is sent through the gui_events queue.
        static std::variant<Application, std::string> initialize(
            const std::string& glade_file_path,
            ThreadSafe::Queue<GUI::Event>& gui_events
        );

        // Executes the GTK::Application's main loop, taking control of the calling thread until
        // the GUI exits.
        int run() const;

        Application(std::function<int()> main_gtk_loop) :
            main_loop(std::move(main_gtk_loop))
        {}

        Application(Application&&) = default;
        Application& operator=(Application&&) = default;
        ~Application() = default;

        Application() = delete;
        Application(const Application&) = delete;
        Application& operator=(const Application&) = delete;

    private:

        // Contains a lambda function that calls Gtk::Application::run 
        std::function<int()> main_loop;
    };
};