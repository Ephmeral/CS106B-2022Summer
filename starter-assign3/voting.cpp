// TODO: remove and replace this file header comment
// This is a .cpp file you will edit and turn in.
// Remove starter comments and add your own
// comments on each function and on complex code sections.
#include <iostream>    // for cout, endl
#include <string>      // for string class
#include "voting.h"
#include "testing/SimpleTest.h"
using namespace std;

Vector<int> path;


void backtrack(Vector<int> &blocks, int start, int sum, int target, Vector<int> &result) {
    if (start >= blocks.size()) {
        if (sum <= target) return;
        for (auto id : path) {
            if (sum - blocks[id] <= target) {
                result[id]++;
            }
        }
//        cout << "in end " << path.size() << " sum = " << sum << endl;
        return;
    }
    if (sum > target) {
//        int left = blocks.size() - start;
//        int k = 1 << left;
        for (auto id : path) {
            if (sum - blocks[id] <= target) {
                result[id]++;
            }
        }
//        cout << "in middle " << path.size() << " sum = " << sum << endl;
    }
    for (int i = start; i < blocks.size(); ++i) {
        sum += blocks[i];
        path.add(i);

        backtrack(blocks, i + 1, sum, target, result);

        path.remove(path.size() - 1);
        sum -= blocks[i];
    }
}
// TODO: Add a function header comment here to explain the
// behavior of the function and how you implemented this behavior
Vector<int> computePowerIndexes(Vector<int>& blocks)
{
    int n = blocks.size();
    Vector<int> result(n);

    int target = 0;
    for (auto block : blocks) {
        target += block;
    }
    target /= 2;
    backtrack(blocks, 0, 0, target, result);

    int sum = 0;
//    cout << "res=======" << endl;
    for (auto res : result) {
//        cout << res << " ";
        sum += res;
    }
//    cout << endl;
//    cout << "res========" << endl;
    for (int i = 0; i < n; ++i) {
        result[i] = result[i] *100 / sum;
    }
    return result;
}

/* * * * * * Test Cases * * * * * */

PROVIDED_TEST("Test power index, blocks 50-49-1") {
    Vector<int> blocks = {50, 49, 1};
    Vector<int> expected = {60, 20, 20};
    EXPECT_EQUAL(computePowerIndexes(blocks), expected);
}

PROVIDED_TEST("Test power index, blocks Hempshead 1-1-3-7-9-9") {
    Vector<int> blocks = {1, 1, 3, 7, 9, 9};
    Vector<int> expected = {0, 0, 0, 33, 33, 33};
    EXPECT_EQUAL(computePowerIndexes(blocks), expected);
}

PROVIDED_TEST("Test power index, blocks CA-TX-NY 55-38-39") {
    Vector<int> blocks = {55, 38, 29};
    Vector<int> expected = {33, 33, 33};
    EXPECT_EQUAL(computePowerIndexes(blocks), expected);
}

PROVIDED_TEST("Test power index, blocks CA-TX-GA 55-38-16") {
    Vector<int> blocks = {55, 38, 16};
    Vector<int> expected = {100, 0, 0};
    EXPECT_EQUAL(computePowerIndexes(blocks), expected);
}

PROVIDED_TEST("Test power index, blocks EU post-Nice") {
    // Estonia is one of those 4s!!
    Vector<int> blocks = {29,29,29,29,27,27,14,13,12,12,12,12,12,10,10,10,7,7,7,7,7,4,4,4,4,4,3};
    Vector<int> expected = {8, 8, 8, 8, 7, 7, 3, 3, 3, 3, 3, 3, 3, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0};
    EXPECT_EQUAL(computePowerIndexes(blocks), expected);
}

PROVIDED_TEST("Time power index operation") {
    Vector<int> blocks;
    for (int i = 0; i < 15; i++) {
        blocks.add(randomInteger(1, 10));
    }
    TIME_OPERATION(blocks.size(), computePowerIndexes(blocks));
}


