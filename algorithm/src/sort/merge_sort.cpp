#include <iostream>
#include <random>
using namespace std;

void merge(int arr[], int l, int m, int r) {
    const int len = r - l + 1;
    int temp[len], j = m + 1, k = l, i;
    for (i = 0; i < len; i++) {
        if (k > m || j > r) {
            break;
        }
        if (arr[k] < arr[j]) {
            temp[i] = arr[k++];
        } else {
            temp[i] = arr[j++];
        }
    }
    while (k <= m) {
        temp[i++] = arr[k++];
    }
    while (j <= r) {
        temp[i++] = arr[j++];
    }
    for (i = 0; i < len; i++) {
        arr[l++] = temp[i];
    }
}

void merge_sort(int arr[], int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;
        merge_sort(arr, left, mid);
        merge_sort(arr, mid + 1, right);
        merge(arr, left, mid, right);
    }
}

void array_print(int arr[], int len) {
    for (size_t i = 0; i < len; i++) {
        cout << arr[i] << " ";
    }
    cout << endl;
}
int main() {
    // int arr[7] = {3,8,-6,-2,3,4,5};
    // merge_sort(arr, 0, 6);
    // 生成10组数据，每组数组长度不一致且元素范围在[-100,100]之间，元素长度在[20,40]之间
    // 采用C++ random 库进行生成
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> item_dis(-100, 100);
    uniform_int_distribution<> len_dis(20, 40);
    for (int cnt = 0; cnt < 10; cnt++) {
        int len = len_dis(gen);
        int arr[len];
        for (int i = 0; i < len; i++) {
            arr[i] = item_dis(gen);
        }
        merge_sort(arr, 0, len - 1);
        array_print(arr, len);
    }
    return 0;
}