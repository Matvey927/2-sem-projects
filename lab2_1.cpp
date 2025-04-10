#include <iostream>
#include <chrono>
#include <fstream>


void strategy1(int num_elements) {
    int* arr = nullptr;
    int size = 0;

    auto start = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < num_elements; ++i) {
        int* new_arr = new int[size + 1];
        for (int j = 0; j < size; ++j) {
            new_arr[j] = arr[j];
        }
        new_arr[size] = i;
        delete[] arr;
        arr = new_arr;
        size++;
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;
    std::cout << "first strategy: " << duration.count() << " s\n";

    delete[] arr;
}

void strategy2(int num_elements, int block_size = 100) {
    int* arr = nullptr;
    int size = 0;
    int capacity = 0;

    auto start = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < num_elements; ++i) {
        if (size >= capacity) {
            capacity += block_size;
            int* new_arr = new int[capacity];
            for (int j = 0; j < size; ++j) {
                new_arr[j] = arr[j];
            }
            delete[] arr;
            arr = new_arr;
        }
        arr[size] = i;
        size++;
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;
    std::cout << "second strategy: " << duration.count() << " s\n";

    delete[] arr;
}

void strategy3(int num_elements) {
    int* arr = nullptr;
    int size = 0;
    int capacity = 1;

    auto start = std::chrono::high_resolution_clock::now();

    arr = new int[capacity];
    for (int i = 0; i < num_elements; ++i) {
        if (size >= capacity) {
            capacity *= 2;
            int* new_arr = new int[capacity];
            for (int j = 0; j < size; ++j) {
                new_arr[j] = arr[j];
            }
            delete[] arr;
            arr = new_arr;
        }
        arr[size] = i;
        size++;
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;
    std::cout << "third strategy: " << duration.count() << " s\n";

    delete[] arr;
}



void strategy1_to_csv(int num_elements, std::ofstream& outfile) {
    int* arr = nullptr;
    int size = 0;

    auto start = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < num_elements; ++i) {
        int* new_arr = new int[size + 1];
        for (int j = 0; j < size; ++j) {
            new_arr[j] = arr[j];
        }
        new_arr[size] = i;
        delete[] arr;
        arr = new_arr;
        size++;
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;
    outfile << size << " " << duration.count() << std::endl;
    delete[] arr;
}

void strategy2_to_csv(int num_elements, int block_size, std::ofstream& outfile) {
    int* arr = nullptr;
    int size = 0;
    int capacity = 0;

    auto start = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < num_elements; ++i) {
        if (size >= capacity) {
            capacity += block_size;
            int* new_arr = new int[capacity];
            for (int j = 0; j < size; ++j) {
                new_arr[j] = arr[j];
            }
            delete[] arr;
            arr = new_arr;
        }
        arr[size] = i;
        size++;
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;
    outfile << size << " " << duration.count() << std::endl;

    delete[] arr;
}

void strategy3_to_csv(int num_elements, std::ofstream& outfile) {
    int* arr = nullptr;
    int size = 0;
    int capacity = 1;

    auto start = std::chrono::high_resolution_clock::now();

    arr = new int[capacity];
    for (int i = 0; i < num_elements; ++i) {
        if (size >= capacity) {
            capacity *= 2;
            int* new_arr = new int[capacity];
            for (int j = 0; j < size; ++j) {
                new_arr[j] = arr[j];
            }
            delete[] arr;
            arr = new_arr;
        }
        arr[size] = i;
        size++;
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;
    outfile << size << " " << duration.count() << std::endl;

    delete[] arr;
}


int main() {
    const int test = 100000;
    std::cout << "for N = " << test << " elements:\n";
    strategy1(test);
    strategy2(test);
    strategy3(test);

    std::ofstream firststr("str1.txt");
    std::ofstream secondstr("str2.txt");
    std::ofstream thirdstr("str3.txt");

    if (firststr.is_open() && secondstr.is_open() && thirdstr.is_open()) {
        for ( int N = 10000; N < 100000; N += 2000) {
            strategy1_to_csv(N, firststr);
            strategy2_to_csv(N, 100, secondstr);
            strategy3_to_csv(N, thirdstr);
        }
        firststr.close(); 
        secondstr.close(); 
        thirdstr.close();
    }
    return 0;
}