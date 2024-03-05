#ifndef CIRCULARQUEUE_H
#define CIRCULARQUEUE_H
#include <iostream>
#include <vector>
#include <type_traits>
#include <stdexcept>

template<typename T>
class CircularQueue {
private:
    std::vector<T> buffer;
    size_t capacity;
    size_t head;
    size_t tail;
    size_t count;

public:
    explicit CircularQueue(size_t size) : capacity(size), head(0), tail(0), count(0) {
        if (size <= 2) {
            throw std::invalid_argument("Size must be greater than 2");
        }
        buffer.resize(size);
    }

    // Make the class uncopyable
    CircularQueue(const CircularQueue&) = delete;
    CircularQueue& operator=(const CircularQueue&) = delete;

    // Make the class movable
    CircularQueue(CircularQueue&& other) noexcept : buffer(std::move(other.buffer)),
                                                    capacity(other.capacity),
                                                    head(other.head),
                                                    tail(other.tail),
                                                    count(other.count) {
        other.capacity = 0;
        other.head = 0;
        other.tail = 0;
        other.count = 0;
    }

    CircularQueue& operator=(CircularQueue&& other) noexcept {
        if (this != &other) {
            buffer = std::move(other.buffer);
            capacity = other.capacity;
            head = other.head;
            tail = other.tail;
            count = other.count;

            other.capacity = 0;
            other.head = 0;
            other.tail = 0;
            other.count = 0;
        }
        return *this;
    }

    // Resize the queue
    void resize(size_t new_size) {
        if (new_size <= 2) {
            throw std::invalid_argument("New size must be greater than 2");
        }
        std::vector<T> new_buffer(new_size);
        size_t len = std::min(count, new_size);
        for (size_t i = 0; i < len; ++i) {
            new_buffer[i] = std::move(buffer[(head + i) % capacity]);
        }
        buffer = std::move(new_buffer);
        head = 0;
        tail = len % new_size;
        capacity = new_size;
    }

    // Make the queue empty
    void clear() {
        head = tail = count = 0;
    }

    // Read data from the queue
    T& front() {
        if (empty()) {
            throw std::out_of_range("Queue is empty");
        }
        return buffer[head];
    }

    // Write data to the queue
    void push(const T& value) {
        if (full()) {
            head = (head + 1) % capacity;
        } else {
            ++count;
        }
        buffer[tail] = value;
        tail = (tail + 1) % capacity;
    }

    // Return number of data elements stored in the queue
    size_t size() const {
        return count;
    }

    // Check if the queue is full
    bool full() const {
        return count == capacity;
    }

    // Check if the queue is empty
    bool empty() const {
        return count == 0;
    }

    // Calculate the average of the data elements stored in the queue
    template<typename U = T>
    typename std::enable_if<std::is_arithmetic<U>::value, double>::type
    average() const {
        if (empty()) {
            throw std::logic_error("Queue is empty");
        }
        double sum = 0;
        for (size_t i = 0; i < count; ++i) {
            sum += buffer[(head + i) % capacity];
        }
        return sum / count;
    }
};

int main() {
    try {
        CircularQueue<int> int_queue(5);
        int_queue.push(1);
        int_queue.push(2);
        int_queue.push(3);
        int_queue.push(4);
        int_queue.push(5);
        std::cout << "Average: " << int_queue.average() << std::endl;

        CircularQueue<double> double_queue(4);
        double_queue.push(1.5);
        double_queue.push(2.5);
        double_queue.push(3.5);
        std::cout << "Average: " << double_queue.average() << std::endl;

        CircularQueue<std::string> string_queue(3);
        string_queue.push("Hello");
        string_queue.push("World");
        string_queue.push("!");
        std::cout << "Size: " << string_queue.size() << std::endl;

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
    return 0;
}

#endif