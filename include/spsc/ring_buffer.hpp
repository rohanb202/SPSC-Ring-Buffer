#pragma once

#include <array>
#include <cstddef>

namespace spsc {

template <typename T, std::size_t Capacity>
class RingBuffer {
    static_assert(Capacity > 1,
                  "Capacity must be greater than 1");

public:
    bool push(const T& value){
        const std::size_t next_tail = (tail_ + 1) % Capacity;
        if (next_tail == head_) {
            return false; // Buffer is full
        }
        buffer_[tail_] = value;
        tail_ = next_tail;
        return true;

    }

    bool pop(T& value){
        if(head_ == tail_) {
            return false; // Buffer is empty
        }
        value = buffer_[head_];
        head_ = (head_ + 1) % Capacity;
        return true;
    }

    bool empty() const{
        return head_ == tail_;
    }

    bool full() const{
        return (tail_ + 1) % Capacity == head_;
    }

    std::size_t size() const{
        return (tail_ - head_ + Capacity) % Capacity;
    }

    static constexpr std::size_t capacity() {
        return Capacity;
    }

private:
    std::array<T, Capacity> buffer_{};

    std::size_t head_ = 0;
    std::size_t tail_ = 0;
};

} // namespace spsc