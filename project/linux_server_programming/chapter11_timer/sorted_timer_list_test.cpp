#include "ascending_linkedlist_timer_impl.h"
#include <time.h>
#include <iostream>
using namespace std;

int main() {
    time_t t = time(NULL);
    cout << t << endl;  // 1970年1月1日至今的秒数
    return 0;
}