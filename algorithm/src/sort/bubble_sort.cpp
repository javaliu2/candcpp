#include <iostream>

using namespace std;


void bubble_sort_head(int arr[], int n);
void bubble_sort_tail(int arr[], int n);
void print(const int arr[], int n);
template<typename T>
void bubble_sort(T arr[], int n);

int main() {
    const int LEN = 7;
    int arr[LEN] = {0, -2, 4, 5, 6, 1, -10};
    cout << "before sort: " << endl;
    print(arr, LEN);
    bubble_sort_tail(arr, LEN);
    cout << "after sort: " << endl;
    print(arr, LEN);
    return 0;
}

/**
 * 头冒，元素往前交换
 */
void bubble_sort_head(int arr[], int n) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = n - 1; j > i; j--) {
            if (arr[j] < arr[j - 1]) {
                int t = arr[j - 1];
                arr[j - 1] = arr[j];
                arr[j] = t;
            }
        }
    }
}
/** 
 * 尾冒，元素往后交换
*/
void bubble_sort_tail(int arr[], int n) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - 1 - i; j++) {
            if (arr[j] > arr[j + 1]) {
                int t = arr[j + 1];
                arr[j + 1] = arr[j];
                arr[j] = t;
            }
        }
    }
}

void print(const int arr[], int len) {
    for (int i = 0; i < len; i++) {
        cout << arr[i] << " ";
    }
    cout << endl;
}

template<typename T>
void bubble_sort(T arr[], int n){
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - 1 - i; j++) {
            if (arr[j] > arr[j + 1]) {
                T t = arr[j + 1];
                arr[j + 1] = arr[j];
                arr[j] = t;
            }
        }
    }
}
