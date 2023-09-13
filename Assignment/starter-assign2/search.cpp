// TODO: remove and replace this file header comment
// This is a .cpp file you will edit and turn in.
// Remove starter comments and add your own
// comments on each function and on complex code sections.
#include <iostream>
#include <fstream>
#include "error.h"
#include "filelib.h"
#include "map.h"
#include "search.h"
#include "set.h"
#include "strlib.h"
#include "vector.h"
#include "testing/SimpleTest.h"
#include "simpio.h"
using namespace std;


/* The cleanToken function takes in a string of characters that appears in the body text
* and returns a "cleaned" version of that token, ready to be stored in the index in its canonical form.
*
* The return value is the trimmed lowercase version, or empty string if the token is to be discarded.
*/
string cleanToken(string s)
{
    // Trim all punctuation from the beginning
    while (s.size() > 0 && ispunct(s[0])) {
        s = s.substr(1);
    }
    // Trim all punctuation from the end
    while (s.size() > 0 && ispunct(s[s.size() - 1])) {
        s = s.substr(0, s.size() - 1);
    }
    // Confirm the token contains at least one letter character
    int numLetter = 0;
    for (int i = 0; i < s.size(); i++) {
        if (isalpha(s[i])) {
            numLetter++;
            s[i] = toLowerCase(s[i]); // Convert the token to lowercase
        }
    }
    if (numLetter == 0) {
        s = "";
    }
    return s;
}

// TODO: Add a function header comment here to explain the
// behavior of the function and how you implemented this behavior
Set<string> gatherTokens(string text)
{
    Set<string> tokens;
    Vector<string> words = stringSplit(text, " ");
    for (string word : words) {
        string cleanedToken = cleanToken(word);
        if (cleanedToken.size() > 0) {
            tokens.add(cleanedToken);
        }
    }
    return tokens;
}

// TODO: Add a function header comment here to explain the
// behavior of the function and how you implemented this behavior
int buildIndex(string dbfile, Map<string, Set<string>>& index)
{
    /* The following code reads data from the file into a Vector
     * of strings representing the lines of the file.
     */
    ifstream in;

    if (!openFile(in, dbfile))
        error("Cannot open file named " + dbfile);

    Vector<string> lines;
    readEntireFile(in, lines);
    // Ensure the length of lines is even
    if (lines.size() % 2 != 0) {
        error("Invalid file");
    }
    // Numbers of pages
    int nPages = lines.size() / 2;
    // Update the inverted index
    for (int i = 0; i < lines.size(); i = i + 2) {
        string url = lines[i];
        string bodyText = lines[i + 1];
        Set<string> tokens = gatherTokens(bodyText);
        for (string token : tokens) {
            index[token].add(url);
        }
    }
    return nPages;
}

// TODO: Add a function header comment here to explain the
// behavior of the function and how you implemented this behavior
Set<string> findQueryMatches(Map<string, Set<string>>& index, string query)
{
    Set<string> result;
    Vector<string> searchTerms = stringSplit(query, " ");
    result = index[cleanToken(searchTerms[0])];
    for (int i = 1; i < searchTerms.size(); i++) {
        string word = searchTerms[i];
        if (word[0] == '+') {
            result = result.intersect(index[cleanToken(word)]);
        } else if (word[0] == '-') {
            result = result.difference(index[cleanToken(word)]);
        } else {
            result = result.unionWith(index[cleanToken(word)]);
        }
    }
    return result;
}

// TODO: Add a function header comment here to explain the
// behavior of the function and how you implemented this behavior
void searchEngine(string dbfile)
{
    cout << "Stand by while building index..." << endl;
    Map<string, Set<string>> invertIndex;
    int nPages = buildIndex(dbfile, invertIndex);
    cout << "Indexed " << nPages << " pages containing " << invertIndex.size() << " unique terms" << endl;
    while (true) {
        string query = getLine("Enter query sentence (RETURN/ENTER to quit)");
        if (query == "") {
            break;
        }
        Set<string> matchPages = findQueryMatches(invertIndex, query);
        cout << "Find " << matchPages.size() << " matching pages" << endl;
        cout << matchPages << endl;
    }
}

/* * * * * * Test Cases * * * * * */

PROVIDED_TEST("cleanToken on strings with no punctuation at beginning or end") {
    EXPECT_EQUAL(cleanToken("hello"), "hello");
    EXPECT_EQUAL(cleanToken("WORLD"), "world");
    EXPECT_EQUAL(cleanToken("CS*106B"), "cs*106b");
}

PROVIDED_TEST("cleanToken on strings with some punctuation at beginning and end") {
    EXPECT_EQUAL(cleanToken("/hello/"), "hello");
    EXPECT_EQUAL(cleanToken("~woRLD!"), "world");
}

PROVIDED_TEST("cleanToken on non-word strings (no letters)"){
    EXPECT_EQUAL(cleanToken("106"), "");
    EXPECT_EQUAL(cleanToken("~!106!!!"), "");
}

PROVIDED_TEST("gatherTokens from simple string") {
    Set<string> expected = {"go", "gophers"};
    EXPECT_EQUAL(gatherTokens("go go go gophers"), expected);
}

PROVIDED_TEST("gatherTokens correctly cleans tokens") {
    Set<string> expected = {"i", "love", "cs*106b"};
    EXPECT_EQUAL(gatherTokens("I _love_ CS*106B!"), expected);
}

PROVIDED_TEST("gatherTokens from seuss, 5 unique words, mixed case, punctuation") {
    Set<string> tokens = gatherTokens("One Fish Two Fish *Red* fish Blue fish ** 10 RED Fish?");
    EXPECT_EQUAL(tokens.size(), 5);
    EXPECT(tokens.contains("fish"));
    EXPECT(!tokens.contains("Fish"));
}

PROVIDED_TEST("buildIndex from tiny.txt, 4 pages, 11 unique tokens") {
    Map<string, Set<string>> index;
    int nPages = buildIndex("res/tiny.txt", index);
    EXPECT_EQUAL(nPages, 4);
    EXPECT_EQUAL(index.size(), 11);
    EXPECT(index.containsKey("fish"));
}

PROVIDED_TEST("findQueryMatches from tiny.txt, single word query") {
    Map<string, Set<string>> index;
    buildIndex("res/tiny.txt", index);
    Set<string> matchesRed = findQueryMatches(index, "red");
    EXPECT_EQUAL(matchesRed.size(), 2);
    EXPECT(matchesRed.contains("www.dr.seuss.net"));
    Set<string> matchesHippo = findQueryMatches(index, "hippo");
    EXPECT(matchesHippo.isEmpty());
}

PROVIDED_TEST("findQueryMatches from tiny.txt, compound queries") {
    Map<string, Set<string>> index;
    buildIndex("res/tiny.txt", index);
    Set<string> matchesRedOrFish = findQueryMatches(index, "red fish");
    EXPECT_EQUAL(matchesRedOrFish.size(), 4);
    Set<string> matchesRedAndFish = findQueryMatches(index, "red +fish");
    EXPECT_EQUAL(matchesRedAndFish.size(), 1);
    Set<string> matchesRedWithoutFish = findQueryMatches(index, "red -fish");
    EXPECT_EQUAL(matchesRedWithoutFish.size(), 1);
}


/* * * * * * Student Test Cases * * * * * */
STUDENT_TEST("cleanToken on strings with no punctuation at beginning or end") {
    EXPECT_EQUAL(cleanToken("hell10"), "hell10");
    EXPECT_EQUAL(cleanToken("WORLD's"), "world's");
    EXPECT_EQUAL(cleanToken("CS*106B**"), "cs*106b");
}

STUDENT_TEST("cleanToken on strings with some punctuation at beginning and end") {
    EXPECT_EQUAL(cleanToken("/hello's"), "hello's");
    EXPECT_EQUAL(cleanToken("~w10"), "w10");
}

STUDENT_TEST("cleanToken on non-word strings (no letters)"){
    EXPECT_EQUAL(cleanToken(""), "");
    EXPECT_EQUAL(cleanToken("20-2"), "");
}

STUDENT_TEST("gatherTokens correctly cleans tokens") {
    Set<string> expected = {"i", "love", "cs*106b"};
    EXPECT_EQUAL(gatherTokens("I _love_ 23 CS*106B! LoVE"), expected);
}

STUDENT_TEST("buildIndex from website.txt, 32 pages") {
    Map<string, Set<string>> index;
    TIME_OPERATION(32, buildIndex("res/website.txt", index));
    int nPages = buildIndex("res/website.txt", index);
    EXPECT_EQUAL(nPages, 32);
    EXPECT(index.containsKey("today"));
    EXPECT(index["today"].contains("http://cs106b.stanford.edu/class/cs106b/lectures/05-grid-stack-queue/"));
}
