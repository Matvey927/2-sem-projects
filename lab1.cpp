#include <iostream>
#include <random>
#include <chrono>
#include <fstream>

int lin_search(int size, unsigned int search_for, unsigned int*& arr);

unsigned int* generate_random_array(std::default_random_engine& rng, std::uniform_int_distribution<unsigned>& dstr, unsigned int size) {
    unsigned int* arr = new unsigned int[size];
    for (unsigned int i = 0; i != size; ++i) {
        arr[i] = dstr(rng);
    }
    return arr;
}

int main() {
    unsigned seed = 1001;
    unsigned int num_tests = 1000;
    int searches = 0;
    std::ofstream l_search_times_avg("lin_s_times_avg.txt");
    std::ofstream l_search_times_worst("lin_s_times_worst.txt");

    std::default_random_engine rng(seed);
    if (l_search_times_avg.is_open() && l_search_times_worst.is_open()) {
        for (unsigned int size = 1000; size < 1000000; size += 1000) {
            std::uniform_int_distribution<unsigned> dstr(0, size);
            unsigned int* arr = generate_random_array(rng, dstr, size);

            std::default_random_engine test_rng(73);
            std::uniform_int_distribution<unsigned> dist(0, size - 1);
            unsigned int* numbers = generate_random_array(test_rng, dist, num_tests);

            auto start = std::chrono::steady_clock::now();
            for (unsigned int i = 0; i < num_tests; ++i) {
                searches = lin_search(size, numbers[i], arr);
            }
            auto end = std::chrono::steady_clock::now();
            auto lin_duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
            l_search_times_avg << size << " " << lin_duration.count() << std::endl;

            delete[] numbers;

            std::uniform_int_distribution<unsigned> dist_w(size, size + num_tests);
            numbers = generate_random_array(test_rng, dist_w, num_tests);

            start = std::chrono::steady_clock::now();
            for (unsigned int i = 0; i < num_tests; ++i) {
                searches = lin_search(size, numbers[i], arr);
            }
            end = std::chrono::steady_clock::now();
            lin_duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
            l_search_times_worst << size << " " << lin_duration.count() << std::endl;

            delete[] numbers;
            delete[]arr;
        }
        l_search_times_avg.close(); 
        l_search_times_worst.close(); 
    }
    std::cout << searches << std::endl;
    return 0;
}


/* int rand_num(const int* arr, int size) {
    if (!arr) return -1;

    std::mt19937 gen(73);
    std::uniform_int_distribution<int> dstr(0, size - 1);

    int index = dstr(gen);
    return arr[index];
}

int rand_num_out(const int* arr, int size) {
    if (!arr) return -1;

    std::mt19937 gen(73);
    std::uniform_int_distribution<int> dstr(size + 1, size + 1000);

    return dstr(gen);
} */

int lin_search(int size, unsigned int search_for, unsigned int*& arr) {
    for (int i = 0; i < size; ++i) {
        if (arr[i] == search_for) {
            return i;
        } 
    }
    return -1;
}
