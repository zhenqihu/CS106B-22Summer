/*
 * TODO: remove and replace this file header comment
 * This is a .cpp file you will edit and turn in.
 * Remove starter comments and add your own
 * comments on each function and on complex code sections.
 */
#include "testing/SimpleTest.h"
#include "strlib.h"
#include "filelib.h"
#include "simpio.h"
#include <fstream>
#include <cctype>
#include <string>
#include "vector.h"
using namespace std;

/* This function is intended to return a string which
 * contains only the letter characters from the original
 * (all non-letter characters are removed)
 *
 * WARNING: The provided code is buggy!
 *
 * Use unit tests to identify which inputs to this function
 * are incorrectly handled. Then, remove this comment and
 * replace it with a description of the bug you fixed.
 */
string removeNonLetters(string s) {
    string result = "";
    for (int i = 0; i < s.length(); i++) {
        if (isalpha(s[i])) {
            result += s[i];
        }
    }
    return result;
}

/* Encode each letter as a digit */
char encodeLetter(char c){
    switch (c) {
    case 'A':
    case 'E':
    case 'I':
    case 'O':
    case 'U':
    case 'H':
    case 'W':
    case 'Y':
        return '0';
    case 'B':
    case 'F':
    case 'P':
    case 'V':
        return '1';
    case 'C':
    case 'G':
    case 'J':
    case 'K':
    case 'Q':
    case 'S':
    case 'X':
    case 'Z':
        return '2';
    case 'D':
    case 'T':
        return '3';
    case 'L':
        return '4';
    case 'M':
    case 'N':
        return '5';
    case 'R':
        return '6';
    default:
        throw "Incorrect Letter";
    }
}

/* Encode the whole string to digits */
string encodeString(string s) {
    string result = "";
    for (int i = 0; i < s.length(); i++) {
        result = result + encodeLetter(toUpperCase(s[i]));
    }
    return result;
}

/* Coalesce adjacent duplicate digits from the code */
string colesceAdjacent(string s) {
    char adjacent = s[0];
    string result = charToString(s[0]);
    for (int i = 1; i < s.length(); i++) {
        if (s[i] == adjacent) {
            continue;
        } else {
            result = result + s[i];
            adjacent = s[i];
        }
    }
    return result;
}

/* Remove all zeros from the code */
string removeZero(string s) {
    string result = "";
    for (int i = 0; i < s.length(); i++) {
        if (s[i] == '0') {
            continue;
        } else {
            result = result + s[i];
        }
    }
    return result;
}

/* Make the code exactly length 4 by padding with zeros or truncating the excess */
string makeLengthFour(string s) {
    while (s.length() < 4) {
        s = s + "0";
    }
    if (s.length() > 4) {
        s = s.substr(0, 4);
    }
    return s;
}


/* Implementing the Soundex algorithm */
string soundex(string s) {
    string result = removeNonLetters(s);
    if (result.length() == 0) {
        return "";
    } else {
        char first = toUpperCase(result[0]);
        result = encodeString(result);
        result = colesceAdjacent (result);
        result = first + result.substr(1);
        result = removeZero(result);
        result = makeLengthFour(result);
        return result;
    }
}


/* TODO: Replace this comment with a descriptive function
 * header comment.
 */
void soundexSearch(string filepath) {
    // The proivded code opens the file with the given name
    // and then reads the lines of that file into a vector.
    ifstream in;
    Vector<string> databaseNames;

    if (openFile(in, filepath)) {
        readEntireFile(in, databaseNames);
    }
    cout << "Read file " << filepath << ", "
         << databaseNames.size() << " names found." << endl;

    // The names in the database are now stored in the provided
    // vector named databaseNames

    /* TODO: Fill in the remainder of this function. */
    while (true) {
        string surname = getLine("Enter a surname (RETURN to quit)");
        if (surname == "") {
            cout << "All done!" << endl;
            break;
        }
        string soundexCode = soundex(surname);
        cout << "Soundex code is " << soundexCode << endl;
        Vector<string> matchNames;
        for (string name: databaseNames) {
            if (soundexCode == soundex(name)) {
                matchNames.add(name);
            }
        }
        matchNames.sort();
        cout << "Matches from database: " <<matchNames << endl;
    }
}


/* * * * * * Test Cases * * * * * */


PROVIDED_TEST("Test removing puntuation, digits, and spaces") {
    string s = "O'Hara";
    string result = removeNonLetters(s);
    EXPECT_EQUAL(result, "OHara");
    s = "Planet9";
    result = removeNonLetters(s);
    EXPECT_EQUAL(result, "Planet");
    s = "tl dr";
    result = removeNonLetters(s);
    EXPECT_EQUAL(result, "tldr");
}


PROVIDED_TEST("Sample inputs from handout") {
    EXPECT_EQUAL(soundex("Curie"), "C600");
    EXPECT_EQUAL(soundex("O'Conner"), "O256");
}

PROVIDED_TEST("hanrahan is in lowercase") {
    EXPECT_EQUAL(soundex("hanrahan"), "H565");
}

PROVIDED_TEST("DRELL is in uppercase") {
    EXPECT_EQUAL(soundex("DRELL"), "D640");
}

PROVIDED_TEST("Liu has to be padded with zeros") {
    EXPECT_EQUAL(soundex("Liu"), "L000");
}

PROVIDED_TEST("Tessier-Lavigne has a hyphen") {
    EXPECT_EQUAL(soundex("Tessier-Lavigne"), "T264");
}

PROVIDED_TEST("Au consists of only vowels") {
    EXPECT_EQUAL(soundex("Au"), "A000");
}

PROVIDED_TEST("Egilsdottir is long and starts with a vowel") {
    EXPECT_EQUAL(soundex("Egilsdottir"), "E242");
}

PROVIDED_TEST("Jackson has three adjcaent duplicate codes") {
    EXPECT_EQUAL(soundex("Jackson"), "J250");
}

PROVIDED_TEST("Schwarz begins with a pair of duplicate codes") {
    EXPECT_EQUAL(soundex("Schwarz"), "S620");
}

PROVIDED_TEST("Van Niekerk has a space between repeated n's") {
    EXPECT_EQUAL(soundex("Van Niekerk"), "V526");
}

PROVIDED_TEST("Wharton begins with Wh") {
    EXPECT_EQUAL(soundex("Wharton"), "W635");
}

PROVIDED_TEST("Ashcraft is not a special case") {
    // Some versions of Soundex make special case for consecutive codes split by hw
    // We do not make this special case, just treat same as codes split by vowel
    EXPECT_EQUAL(soundex("Ashcraft"), "A226");
}

// TODO: add your test cases here
STUDENT_TEST("Additional tests for removeNonLetters"){
    string s = "_rlds";
    string result = removeNonLetters(s);
    EXPECT_EQUAL(result, "rlds");
}

