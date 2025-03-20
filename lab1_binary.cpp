#include <iostream>
#include <random>
#include <chrono>
#include <fstream>

int bin_search(int size, unsigned int search_for, unsigned int*& arr);
void my_sort(unsigned int* arr, int size);

unsigned int* generate_random_array(std::default_random_engine& rng, std::uniform_int_distribution<unsigned>& dstr, unsigned int size) {
    unsigned int* arr = new unsigned int[size];
    for (unsigned int i = 0; i != size; ++i) {
        arr[i] = dstr(rng);
    }
    return arr;
}

int main() {
    unsigned seed = 1;
    unsigned int num_tests = 10000000;
    int searches = 0;

    std::default_random_engine rng(seed);
    
    std::ofstream b_search_times_avg("bin_s_times_avg.txt");
    std::ofstream b_search_times_worst("bin_s_times_worst.txt");
    if (b_search_times_avg.is_open() && b_search_times_worst.is_open()) {
        for (unsigned int size = 70000; size < 1000000; size += 70000) {
            std::uniform_int_distribution<unsigned> dstr(0, size);
            unsigned int* arr = generate_random_array(rng, dstr, size);
            my_sort(arr, size);

            std::default_random_engine test_rng(73);
            std::uniform_int_distribution<unsigned> dist(0, size - 1);
            unsigned int* numbers = generate_random_array(test_rng, dist, num_tests);

            auto start = std::chrono::steady_clock::now();
            for (unsigned int i = 0; i < num_tests; ++i) {
                searches = bin_search(size, numbers[i], arr);
            }
            auto end = std::chrono::steady_clock::now();
            auto bin_duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
            b_search_times_avg << size << " " << bin_duration.count() << std::endl;

            delete[] numbers;

            std::uniform_int_distribution<unsigned int> dist_w(size, size + num_tests);
            numbers = generate_random_array(test_rng, dist_w, num_tests);

            start = std::chrono::steady_clock::now();
            for (unsigned int i = 0; i < num_tests; ++i) {
                searches = bin_search(size, numbers[i], arr);
            }
            end = std::chrono::steady_clock::now();
            bin_duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
            b_search_times_worst << size << " " << bin_duration.count() << std::endl;

            delete[] numbers;
            delete[]arr;
        }
        b_search_times_avg.close(); 
        b_search_times_worst.close(); 
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


int bin_search(int size, unsigned int search_for, unsigned int*& arr) {
    int left = 0;
    int right = size - 1;

    while (left <= right) {
        int mid = left + (right - left) / 2;

        if (arr[mid] == search_for) {
            return mid;
        } else if (arr[mid] < search_for) {
            left = mid + 1;
        } else {
            right = mid - 1;
        }
    }
    return -1;
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