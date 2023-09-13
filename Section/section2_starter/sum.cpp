/*
 * CS106B Section Handout Test Harness: Section 1
 * ----------------------------------------------
 * These problems have been galvanized from years of
 * section handouts that have been worked on by numerous
 * instructors and TA's. Codified by Trip Master and Nick
 * Bowman for CS106B Spring 2020.
 *
 * A huge thank you to Keith Schwarz and Julie Zelenski
 * for creating an amazing testing harness!
 */
#include <iostream>
#include "grid.h"
#include "testing/SimpleTest.h"
#include "testing/TextUtils.h"
#include "filelib.h"
using namespace std;

/*
 * Sum Numbers (Code Write)
 * ----------------------------------
 * Write a program to read through a given file and sum
 * all of the numbers in the file. You can assume that numbers
 * will be composed entirely of numerical digits, optionally
 * preceded by a single negative sign.
 */
bool isNumber(string word) {
    if (word.size() > 0 && word[0] == '-') {
        word = word.substr(1);
    }
    if (word == "") {
        return false;
    }
    for (char ch : word) {
        if (!isdigit(ch)) {
            return false;
        }
    }
    return true;
}

int sumNumbers(string filename){
    /* The following code reads data from the file into a Vector
     * of strings representing the lines of the file.
     */
    ifstream in;
    // nonexistent file should sum to zero
    if (!openFile(in, filename)) {
        return 0;
    }
    // read file to vector lines
    Vector<string> lines;
    readEntireFile(in, lines);

    int sum = 0;
    for (string line : lines) {
        Vector<string> tokens = stringSplit(line, " ");
        for (string token : tokens) {
            if (isNumber(token)) {
                sum += stringToInteger(token);
            }
        }
    }
    return sum;
}


/* * * * * Provided Tests Below This Point * * * * */
PROVIDED_TEST("given file in handout"){
    EXPECT_EQUAL(sumNumbers("res/numbers.txt"), 42);
}

PROVIDED_TEST("Nonexistent file should sum to zero") {
    EXPECT_EQUAL(sumNumbers("nonexistent_filename"), 0);
}

