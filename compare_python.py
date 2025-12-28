import time
import random
import numpy as np

MAX_VALUE = 1000
ITERATIONS = 20

def bubble_sort(arr):
    n = len(arr)
    for i in range(n):
        for j in range(0, n - i - 1):
            if arr[j] > arr[j + 1]:
                arr[j], arr[j + 1] = arr[j + 1], arr[j]
    return arr

def quick_sort(arr):
    if len(arr) <= 1:
        return arr
    pivot = arr[-1]
    left = [x for x in arr[:-1] if x <= pivot]
    right = [x for x in arr[:-1] if x > pivot]
    return quick_sort(left) + [pivot] + quick_sort(right)

def counting_sort(arr):
    count = [0] * (MAX_VALUE + 1)
    for num in arr:
        count[num] += 1
    
    result = []
    for i in range(MAX_VALUE + 1):
        result.extend([i] * count[i])
    
    return result

def measure_time(sort_func, size):
    times = []
    
    for _ in range(ITERATIONS):
        arr = [random.randint(0, MAX_VALUE) for _ in range(size)]
        
        start = time.perf_counter()
        if sort_func.__name__ in ['bubble_sort', 'quick_sort', 'counting_sort']:
            sort_func(arr.copy())
        elif sort_func == list.sort:
            arr.sort()
        else:  # sorted
            sorted(arr)
        end = time.perf_counter()
        
        times.append((end - start) * 1000)
    
    return np.mean(times)

def main():
    print("Python Сравнение сортировок\n")
    
    sizes = [1000, 10000, 100000]
    
    print(f"{'Размер':>8} | {'Bubble':>8} | {'Quick':>8} | {'Counting':>10} | {'list.sort':>10} | {'sorted()':>10} | {'numpy':>8}")
    print("-" * 80)
    
    for size in sizes:
        results = [f"{size:>8}"]
        
        # Bubble
        if size == 1000:
            t = measure_time(bubble_sort, size)
            results.append(f"{t:>8.2f}")
        else:
            results.append(f"{'N/A':>8}")
        
        # Quick
        t = measure_time(quick_sort, size)
        results.append(f"{t:>8.2f}")
        
        # Counting
        t = measure_time(counting_sort, size)
        results.append(f"{t:>10.2f}")
        
        # list.sort
        t = measure_time(list.sort, size)
        results.append(f"{t:>10.2f}")
        
        # sorted()
        t = measure_time(sorted, size)
        results.append(f"{t:>10.2f}")
        
        # numpy (если установлен)
        try:
            import numpy
            times = []
            for _ in range(ITERATIONS):
                arr = np.random.randint(0, MAX_VALUE + 1, size)
                start = time.perf_counter()
                np.sort(arr)
                end = time.perf_counter()
                times.append((end - start) * 1000)
            t = np.mean(times)
            results.append(f"{t:>8.2f}")
        except:
            results.append(f"{'N/A':>8}")
        
        print(" | ".join(results))

if __name__ == "__main__":
    main()
