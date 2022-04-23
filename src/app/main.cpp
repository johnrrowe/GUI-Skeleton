#include "gui/GUI.hpp"


int main(int, char**)
{
    if (const auto app_error = GUI::run("../../../resources/Main.glade"))
    {
        printf("ERROR :: GUI :: %s\n", app_error->reason.c_str());
        return -1;
    }

    return 0;
}
