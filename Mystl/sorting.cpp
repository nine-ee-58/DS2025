#ifndef SORTING_H
#define SORTING_H

#include <vector>
#include <functional>

using namespace std;

// 通用排序算法模板类
template<typename T>
class Sorting {
public:
    // 快速排序 - 使用默认比较（升序）
    static void quickSort(vector<T>& arr) {
        if (!arr.empty()) {
            quickSortOptimized(arr, 0, arr.size() - 1, less<T>());
        }
    }
    
    // 快速排序 - 自定义比较函数
    template<typename Compare>
    static void quickSort(vector<T>& arr, Compare comp) {
        if (!arr.empty()) {
            quickSortOptimized(arr, 0, arr.size() - 1, comp);
        }
    }
    
    // 快速排序 - 指定范围和比较函数
    template<typename Compare>
    static void quickSort(vector<T>& arr, int low, int high, Compare comp) {
        if (low < high) {
            quickSortOptimized(arr, low, high, comp);
        }
    }

private:
    // 优化的快速排序 - 结合堆优化策略避免最坏情况
    template<typename Compare>
    static void quickSortOptimized(vector<T>& arr, int low, int high, Compare comp) {
        while (low < high) {
            // 对于小数组使用插入排序
            if (high - low < 10) {
                insertionSortRange(arr, low, high, comp);
                break;
            } else {
                // 使用三数取中法选择pivot
                int pivotIndex = medianOfThree(arr, low, high, comp);
                swap(arr[pivotIndex], arr[high]);
                
                int pi = partition(arr, low, high, comp);
                
                // 为了避免最坏情况，检查分区是否不平衡
                // 如果分区非常不平衡，使用堆排序处理大的分区
                if (pi - low > (high - low) * 0.9 || high - pi > (high - low) * 0.9) {
                    // 分区不平衡，对较大的分区使用堆排序
                    if (pi - low > high - pi) {
                        // 左边较大，对左边使用堆排序，继续快排右边
                        heapSortRange(arr, low, pi - 1, comp);
                        high = pi - 1; // 继续处理右边
                    } else {
                        // 右边较大，对右边使用堆排序，继续快排左边
                        heapSortRange(arr, pi + 1, high, comp);
                        high = pi - 1; // 继续处理左边
                    }
                } else {
                    // 分区相对平衡，继续使用快排
                    // 优化递归：先处理较小的部分，减少栈深度
                    if (pi - low < high - pi) {
                        quickSortOptimized(arr, low, pi - 1, comp);
                        low = pi + 1; // 迭代处理较大的部分
                    } else {
                        quickSortOptimized(arr, pi + 1, high, comp);
                        high = pi - 1; // 迭代处理较大的部分
                    }
                }
            }
        }
    }
    
    // 在指定范围内进行插入排序
    template<typename Compare>
    static void insertionSortRange(vector<T>& arr, int low, int high, Compare comp) {
        for (int i = low + 1; i <= high; i++) {
            T key = arr[i];
            int j = i - 1;
            
            while (j >= low && !comp(arr[j], key)) {
                arr[j + 1] = arr[j];
                j--;
            }
            arr[j + 1] = key;
        }
    }
    
    // 在指定范围内进行堆排序
    template<typename Compare>
    static void heapSortRange(vector<T>& arr, int low, int high, Compare comp) {
        int n = high - low + 1;
        // 在原数组上进行堆排序
        for (int i = n / 2 - 1; i >= 0; i--) {
            heapifyRange(arr, n, i, low, comp);
        }
        
        for (int i = n - 1; i > 0; i--) {
            swap(arr[low], arr[low + i]);
            heapifyRange(arr, i, 0, low, comp);
        }
    }
    
    // 在指定范围内进行堆化
    template<typename Compare>
    static void heapifyRange(vector<T>& arr, int n, int i, int offset, Compare comp) {
        int largest = i;
        int left = 2 * i + 1;
        int right = 2 * i + 2;
        
        if (left < n && comp(arr[offset + left], arr[offset + largest]))
            largest = left;
        
        if (right < n && comp(arr[offset + right], arr[offset + largest]))
            largest = right;
        
        if (largest != i) {
            swap(arr[offset + i], arr[offset + largest]);
            heapifyRange(arr, n, largest, offset, comp);
        }
    }
    
    // 三数取中法选择pivot
    template<typename Compare>
    static int medianOfThree(vector<T>& arr, int low, int high, Compare comp) {
        int mid = low + (high - low) / 2;
        
        if (comp(arr[mid], arr[low]))
            swap(arr[low], arr[mid]);
        if (comp(arr[high], arr[low]))
            swap(arr[low], arr[high]);
        if (comp(arr[high], arr[mid]))
            swap(arr[mid], arr[high]);
            
        return mid;
    }
    
    template<typename Compare>
    static int partition(vector<T>& arr, int low, int high, Compare comp) {
        T pivot = arr[high];
        int i = low - 1;

        for (int j = low; j <= high - 1; j++) {
            if (comp(arr[j], pivot)) { // 使用比较函数
                i++;
                swap(arr[i], arr[j]);
            }
        }
        swap(arr[i + 1], arr[high]);
        return i + 1;
    }

public:
    // 归并排序 - 使用默认比较（升序）
    static void mergeSort(vector<T>& arr) {
        if (!arr.empty()) {
            mergeSort(arr, 0, arr.size() - 1, less<T>());
        }
    }
    
    // 归并排序 - 自定义比较函数
    template<typename Compare>
    static void mergeSort(vector<T>& arr, Compare comp) {
        if (!arr.empty()) {
            mergeSort(arr, 0, arr.size() - 1, comp);
        }
    }
    
    // 归并排序 - 指定范围和比较函数
    template<typename Compare>
    static void mergeSort(vector<T>& arr, int left, int right, Compare comp) {
        if (left < right) {
            int mid = left + (right - left) / 2;
            mergeSort(arr, left, mid, comp);
            mergeSort(arr, mid + 1, right, comp);
            merge(arr, left, mid, right, comp);
        }
    }

private:
    template<typename Compare>
    static void merge(vector<T>& arr, int left, int mid, int right, Compare comp) {
        int n1 = mid - left + 1;
        int n2 = right - mid;

        vector<T> L(n1), R(n2);

        for (int i = 0; i < n1; i++)
            L[i] = arr[left + i];
        for (int j = 0; j < n2; j++)
            R[j] = arr[mid + 1 + j];

        int i = 0, j = 0, k = left;

        while (i < n1 && j < n2) {
            if (comp(L[i], R[j])) {
                arr[k] = L[i];
                i++;
            } else {
                arr[k] = R[j];
                j++;
            }
            k++;
        }

        while (i < n1) {
            arr[k] = L[i];
            i++;
            k++;
        }

        while (j < n2) {
            arr[k] = R[j];
            j++;
            k++;
        }
    }

public:
    // 堆排序 - 使用默认比较（升序）
    static void heapSort(vector<T>& arr) {
        if (!arr.empty()) {
            heapSort(arr, less<T>());
        }
    }
    
    // 堆排序 - 自定义比较函数
    template<typename Compare>
    static void heapSort(vector<T>& arr, Compare comp) {
        int n = arr.size();

        // 构建堆
        for (int i = n / 2 - 1; i >= 0; i--)
            heapify(arr, n, i, comp);

        // 逐个提取元素
        for (int i = n - 1; i > 0; i--) {
            swap(arr[0], arr[i]);
            heapify(arr, i, 0, comp);
        }
    }

private:
    template<typename Compare>
    static void heapify(vector<T>& arr, int n, int i, Compare comp) {
        int largest = i;
        int left = 2 * i + 1;
        int right = 2 * i + 2;

        if (left < n && comp(arr[largest], arr[left]))
            largest = left;

        if (right < n && comp(arr[largest], arr[right]))
            largest = right;

        if (largest != i) {
            swap(arr[i], arr[largest]);
            heapify(arr, n, largest, comp);
        }
    }

public:
    // 插入排序 - 使用默认比较（升序）
    static void insertionSort(vector<T>& arr) {
        if (!arr.empty()) {
            insertionSort(arr, less<T>());
        }
    }
    
    // 插入排序 - 自定义比较函数
    template<typename Compare>
    static void insertionSort(vector<T>& arr, Compare comp) {
        int n = arr.size();
        for (int i = 1; i < n; i++) {
            T key = arr[i];
            int j = i - 1;

            while (j >= 0 && !comp(arr[j], key)) {
                arr[j + 1] = arr[j];
                j--;
            }
            arr[j + 1] = key;
        }
    }
};

#endif // SORTING_H