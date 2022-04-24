#include "gui/Application.hpp"


int main(int, char**)
{
    auto gui_init_attempt = GUI::Application::initialize("../../../resources/Main.glade");

    if (const auto* init_failure = std::get_if<GUI::InitError>(&gui_init_attempt)) 
    {
        printf("ERROR :: GUI :: %s\n", init_failure->reason.c_str());
        return -1;
    }

    auto& gui = std::get<GUI::Application>(gui_init_attempt);

    return gui.run();
}
