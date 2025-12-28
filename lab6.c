#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define ITERATIONS 100
#define MAX_VALUE 1000

void bubbleSort(int* arr, size_t n) {
   for (size_t i = 0; i < n; i++) {
        for (size_t j = 0; j + 1 < n - i; j++) {
            if (arr[j] > arr[j + 1]) {
                int tmp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = tmp;
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
                int tmp = arr[i];
                arr[i] = arr[j];
                arr[j] = tmp;
            }
        }

        int tmp = arr[i + 1];
        arr[i + 1] = arr[high];
        arr[high] = tmp;

        int pi = i + 1;
        quickSort(arr, low, pi - 1);
        quickSort(arr, pi + 1, high);
    }
}

void quickSortWrapper(int* arr, size_t n) {
    quickSort(arr, 0, n - 1);
}

void countingSort(int* arr, size_t n, int max_value) {
    int* count = (int*)calloc(max_value + 1, sizeof(int));
    if (!count) {
        perror("Memory allocation failed");
        exit(EXIT_FAILURE);
    }

    for (size_t i = 0; i < n; i++) {
        count[arr[i]]++;
    }

    size_t index = 0;
    for (int value = 0; value <= max_value; value++) {
        while (count[value] > 0) {
            arr[index++] = value;
            count[value]--;
        }
    }

    free(count);
}

void countingSortWrapper(int* arr, size_t n) {
    countingSort(arr, n, MAX_VALUE);
}

int* createRandomArray(size_t size) {
    static int seeded = 0;
    if (!seeded) {
        srand(time(NULL));
        seeded = 1;
    }
    
    int* array = (int*)malloc(size * sizeof(int));
    if (!array) {
        perror("Memory allocation failed");
        exit(EXIT_FAILURE);
    }
    
    for (size_t i = 0; i < size; i++) {
        array[i] = rand() % (MAX_VALUE + 1);
    }
    
    return array;
}

int* copyArray(const int* source, size_t n) {
    int* dest = (int*)malloc(n * sizeof(int));
    if (!dest) {
        perror("Memory allocation failed");
        exit(EXIT_FAILURE);
    }
    
    for (size_t i = 0; i < n; i++) {
        dest[i] = source[i];
    }
    
    return dest;
}

void saveToFile(const char* filename, double* times, int count) {
    FILE* file = fopen(filename, "w");
    if (!file) {
        perror("Failed to open file");
        return;
    }
    
    for (int i = 0; i < count; i++) {
        fprintf(file, "%.3f\n", times[i]);
    }
    
    fclose(file);
}

void runExperiment(size_t size, int iterations) {
    printf("N = %zu\n", size);
    
    double bubble_times[ITERATIONS];
    double quick_times[ITERATIONS];
    double counting_times[ITERATIONS];
    
    // Bubble Sort
    if (size <= 10000) {
        printf("Bubble Sort: ");
        fflush(stdout);
        
        for (int i = 0; i < iterations; i++) {
            int* original = createRandomArray(size);
            int* array = copyArray(original, size);
            
            clock_t start = clock();
            bubbleSort(array, size);
            clock_t end = clock();
            
            bubble_times[i] = (double)(end - start) / CLOCKS_PER_SEC * 1000.0;
            
            free(original);
            free(array);
            
            if ((i + 1) % 20 == 0) {
                printf("%d/%d ", i + 1, iterations);
                fflush(stdout);
            }
        }
        printf("\n");
        
        char filename[100];
        sprintf(filename, "bubble_sort_%zu.txt", size);
        saveToFile(filename, bubble_times, iterations);
    }
    
    // Quick Sort
    printf("Quick Sort: ");
    fflush(stdout);
    
    for (int i = 0; i < iterations; i++) {
        int* original = createRandomArray(size);
        int* array = copyArray(original, size);
        
        clock_t start = clock();
        quickSortWrapper(array, size);
        clock_t end = clock();
        
        quick_times[i] = (double)(end - start) / CLOCKS_PER_SEC * 1000.0;
        
        free(original);
        free(array);
        
        if ((i + 1) % 20 == 0) {
            printf("%d/%d ", i + 1, iterations);
            fflush(stdout);
        }
    }
    printf("\n");
    
    char filename[100];
    sprintf(filename, "quick_sort_%zu.txt", size);
    saveToFile(filename, quick_times, iterations);
    
    // Counting Sort
    printf("Counting Sort: ");
    fflush(stdout);
    
    for (int i = 0; i < iterations; i++) {
        int* original = createRandomArray(size);
        int* array = copyArray(original, size);
        
        clock_t start = clock();
        countingSortWrapper(array, size);
        clock_t end = clock();
        
        counting_times[i] = (double)(end - start) / CLOCKS_PER_SEC * 1000.0;
        
        free(original);
        free(array);
        
        if ((i + 1) % 20 == 0) {
            printf("%d/%d ", i + 1, iterations);
            fflush(stdout);
        }
    }
    printf("\n");
    
    sprintf(filename, "counting_sort_%zu.txt", size);
    saveToFile(filename, counting_times, iterations);
    
    printf("\n");
}

int main() {
    size_t sizes[] = {1000, 10000, 100000, 1000000};
    int num_sizes = sizeof(sizes) / sizeof(sizes[0]);
    
    printf("Starting experiments...\n\n");
    
    for (int i = 0; i < num_sizes; i++) {
        runExperiment(sizes[i], ITERATIONS);
    }
    
    printf("Experiments completed. Files saved.\n");
    
    return 0;
}