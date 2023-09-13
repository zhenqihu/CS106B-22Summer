/*
 * TODO: remove and replace this file header comment
 * You will edit and turn in this file.
 * Remove starter comments and add your own
 * comments on each function and on complex code sections.
 */
#include <iostream>    // for cout, endl
#include <string>      // for string class
#include "recursion.h"
#include "testing/SimpleTest.h"

using namespace std;

/*
 * TODO: Replace this comment with a descriptive function
 * header comment.
 */
string operatorsFrom(string str) {
    if (str.empty()) {
        return "";
    } else {
        char first = str[0];
        if (first == '{' || first == '}' || first == '[' || first == ']' || first == '(' || first == ')') {
            return first + operatorsFrom(str.substr(1));
        } else {
            return operatorsFrom(str.substr(1));
        }
    }
    return "";
}

/*
 * TODO: Replace this comment with a descriptive function
 * header comment.
 */
bool operatorsAreMatched(string ops) {
    if (ops.empty()) {
        return true;
    } else {
        if (stringContains(ops, "()")) {
            return operatorsAreMatched(stringReplace(ops, "()", ""));
        } else if (stringContains(ops, "[]")) {
            return operatorsAreMatched(stringReplace(ops, "[]", ""));
        } else if (stringContains(ops, "{}")) {
            return operatorsAreMatched(stringReplace(ops, "{}", ""));
        }
    }
    return false;
}

/*
 * The isBalanced function assumes correct implementation of
 * the above two functions operatorsFrom and operatorsMatch.
 * It uses operatorsFrom to extract the operator characters
 * from the input string and then confirms that those
 * operators are balanced by using operatorsMatch.
 * You should not modify the provided code in the isBalanced
 * function.  If the previous two functions have been implemented
 * correctly, the provided isBalanced will correctly report whether
 * the input string has balanced bracketing operators.
 */
bool isBalanced(string str) {
    string ops = operatorsFrom(str);
    return operatorsAreMatched(ops);
}


/* * * * * * Test Cases * * * * * */

PROVIDED_TEST("operatorsFrom on simple example") {
    EXPECT_EQUAL(operatorsFrom("vec[3]"), "[]");
}

PROVIDED_TEST("operatorsAreMatched on simple example") {
    EXPECT(operatorsAreMatched("{}"));
}

PROVIDED_TEST("isBalanced on example from writeup") {
    string example ="int main() { int x = 2 * (vec[2] + 3); x = (1 + random()); }";
    EXPECT(isBalanced(example));
}

PROVIDED_TEST("isBalanced on non-balanced examples from writeup") {
    EXPECT(!isBalanced("( ( [ a ] )"));
    EXPECT(!isBalanced("3 ) ("));
    EXPECT(!isBalanced("{ ( x } y )"));
}

/* * * * * * Test Cases * * * * * */

STUDENT_TEST("operatorsFrom on simple example") {
    EXPECT_EQUAL(operatorsFrom("a{b[c]d(e)f}"), "{[]()}");
}

STUDENT_TEST("operatorsAreMatched on more examples") {
    EXPECT(operatorsAreMatched("({()[]})"));
    EXPECT(!operatorsAreMatched("({()[])}"));
}

STUDENT_TEST("isBalanced on example from writeup") {
    EXPECT(isBalanced("a{b[c]d(e)f}"));
}

STUDENT_TEST("isBalanced on non-balanced examples from writeup") {
    EXPECT(!isBalanced("{ ( [ a } ])"));
    EXPECT(!isBalanced("3 ) [ 4 ] ("));
}

