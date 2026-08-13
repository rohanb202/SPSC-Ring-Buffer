#pragma once

#include <array>
#include <cstddef>
#include <atomic>

namespace spsc {

template <typename T, std::size_t Capacity>
class SPSCQueue{
    static_assert(Capacity > 1,
                  "Capacity must be greater than 1");

public:
    bool push(const T& value){
        const std::size_t next_tail = next_index(tail_.load(std::memory_order_relaxed));
        if (next_tail == head_.load(std::memory_order_acquire)) {
            return false; // Buffer is full
        }
        buffer_[tail_.load(std::memory_order_relaxed)] = value;
        tail_.store(next_tail, std::memory_order_release);
        return true;

    }
    
    static constexpr std::size_t next_index(std::size_t index)
    {
        return (index + 1) % Capacity;
    }

    bool pop(T& value){
        const std::size_t head = head_.load(std::memory_order_relaxed);
        if(head == tail_.load(std::memory_order_acquire)) {
            return false; // Buffer is empty
        }
        value = buffer_[head];
        head_.store(next_index(head), std::memory_order_release);
        return true;
    }

    bool empty() const{
        return head_.load(std::memory_order_acquire) == tail_.load(std::memory_order_acquire);
    }

    bool full() const{
        return (tail_.load(std::memory_order_acquire) + 1) % Capacity == head_.load(std::memory_order_acquire);
    }

    std::size_t size() const{
        return (tail_.load(std::memory_order_acquire) - head_.load(std::memory_order_acquire ) + Capacity) % Capacity;
    }

    static constexpr std::size_t capacity() {
        return Capacity;
    }

private:
    std::array<T, Capacity> buffer_{};

    std::atomic<std::size_t> head_ = {0};
    std::atomic<std::size_t> tail_ = {0};
};

} // namespace spsc