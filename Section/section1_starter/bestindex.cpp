/*
 * CS106B Section Handout Test Harness: Section 1
 * ----------------------------------------------
 * These problems have been galvanized from years of
 * section handouts that have been worked on by numerous
 * instructors and TA's. Originally codified by Trip Master
 * and Nick Bowman for CS106B Spring 2020.
 *
 * A huge thank you to Keith Schwarz and Julie Zelenski
 * for creating an amazing testing harness!
 */

#include <iostream>
#include "string.h"
#include "testing/SimpleTest.h"
#include "testing/TextUtils.h"
using namespace std;

int getMatches (string s1, string s2);

bool match (char c1, char c2){
    if (c1 == 'A' && c2 == 'T'){
        return true;
    }
    else if (c1 == 'T' && c2 == 'A'){
        return true;
    }
    else if (c1 == 'C' && c2 == 'G'){
        return true;
    }
    else if (c1 == 'G' && c2 == 'C'){
        return true;
    }
    else {
        return false;
    }
}

/**
 * @brief bestIndexInSequence - returns the index within
 * SEQUENCE with the most base-pair matches
 * @param sequence - a string sequence of ATCG bases.
 * @param sample - a smaller sequence of ATCG bases that
 * we are comparing against subsequences of SEQUENCE to
 * find the index that results in the most matches.
 * @return - the integer value signifying the index with
 * the most base pair matches.
 *
 *
 * As a reference, A matches with T, C matches with G
 */
int bestIndexInSequence (string sequence, string sample) {
    int bestMatches = -1;
    int bestIndex = 0;
    for (int i = 0; i < (sequence.length() - sample.length()); i++) {
        int numMatches = getMatches(sequence.substr(i, sample.length()), sample);
        if (numMatches >= bestMatches) {
            bestIndex = i;
            bestMatches = numMatches;
        }
    }
    return bestIndex;
}

/**
 * @brief getMatches - returns the number of ATCG matches between s1 and s2.
 * Assumes s1 and s2 are the same length.
 * @param s1 - the first string, representing a portion of SEQUENCE.
 * @param s2 - the second string, equivalent to SAMPLE.
 * @return - the integer value signifying how many AT or CG pairs exist between
 * the two strings.
 */
int getMatches (string s1, string s2) {
    int numMatches = 0;
    for (int i = 0; i < s1.length(); i++) {
        if (match(s1[i], s2[i])) numMatches++;
    }

    return numMatches;
}

/* * * * * Tests Below This Point * * * * */

STUDENT_TEST("Verify getMatches identifies all kinds of matches."){

    // Case 1: A simple 4-for-4.
    string s1 = "ATCG";
    string s2 = "TAGC";
    EXPECT_EQUAL(getMatches(s1,s2), 4);

    // Case 1.5: Make sure order doesn't matter. This would
    // come into play if the writer did some hard-codey stuff.
    EXPECT_EQUAL(getMatches(s2,s1), 4);

    // Case 2: An example with mismatches.
    string s3 = "AAAA";
    EXPECT_EQUAL(getMatches(s1, s3), 1);
}

STUDENT_TEST("Test that bestIndexInSequence does not go off the SEQUENCE string."){
    // Use a modified example from the handout, which could produce the
    // wrong answer if getMatches does something incorrect with the edge case.
    string s1 = "TTCTTC";
    string s2 = "AGT";
    EXPECT_EQUAL(bestIndexInSequence(s1, s2), 1);
}

STUDENT_TEST("Verify bestIndexInSequence actually returns the best index."){

    // Case 1: Perfect match.
    string s1 = "TCGATCAGTCCC";
    string s2 = "AGT";
    EXPECT_EQUAL(bestIndexInSequence(s1, s2), 4);

    // Case 2: Imperfect match, single best index.
    string s3 = "TCTTC";
    string s4 = "AGT";
    EXPECT_EQUAL(bestIndexInSequence(s3, s4), 0);

    // Case 3: Multiple possible best indices.
    string s5 = "TCGATCAGTCCTCAC";
    string s6 = "AGT";
    bool firstMatch = bestIndexInSequence(s5, s6) == 4;
    bool secondMatch = bestIndexInSequence(s5, s6) == 11;
    // A finer point -- you can use EXPECT wherever you can use
    // EXPECT_EQUAL, but sometimes it makes more sense to use one
    // or the other.
    EXPECT(firstMatch || secondMatch);
}

