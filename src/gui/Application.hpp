#pragma once

#include <memory>
#include <string>
#include <variant>


namespace Gtk
{
    class Application;
    class Builder;
    class Window;
};


namespace GUI
{
    struct InitError
    {
        std::string reason;
    };


    class Application
    {
    public:

        static std::variant<GUI::Application, InitError> initialize(const std::string& glade_file_path);

        Application(Gtk::Application* a, Gtk::Window* w);

        Application() = delete;
        Application(Application&&) = default;
        Application& operator=(Application&&) = default;
        Application(const Application&) = delete;
        Application& operator=(const Application&) = delete;
        ~Application();

        int run();

    private:

        Gtk::Window* main_window { nullptr };
        std::unique_ptr<Gtk::Application> app { nullptr };
    };
};