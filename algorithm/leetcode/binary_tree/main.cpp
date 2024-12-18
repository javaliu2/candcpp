#include <iostream>
#include <vector>

#include "binarytree.h"
using namespace std;

int main() {
    // vector<int> nums = {1, -1, 2, 3, 4};
    vector<int> nums = {1,2,3,5,6,7,8,9};
    BinaryTree tree(nums);
    tree.print(3);
    return 0;
}