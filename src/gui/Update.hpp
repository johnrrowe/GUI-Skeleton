#pragma once

#include <variant>
    

namespace GUI
{
    struct AddRows
    {
        int count;
    };


    struct RemoveSelectedRows
    {
    };


    using Update = std::variant<
        AddRows,
        RemoveSelectedRows
    >;
}
