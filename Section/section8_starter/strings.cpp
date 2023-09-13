/*
 * CS106B Section Handout Test Harness: Section 8
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
#include <string>
#include "testing/SimpleTest.h"
#include "error.h"
using namespace std;

/*
 * String Review (Code Writing)
 * --------------------------------------------------------
 * Write a function that reverses a string “in-place.” That is,
 * you should take the string to reverse as a reference parameter
 * and modify it so that it ends up holding its reverse. Your
 * function should use only O(1) auxiliary space.
 *
 * Then, imagine you have a string containing a bunch of words
 * from a sentence.Here’s a nifty little algorithm for reversing
 * the order of the words in the sentence: reverse each individual
 * string in the sentence, then reverse the entire resulting
 * string. (Try it – it works!) Go and code a function that
 * accomplishes this task and uses only O(1) auxiliary storage space.
 */

void reverseInPlace(string& str) {
    for (int i = 0; i < str.length() / 2; i++) {
        char temp = str[i];
        str[i] = str[str.length() - 1 -i];
        str[str.length() - 1 -i] = temp;
    }
}

void reverseInPlace(string& str, int start, int end) {
    for (int i = 0; i < (end - start) / 2; i++) {
        char temp = str[start + i];
        str[start + i] = str[end - 1 - i];
        str[end - 1 - i] = temp;
    }
}

void reverseWordOrderingIn(string& sentence) {
    int start = 0;
    for (int i = 0; i < sentence.length(); i++) {
        if (sentence[i] == ' ') {
            reverseInPlace(sentence, start, i);
            start = i + 1;
        }
    }

    /* We need to account for the fact that there might be a word
    * flush up at the end of the sentence.
    */
    reverseInPlace(sentence, start, sentence.length());

    /* Now reverse everything. */
    reverseInPlace(sentence, 0, sentence.length());
}

PROVIDED_TEST("Simple Tests of Reverse String") {
    string s = "Hello, World!";
    string soln = "!dlroW ,olleH";
    reverseInPlace(s);
    EXPECT_EQUAL(s, soln);

    s = "";
    soln = "";
    reverseInPlace(s);
    EXPECT_EQUAL(s, soln);

    s = "I";
    soln = "I";
    reverseInPlace(s);
    EXPECT_EQUAL(s, soln);
}

PROVIDED_TEST("Simple Tests of Reverse Sentence") {
    string s = "Hello, World!";
    string soln = "World! Hello,";
    reverseWordOrderingIn(s);
    EXPECT_EQUAL(s, soln);

    s = "Congrats on finishing CS106B";
    soln = "CS106B finishing on Congrats";
    reverseWordOrderingIn(s);
    EXPECT_EQUAL(s, soln);

    s = "";
    soln = "";
    reverseWordOrderingIn(s);
    EXPECT_EQUAL(s, soln);

    s = "I";
    soln = "I";
    reverseWordOrderingIn(s);
    EXPECT_EQUAL(s, soln);
}
