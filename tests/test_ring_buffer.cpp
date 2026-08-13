#include <cassert>
#include <iostream>

#include <spsc/ring_buffer.hpp>

void test_basic_push_pop()
{
    spsc::RingBuffer<int, 8> queue;

    assert(queue.empty());
    assert(queue.size() == 0);

    assert(queue.push(42));

    assert(!queue.empty());
    assert(queue.size() == 1);

    int value = 0;

    assert(queue.pop(value));
    assert(value == 42);

    assert(queue.empty());
    assert(queue.size() == 0);

    std::cout << "basic_push_pop tests passed\n";
    
}

void test_fifo()
{
    spsc::RingBuffer<int, 8> queue;

    for (int i = 0; i < 7; ++i) {
        assert(queue.push(i));
    }

    int value = 0;
    for (int i = 0; i < 7; ++i) {
        assert(queue.pop(value));
        assert(value == i);
    }

    std::cout << "fifo tests passed\n";
}

void test_empty()
{
    spsc::RingBuffer<int, 8> queue;
    int val=123;
    assert(queue.empty());
    assert(!queue.pop(val)); // Should fail, buffer is empty
    assert(val == 123); // Value should remain unchanged
    std::cout << "empty tests passed\n";
}


void test_full()
{
    spsc::RingBuffer<int, 8> queue;

    for (int i = 0; i < 7; ++i) {
        assert(queue.push(i));
    }

    assert(!queue.push(7)); // Should fail, buffer is full

    std::cout << "full tests passed\n";
}

void test_wraparound()
{
    spsc::RingBuffer<int, 8> queue;

    for (int i = 0; i < 7; ++i) {
        assert(queue.push(i));
    }

    int value = 0;
    for (int i = 0; i < 3; ++i) {
        assert(queue.pop(value));
        assert(value == i);
    }

    for (int i = 7; i < 10; ++i) {
        assert(queue.push(i));
    }

    for (int i = 3; i < 10; ++i) {
        assert(queue.pop(value));
        assert(value == i);
    }

    std::cout << "wraparound tests passed\n";
}

void test_size()
{
    spsc::RingBuffer<int, 8> queue;

    assert(queue.size() == 0);

    for (int i = 0; i < 7; ++i) {
        assert(queue.push(i));
        assert(queue.size() == i + 1);
    }

    int value = 0;
    for (int i = 0; i < 7; ++i) {
        assert(queue.pop(value));
        assert(value == i);
        assert(queue.size() == 6 - i);
    }

    std::cout << "size tests passed\n";
}




int main()
{
    test_basic_push_pop();
    test_fifo();
    test_empty();
    test_full();
    test_wraparound();
    test_size();

    std::cout << "All tests passed\n";
    return 0;

}