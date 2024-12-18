#include <cstdlib>

/**
 * 716 / 743 个通过的测试用例
 */
int minMovesToCaptureTheQueen_my(int a, int b, int c, int d, int e, int f) {
    if (a == e) {
        if (a != c)
            return 1;
        else
            return 2;
    }
    if (b == f) {
        if (b != d)
            return 1;
        else
            return 2;
    }
    if (abs(e - c) == abs(f - d)) {
        if (abs(a - c) != abs(b - d))
            return 1;
        else
            return 2;
    }
    return 2;
}
