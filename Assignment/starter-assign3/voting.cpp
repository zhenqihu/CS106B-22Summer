// TODO: remove and replace this file header comment
// This is a .cpp file you will edit and turn in.
// Remove starter comments and add your own
// comments on each function and on complex code sections.
#include <iostream>    // for cout, endl
#include <string>      // for string class
#include "voting.h"
#include "testing/SimpleTest.h"
using namespace std;

int sumVector(Vector<int>& vec) {
    int sum = 0;
    for (int elem : vec) {
        sum += elem;
    }
    return sum;
}

void computePowerIndexesHelper(int& target, Vector<int>& blocks, int& critical, int& majority, int index, int sofar) {
    if (index == blocks.size()) {
        if (sofar < majority && sofar + target >= majority) {
            critical += 1;
        }
    } else {
        computePowerIndexesHelper(target, blocks, critical, majority, index + 1, sofar + blocks[index]);
        computePowerIndexesHelper(target, blocks, critical, majority, index + 1, sofar);
    }
}

// TODO: Add a function header comment here to explain the
// behavior of the function and how you implemented this behavior
Vector<int> computePowerIndexes(Vector<int>& blocks)
{
    Vector<int> result;
    // calculate the strict majority of votes
    int majority = sumVector(blocks) / 2 + 1;
    // recursion
    for (int index = 0; index < blocks.size(); index++) {
        int target = blocks[index];
        int critical = 0;
        blocks.remove(index);
        computePowerIndexesHelper(target, blocks, critical, majority, 0, 0);
        blocks.insert(index, target);
        result.add(critical);
    }
    // percentage
    int sumResult = sumVector(result);
    for (int i = 0; i < result.size(); i++){
        result[i] = (100 * result[i]) / sumResult;
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

/* * * * * * Student Cases * * * * * */
STUDENT_TEST("Time power index operation (Big O: 2^n)") {
    Vector<int> blocks1;
    for (int i = 0; i < 15; i++) {
        blocks1.add(randomInteger(1, 10));
    }
    TIME_OPERATION(blocks1.size(), computePowerIndexes(blocks1));

    Vector<int> blocks2;
    for (int i = 0; i < 16; i++) {
        blocks2.add(randomInteger(1, 10));
    }
    TIME_OPERATION(blocks2.size(), computePowerIndexes(blocks2));

    Vector<int> blocks3;
    for (int i = 0; i < 17; i++) {
        blocks3.add(randomInteger(1, 10));
    }
    TIME_OPERATION(blocks3.size(), computePowerIndexes(blocks3));

    Vector<int> blocks4;
    for (int i = 0; i < 18; i++) {
        blocks4.add(randomInteger(1, 10));
    }
    TIME_OPERATION(blocks4.size(), computePowerIndexes(blocks4));
}
