#include <cstring>
#include <iostream>
#include <string>

using namespace std;
/**
 * @hint:
 * 观察棋盘可知，纵轴8 7 6 5 4 3 2 1，
 * 横轴（考虑字符对应的ASCII码）97，98，99，100，101，102，103，104
 * 那么纵轴为奇数，横轴也为奇数的方块为黑色；纵偶，奇偶也是黑色
 * 其他两种情况为白色。
 * (s[0] ^ s[1])&1：考虑最后一位是否相同，为1，证明不同，是白色
 * 为0，证明相同，是黑色
 */
/**
 * @author 灵茶山艾府
 */
bool checkTwoChessboards(string s, string t) {
    return ((s[0] ^ s[1]) & 1) == ((t[0] ^ t[1]) & 1);
}

bool checkTwoChessboards_my(string s1, string s2) {
    int board[8][8];
    // board初始化全为0，表示白色
    // 对黑色块赋值为1
    memset(board, 0, sizeof(board));
    // size_t不能为负数，当i--为-1时，i的值为SIZE_MAX，所以发生segment fault
    for (int i = 7; i >= 0; i--) {
        int j = 0;
        if (i & 1) {
            j = 1;
        }
        for (; j <= 7; j += 2) {
            board[i][j] = 1;
        }
    }
    return board[s1[1] - '1'][s1[0] - 'a'] == board[s2[1] - '1'][s2[0] - 'a'];
}

int main() {
    string s1("a1");
    string s2("c3");
    checkTwoChessboards(s1, s2);
    return 0;
}