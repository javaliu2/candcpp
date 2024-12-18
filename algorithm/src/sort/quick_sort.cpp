#include <iostream>

using namespace std;

void mySwap(int arr[], int i, int j) {
    int t = arr[i];
    arr[i] = arr[j];
    arr[j] = t;
}

/* failure version, learn from predecessor */
void quick_sort_failure(int arr[], int left, int right) {
    /*
    ==表示只有一个元素；>表示空数组。这两者肯定有序
    */
    if (left >= right) {
        return;
    }
    int mid = (left + right) / 2, target = arr[mid];
    int i = left, j = right;
    while (i < j) {
        // mid右侧找小于arr[mid]
        while (arr[j] > target) {
            j--;
        }
        // mid左侧找大于arr[mid]
        while (arr[i] < target) {
            i++;
        }
        if (i < j) {
            mySwap(arr, i, j);
            i++;
            j--;
        }
    }
    // 递归解决子问题
    // 这里的mid，不对，要看i和j的值，而不是mid，[mid]所在的值已经发生变化
    // quick_sort(arr, left, mid - 1);
    // quick_sort(arr, mid + 1, right);
    quick_sort_failure(arr, left, i - 1);
    quick_sort_failure(arr, i + 1, right);
}

/**
 * 快速排序，采用分治的思想，通过找一个pivot，将小于pivot的元素放于pivot_index左侧，
 * 大于pivot的元素置于右侧；进而递归解决两侧的子问题
 * 实现关键：partition函数的编写，partition函数需要返回划分好左右区间的分割元素的索引
 */
int partition(int arr[], int left, int right) {
    int pivot_index = (left + right) / 2;
    // int pivot_index = (left + right) >> 1;  // 右移1位等价于除以2
    // int pivot_index = left + (right - left) / 2;  // 不会发生溢出
    int pivot = arr[pivot_index];
    int storeIndex = left;  // 指向第一个大于等于pivot值的位置
    // 将pivot交换到数组最末端
    mySwap(arr, pivot_index, right);
    // 这里的区间是到right-1
    for (int i = left; i <= right - 1; i++) {
        // 
        if (arr[i] < pivot) {
            mySwap(arr, i, storeIndex);
            storeIndex += 1;
        }
    }
    // 最后需要把pivot交换回来
    mySwap(arr, right, storeIndex);
    return storeIndex;
}

void quick_sort(int arr[], int left, int right) {
    // ==的话，是一个元素，肯定有序，递归结束
    if (left < right) {
        int index = partition(arr, left, right);
        quick_sort(arr, left, index - 1);
        quick_sort(arr, index + 1, right);
    }
}

int main() {
    int arr[7] = {3,7,-6,3,-1,8,5};
    quick_sort(arr, 0, 6);
    for (int i = 0; i < 7; i++) {
        cout << arr[i] << " ";
    }
    return 0;
}