#include <iostream>
#include <random>
#include <chrono>
#include <fstream>

unsigned int* generate_random_array(std::default_random_engine& rng, std::uniform_int_distribution<unsigned>& dstr, unsigned int size) {
    unsigned int* arr = new unsigned int[size];
    for (unsigned int i = 0; i != size; ++i) {
        arr[i] = dstr(rng);
    }
    return arr;
}

bool linear_sum_search(int size, unsigned int target, unsigned int* arr) {
    for (int i = 0; i < size; ++i) {
        for (int j = i; j < size; ++j) {
            if (arr[i] + arr[j] == target) {
                return true;
            }
        }
    }
    return false;
}

void my_sort(unsigned int* arr, int size) {
    bool sorted;
    do {
        sorted = true;
        for (int i = 0; i + 1 < size; ++i)
            if (arr[i] > arr[i+1]) {
                int temp = arr[i];
                arr[i] = arr[i+1];
                arr[i+1] = temp;
                sorted = false;            
            }
    } while (!sorted);
}
bool optimized_sum_search(int size, unsigned int target, unsigned int* arr) {
    int left = 0;
    int right = size - 1;

    while (left < right) {
        unsigned int current = arr[left] + arr[right];
        if (current < target) {
            ++left;
        } else if (current > target) {
            --right;
        } else {
            return true;
        }
    }
    return false;
}

float measure_search_time(bool(*search_func)(int, unsigned int, unsigned int*), int size, unsigned int target_sum, unsigned int* arr) {
    auto start = std::chrono::steady_clock::now();
    search_func(size, target_sum, arr);
    auto end = std::chrono::steady_clock::now();
    return std::chrono::duration<float, std::milli>(end - start).count();
}

int main() {
    unsigned seed = 1001;    
    std::ofstream first("afirst.txt");
    std::ofstream second("asecond.txt");

    std::default_random_engine rng(seed);
    if (first.is_open() && second.is_open()) {
        for (unsigned int size = 20000; size < 1000000; size += 20000) {
            std::uniform_int_distribution<unsigned> dstr(0, size);
            unsigned int* arr = generate_random_array(rng, dstr, size);

            unsigned int target = 150;
            float linear_time = measure_search_time(linear_sum_search, size, target, arr);
            first << size << " " << linear_time << std::endl;
            
            float optimized_time = measure_search_time(optimized_sum_search, size, target, arr);
            second << size << " " << optimized_time << std::endl;

            delete[] arr;
        }
        first.close();
        second.close();
    }
    return 0;
}
