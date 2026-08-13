#include <atomic>
#include <cassert>
#include <cstddef>
#include <iostream>
#include <thread>

#include <spsc/spsc_queue.hpp>

void test_basic()
{
    spsc::SPSCQueue<int, 8> queue;

    assert(queue.push(42));

    int value = 0;

    assert(queue.pop(value));
    assert(value == 42);
}

void test_fifo()
{
    spsc::SPSCQueue<int, 8> queue;

    assert(queue.push(10));
    assert(queue.push(20));
    assert(queue.push(30));

    int value;

    assert(queue.pop(value));
    assert(value == 10);

    assert(queue.pop(value));
    assert(value == 20);

    assert(queue.pop(value));
    assert(value == 30);
}

void test_empty()
{
    spsc::SPSCQueue<int, 8> queue;

    int value;

    assert(!queue.pop(value));
}

void test_full()
{
    spsc::SPSCQueue<int, 4> queue;

    assert(queue.push(1));
    assert(queue.push(2));
    assert(queue.push(3));

    assert(!queue.push(4));
}

void test_wraparound()
{
    spsc::SPSCQueue<int, 4> queue;

    assert(queue.push(1));
    assert(queue.push(2));
    assert(queue.push(3));

    int value;

    assert(queue.pop(value));
    assert(value == 1);

    assert(queue.pop(value));
    assert(value == 2);

    assert(queue.push(4));
    assert(queue.push(5));

    assert(queue.pop(value));
    assert(value == 3);

    assert(queue.pop(value));
    assert(value == 4);

    assert(queue.pop(value));
    assert(value == 5);
}

void test_concurrent()
{
    constexpr std::size_t N = 10'000'000;

    spsc::SPSCQueue<std::size_t, 1024> queue;

    std::atomic<bool> producer_done{false};

    std::thread producer([&]() {
        for (std::size_t i = 0; i < N; ) {
            if (queue.push(i)) {
                ++i;
            }
        }

        producer_done.store(true, std::memory_order_release);
    });

    std::thread consumer([&]() {
        std::size_t expected = 0;

        while (expected < N) {
            std::size_t value;

            if (queue.pop(value)) {
                assert(value == expected);
                ++expected;
            }
        }
    });

    producer.join();
    consumer.join();

    assert(producer_done.load(std::memory_order_acquire));

    std::cout << "Concurrent test passed\n";
}

int main()
{
    test_basic();
    test_fifo();
    test_empty();
    test_full();
    test_wraparound();
    test_concurrent();
    std::cout << "All SPSC tests passed\n";
}