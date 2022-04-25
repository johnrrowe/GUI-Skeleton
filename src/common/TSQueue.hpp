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

    template <typename T>
    void send(T&& message)
    {
        std::lock_guard<std::mutex> lock { queueMutex };
        queue.push(std::forward<T>(message));
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

private:

    std::mutex queueMutex;
    std::queue<Message> queue;
};
}