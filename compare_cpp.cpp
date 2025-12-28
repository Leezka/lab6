#include <iostream>
#include <vector>
#include <algorithm>
#include <chrono>
#include <random>
#include <fstream>

#define MAX_VALUE 1000
#define ITERATIONS 30

void bubbleSort(int* arr, size_t n) {
    for (size_t i = 0; i < n; i++) {
        for (size_t j = 0; j + 1 < n - i; j++) {
            if (arr[j] > arr[j + 1]) {
                std::swap(arr[j], arr[j + 1]);
            }
        }
    }
}

void quickSort(int* arr, int low, int high) {
    if (low < high) {
        int pivot = arr[high];
        int i = low - 1;
        for (int j = low; j < high; j++) {
            if (arr[j] <= pivot) {
                i++;
                std::swap(arr[i], arr[j]);
            }
        }
        std::swap(arr[i + 1], arr[high]);
        int pi = i + 1;
        quickSort(arr, low, pi - 1);
        quickSort(arr, pi + 1, high);
    }
}

void quickSortWrapper(int* arr, size_t n) {
    quickSort(arr, 0, n - 1);
}

void countingSort(int* arr, size_t n) {
    int* count = new int[MAX_VALUE + 1]();
    for (size_t i = 0; i < n; i++) {
        count[arr[i]]++;
    }
    size_t index = 0;
    for (int value = 0; value <= MAX_VALUE; value++) {
        while (count[value] > 0) {
            arr[index++] = value;
            count[value]--;
        }
    }
    delete[] count;
}

void stdSortWrapper(int* arr, size_t n) {
    std::vector<int> vec(arr, arr + n);
    std::sort(vec.begin(), vec.end());
    std::copy(vec.begin(), vec.end(), arr);
}

int* createRandomArray(size_t size) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static bool seeded = false;
    if (!seeded) {
        gen.seed(std::chrono::system_clock::now().time_since_epoch().count());
        seeded = true;
    }
    std::uniform_int_distribution<> dis(0, MAX_VALUE);
    
    int* array = new int[size];
    for (size_t i = 0; i < size; i++) {
        array[i] = dis(gen);
    }
    return array;
}

double measureTime(void (*sortFunc)(int*, size_t), size_t size) {
    double total = 0.0;
    
    for (int i = 0; i < ITERATIONS; i++) {
        int* arr = createRandomArray(size);
        
        auto start = std::chrono::high_resolution_clock::now();
        sortFunc(arr, size);
        auto end = std::chrono::high_resolution_clock::now();
        
        std::chrono::duration<double, std::milli> elapsed = end - start;
        total += elapsed.count();
        
        delete[] arr;
    }
    
    return total / ITERATIONS;
}

int main() {
    std::cout << "C++ Сравнение сортировок\n\n";
    
    size_t sizes[] = {1000, 10000, 100000};
    
    std::cout << "Размер | Bubble | Quick | Counting | std::sort\n";
    std::cout << "------ | ------ | ----- | -------- | ---------\n";
    
    for (size_t size : sizes) {
        std::cout << size;
        
        // Bubble
        if (size == 1000) {
            double t = measureTime(bubbleSort, size);
            std::cout << " | " << t;
        } else {
            std::cout << " | -";
        }
        
        // Quick
        double t = measureTime(quickSortWrapper, size);
        std::cout << " | " << t;
        
        // Counting
        t = measureTime(countingSort, size);
        std::cout << " | " << t;
        
        // std::sort
        t = measureTime(stdSortWrapper, size);
        std::cout << " | " << t;
        
        std::cout << "\n";
    }
    
    return 0;
}
