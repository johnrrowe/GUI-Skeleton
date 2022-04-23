#include "GUI.hpp"

#include <gtkmm.h>

using namespace GUI;


std::optional<AppError> GUI::run(const std::string& glade_file_path) noexcept
{
    const auto app = Gtk::Application::create();

    if (app.get() == nullptr)
    {
        return AppError{ "Failed to initialize the GTK application" };
    }

    Gtk::Window* main_window = nullptr; 

    try 
    {
        auto builder = Gtk::Builder::create_from_file(glade_file_path);
        builder->get_widget("main_window", main_window);
    } 
    catch (...) 
    {
        return AppError{ "Failed to load the GLADE file: " + glade_file_path };
    }

    if (!main_window)
    {
        return AppError{ "Could not retrieve the main window by the name: main_window" };
    }

    const int app_result = app->run(*main_window); 

    if (app_result != 0)
    {
        return AppError{ "Application exited with error code: " + std::to_string(app_result) };
    }

    return std::nullopt;
}