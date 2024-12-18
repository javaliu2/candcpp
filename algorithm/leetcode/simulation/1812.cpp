#include <string>
using namespace std;

class Solution {
public:
    bool squareIsWhite(string s) {
        int h = s[0] - 'a', v = s[1] - '1';
        if ((h ^ v) & 1) {
            return true;
        }
        return false;
    }
};