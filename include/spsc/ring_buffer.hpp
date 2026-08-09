#pragma once

#include <array>
#include <cstddef>

namespace spsc {

template <typename T, std::size_t Capacity>
class RingBuffer {
    static_assert(Capacity > 1,
                  "Capacity must be greater than 1");

public:
    bool push(const T& value);

    bool pop(T& value);

    bool empty() const;

    bool full() const;

    std::size_t size() const;

    static constexpr std::size_t capacity() {
        return Capacity;
    }

private:
    std::array<T, Capacity> buffer_{};

    std::size_t head_ = 0;
    std::size_t tail_ = 0;
};

} // namespace spsc