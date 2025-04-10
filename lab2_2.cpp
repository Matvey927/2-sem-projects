#include <iostream>
#include <chrono>
#include <fstream>

std::size_t capacity = 0;

void queue_new(std::size_t &poll_idx, std::size_t &push_idx, int* &arr) {
    capacity = 1;
    poll_idx = 0;
    push_idx = 0;
    arr = new int[capacity];
}

void queue_del(int* &arr) {
    delete[] arr;
    arr = nullptr;
}

bool queue_poll(std::size_t &poll_idx, std::size_t &push_idx, int* &arr, int &element) {
    if (push_idx == poll_idx) {
        return false;
    element = arr[poll_idx];
    poll_idx= (poll_idx + 1) % capacity;
    return true;
    }
    
    element = arr[poll_idx];
    poll_idx = (poll_idx + 1) % capacity;
    return true;
}

bool queue_push(std::size_t &poll_idx, std::size_t &push_idx, int* &arr, int element) {
    std::size_t next = (push_idx + 1) % capacity;
    if (next == poll_idx) {
        std::size_t new_capacity = capacity * 2;
        int* new_arr = new(std::nothrow) int[new_capacity];
        if (!new_arr) return false;

        std::size_t i = 0;
        while (poll_idx != push_idx) {
            new_arr[i++] = arr[poll_idx];
            poll_idx = (poll_idx + 1) % capacity;
        }
        
        delete[] arr;
        arr = new_arr;
        poll_idx = 0;
        push_idx = i;
        capacity = new_capacity;
    }
    
    arr[push_idx] = element;
    push_idx = (push_idx + 1) % capacity;
    return true;
}

int main() {
    std::ofstream push("push.txt");
    std::ofstream poll("poll.txt");

    if (push.is_open() && poll.is_open()) {
        for (std::size_t size = 10; size <= 1000000; size *= 10) {

            std::size_t read = 0, write = 0;
            int* data = nullptr;
            queue_new(read, write, data);

            auto start = std::chrono::high_resolution_clock::now();
            for (std::size_t i = 0; i < size; ++i) {
                if (queue_push(read, write, data, i) == false) break;
            }
            auto end = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double> duration = end - start;
            push << size << " " << duration.count() << std::endl;

            int temp;
            auto poll_start = std::chrono::high_resolution_clock::now();
            while (queue_poll(read, write, data, temp));
            auto poll_end = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double> poll_duration = poll_end - poll_start;
            poll << size << " " << poll_duration.count() << std::endl;

            queue_del(data);
        }

        push.close();
        poll.close();
    }
    return 0;
}