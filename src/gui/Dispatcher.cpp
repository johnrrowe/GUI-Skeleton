#include "Dispatcher.hpp"

#include <glibmm/dispatcher.h>


GUI::DispatchContext::DispatchContext(std::function<void()> handler) :
    dispatcher(new Glib::Dispatcher())
{
    dispatcher->connect(std::move(handler));
}


void GUI::DispatchContext::trigger() const
{
    dispatcher->emit();
}


GUI::DispatchContext::DispatchContext() = default;
GUI::DispatchContext::DispatchContext(DispatchContext&&) = default;
GUI::DispatchContext& GUI::DispatchContext::operator=(DispatchContext&&) = default;
GUI::DispatchContext::~DispatchContext() = default;