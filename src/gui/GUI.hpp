#pragma once

#include <string>
#include <optional>


namespace GUI
{
    struct AppError
    {
        std::string reason;
    };

    std::optional<AppError> run(const std::string& glade_file_path) noexcept;
};