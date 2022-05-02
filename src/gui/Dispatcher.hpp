#pragma once

#include "common/TSQueue.hpp"

#include <functional>
#include <memory>


namespace Glib
{
    // NOTE: Glib::Dispatchers must be created and destroyed in the same threa that GTK runs on.
    // NOTE: Glib::Dispatcher::connect() must be called in the same thread that GTK runs on.
    class Dispatcher;
}


namespace GUI
{
    class DispatchContext 
    {
    public:

        // Wrapper around the Glib::Dispatcher. Executes the provided callback whenever  
        // trigger() is called.
        DispatchContext(std::function<void()> callback);

        DispatchContext();
        DispatchContext(DispatchContext&&);
        DispatchContext& operator=(DispatchContext&&);
        ~DispatchContext();

        DispatchContext(const DispatchContext&) = delete;
        DispatchContext& operator=(const DispatchContext&) = delete;

        void trigger() const;

    private:

        std::unique_ptr<Glib::Dispatcher> dispatcher;
    };


    template <typename Message>
    class Dispatcher
    {
    public:

        // Dispatcher is used to send messages from another thread to a DispatchContext running
        // in the GTK thread. Dispatchers are created from a DispatchManager, which takes care
        // of connecting the ThreadSafe::Queue to the DispatchContext.

        Dispatcher() = default;
        Dispatcher(Dispatcher&&) = default;
        Dispatcher(const Dispatcher&) = default;
        Dispatcher& operator=(Dispatcher&&) = default;
        Dispatcher& operator=(const Dispatcher&) = default;

        template <typename MessageT>
        void send(MessageT&& message) const
        {
            queue->send(std::forward<MessageT>(message));
            context->trigger();
        }

    private:

        template <typename T>
        friend class DispatchManager;

        Dispatcher(
            DispatchContext& ctx, 
            ThreadSafe::Queue<Message>& mqueue 
        ) :
            context(&ctx),
            queue(&mqueue)
        {}

        DispatchContext* context { nullptr };
        ThreadSafe::Queue<Message>* queue { nullptr }; 
    };


    template <typename Message>
    class DispatchManager
    {
    public:

        // DispatchManager bundles a DispatchContext with a ThreadSafe::Queue and acts
        // as a factory for creating Dispatchers.

        Dispatcher<Message> make_dispatcher()
        {
            return Dispatcher<Message>{ context, queue };
        }

        // Initializes a DispatchContext that runs the provided callback every time
        // a Message is sent on the accompanied ThreadSafe::Queue. Dispatchers add
        // messages to the queue and then trigger the DispatchContext to run.  
        template <typename Callback>
        DispatchManager(Callback&& callback)
        {
            context = DispatchContext {
                [this, message_handler = std::forward<Callback>(callback)] () mutable
                {
                    while (auto message = queue.receive())
                        message_handler(std::move(*message));
                }
            };
        }

        DispatchManager() = default;

        // DispatchManagers cannot be copied and moved because ThreadSafe::Queues
        // cannot be copied or moved. However, Dispatcher<Message> can be safely 
        // copied and moved.  
        DispatchManager(DispatchManager&&) = delete;
        DispatchManager(const DispatchManager&) = delete;
        DispatchManager& operator=(DispatchManager&&) = delete;
        DispatchManager& operator=(const DispatchManager&) = delete;

    private:

        DispatchContext context; 
        ThreadSafe::Queue<Message> queue; 
    };
}