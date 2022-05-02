#pragma once

#include <mutex>
#include <optional>
#include <queue>


namespace ThreadSafe 
{
template <typename Message>
class Queue
{
public:

    template <typename MessageT>
    void send(MessageT&& message)
    {
        std::lock_guard<std::mutex> lock { queueMutex };
        queue.push(std::forward<MessageT>(message));
    }

    std::optional<Message> receive()
    {
        std::lock_guard<std::mutex> lock { queueMutex };

        if (queue.empty())
        {
            return std::nullopt;
        }
        else
        {
            const Message message = std::move(queue.front()); 
            queue.pop();
            return message;
        }
    }

    Message unsafe_receive()
    {
        std::lock_guard<std::mutex> lock { queueMutex };

        const Message message = std::move(queue.front()); 
        queue.pop();
        return message;
    }

    Queue() = default;

    Queue(Queue&&) = delete;
    Queue(const Queue&) = delete;
    Queue& operator=(Queue&&) = delete;
    Queue& operator=(const Queue&) = delete;

private:

    std::mutex queueMutex;
    std::queue<Message> queue;
};
}