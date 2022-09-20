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
#include "simpio.h"
#include "testing/SimpleTest.h"
using namespace std;


// TODO: Add a function header comment here to explain the
// behavior of the function and how you implemented this behavior
string cleanToken(string s)
{
    int i = 0, n = s.length();
    while (i < n && ispunct(s[i])) {
        ++i;
    }
    int j = n - 1;
    while (j >= 0 && ispunct(s[j])) {
        --j;
    }
    if (j < i) return "";
    string res = s.substr(i, j - i + 1);
    bool flag = false;
    for (int i = 0; i < res.length(); ++i) {
        if (!flag && isalpha(res[i])) {
            flag = true;
        }
        if (res[i] >= 'A' && res[i] <= 'Z') {
            res[i] = tolower(res[i]);
        }
    }
    if (!flag) {
        return "";
    }
    return res;
}

// TODO: Add a function header comment here to explain the
// behavior of the function and how you implemented this behavior
Set<string> gatherTokens(string text)
{
    Set<string> tokens;
    Vector<string> arr = stringSplit(text, " ");
    for (auto &str : arr) {
        string token = cleanToken(str);
        if (token != "") {
            tokens.add(token);
        }
    }
    return tokens;
}

// TODO: Add a function header comment here to explain the
// behavior of the function and how you implemented this behavior
int buildIndex(string dbfile, Map<string, Set<string>>& index)
{
    ifstream in;

    if (!openFile(in, dbfile))
        error("Cannot open file named " + dbfile);

    Vector<string> lines;
    readEntireFile(in, lines);
    int n = lines.size(), filesize = n / 2;
    for (int i = 0; i < n; i += 2) {
        Set<string> tokens = gatherTokens(lines[i + 1]);
        for (auto &token : tokens) {
            if (!index.containsKey(token)) {
                Set<string> tmpSet{lines[i]};
                index.put(token, tmpSet);
            } else {
                auto tmpSet = index.get(token);
                tmpSet.add(lines[i]);
                index.put(token, tmpSet);
            }
        }
    }
    return filesize;
}

// TODO: Add a function header comment here to explain the
// behavior of the function and how you implemented this behavior
Set<string> findQueryMatches(Map<string, Set<string>>& index, string query)
{
    Set<string> result;
    // TODO: your code here
    Vector<string> tmpArr = stringSplit(query, " ");
    int n = tmpArr.size();

    Vector<string> querys(n);
    Vector<char> sign(n, ' ');

    for (int i = 0; i < n; ++i) {
        string &q = tmpArr[i];
        if (q != "" && (q[0] == '+' || q[0] == '-')) {
            sign[i] = q[0];
        }
        querys[i] = cleanToken(q);
    }

    for (int i = 0; i < n; ++i) {
        string &q = querys[i];
        if (sign[i] == '+') {
            result.intersect(index.get(q));
        } else if (sign[i] == '-') {
            result.difference(index.get(q));
        } else {
            result.unionWith(index.get(q));
        }
    }
    return result;
}

// TODO: Add a function header comment here to explain the
// behavior of the function and how you implemented this behavior
void searchEngine(string dbfile)
{
    // TODO: your code here
    std::cout << "Stand by while building index..." << endl;

    Map<string, Set<string>> index;
    int nPages = buildIndex(dbfile, index);
    cout << "Indexed " << nPages << " pages containing "
         << index.size() << " unique terms" << endl;
    string query;
    while ((query = getLine("\nEnter query sentence (RETURN/ENTER to quit): ")) != "") {
        Set<string> result = findQueryMatches(index, query);
        cout << "Found " << result.size() << " matching pages " << endl;
        cout << result << endl;
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


// TODO: add your test cases here
STUDENT_TEST("test for cleanToken") {
    EXPECT_EQUAL(cleanToken("/hello/,.;?.."), "hello");
    EXPECT_EQUAL(cleanToken("wo!RLD!"), "wo!rld");
    EXPECT_EQUAL(cleanToken("/hel..lo/"), "hel..lo");
    EXPECT_EQUAL(cleanToken("/./."), "");

    Map<string, Set<string>> index;
    int nPages = buildIndex("res/website.txt", index);
    EXPECT_EQUAL(nPages, 32);
    EXPECT_EQUAL(index.size(), 3875);
    EXPECT(index.containsKey("that"));
    EXPECT(index.containsKey("cs106b"));

//    TIME_OPERATION()
    searchEngine("res/website.txt");
}
