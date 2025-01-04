#include <string>
#include <algorithm>
#include <bitset>

using namespace std;
class Solution {
public:
    string decimal2binary(int num) {
        string s;
        while (num) {
            s.append(to_string(num % 2));
            num /= 2;
        }
        reverse(s.begin(), s.end());
        return s;
    }
    string convertDateToBinary(string date) {
        int year, month, day;
        year = (date[0]-'0') * 1000 + (date[1]-'0') * 100 + (date[2]-'0') * 10 + (date[3]-'0');
        month = (date[5]-'0') * 10 + (date[6]-'0');
        day = (date[8]-'0') * 10 + (date[9]-'0');
        string year_str = decimal2binary(year);
        string month_str = decimal2binary(month);
        string day_str = decimal2binary(day);
        return year_str + "-" + month_str + "-" + day_str;
    }
};
/**
 * @author 灵神
 */
class Solution2 {
public:
    string convertDateToBinary(string date) {
        auto bin = [](int x) -> string {
            string s = bitset<32>(x).to_string();
            return s.substr(s.find('1'));
        };
        return bin(stoi(date.substr(0, 4))) + "-" +
               bin(stoi(date.substr(5, 2))) + "-" +
               bin(stoi(date.substr(8, 2)));
    }
};