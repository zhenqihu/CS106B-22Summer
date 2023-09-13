/*
 * TODO: remove and replace this file header comment
 * You will edit and turn in this file.
 * Remove starter comments and add your own
 * comments on each function and on complex code sections.
 */
#include <iostream>    // for cout, endl
#include "queue.h"
#include "testing/SimpleTest.h"
using namespace std;

/*
 * TODO: Replace this comment with a descriptive function
 * header comment.
 */
Queue<int> binaryMerge(Queue<int> a, Queue<int> b) {
    Queue<int> result;
    int chosen;
    double last = -std::numeric_limits<double>::infinity();
    while (!a.isEmpty() || !b.isEmpty()) {
        if (a.isEmpty()) {
            chosen = b.dequeue();
        } else if (b.isEmpty()) {
            chosen = a.dequeue();
        } else if (a.peek() <= b.peek()) {
            chosen = a.dequeue();
        } else {
            chosen = b.dequeue();
        }
        if (chosen < last) {
            error("Unsorted Queue");
        } else {
            result.enqueue(chosen);
            last = chosen;
        }
    }
    return result;
}

/*
 * This function assumes correct functionality of the previously
 * defined binaryMerge function and makes use of this function to
 * iteratively merge a collection of sequences, starting off with
 * an empty sequence and progressively merging in one sequence at
 * a time. This function is provided for you fully implemented –
 * it does not need to modified at all.
 */
Queue<int> naiveMultiMerge(Vector<Queue<int>>& all) {
    Queue<int> result;

    for (Queue<int>& q : all) {
        result = binaryMerge(q, result);
    }
    return result;
}

/*
 * TODO: Replace this comment with a descriptive function
 * header comment.
 */
Queue<int> recMultiMerge(Vector<Queue<int>>& all) {
    Queue<int> result;
    if (all.size() == 0) {
        result = {};
    } else if (all.size() == 1) {
        result = all[0];
    } else {
        Vector<Queue<int>> leftVec = all.subList(0, all.size() / 2);
        Vector<Queue<int>> rightVec = all.subList(all.size() / 2);
        result = binaryMerge(recMultiMerge(leftVec), recMultiMerge(rightVec));
    }
    return result;
}


/* * * * * * Test Cases * * * * * */

Queue<int> createSequence(int size);
void distribute(Queue<int> input, Vector<Queue<int>>& all);

PROVIDED_TEST("binaryMerge, two short sequences") {
    Queue<int> a = {2, 4, 5};
    Queue<int> b = {1, 3, 3};
    Queue<int> expected = {1, 2, 3, 3, 4, 5};
    EXPECT_EQUAL(binaryMerge(a, b), expected);
    EXPECT_EQUAL(binaryMerge(b, a), expected);
}

PROVIDED_TEST("naiveMultiMerge, small collection of short sequences") {
    Vector<Queue<int>> all = {{3, 6, 9, 9, 100},
                             {1, 5, 9, 9, 12},
                             {5},
                             {},
                             {-5, -5},
                             {3402}
                            };
    Queue<int> expected = {-5, -5, 1, 3, 5, 5, 6, 9, 9, 9, 9, 12, 100, 3402};
    EXPECT_EQUAL(naiveMultiMerge(all), expected);
}

PROVIDED_TEST("recMultiMerge, compare to naiveMultiMerge") {
    int n = 20;
    Queue<int> input = createSequence(n);
    Vector<Queue<int>> all(n);
    distribute(input, all);
    EXPECT_EQUAL(recMultiMerge(all), naiveMultiMerge(all));
}

PROVIDED_TEST("Time binaryMerge operation") {
    int n = 1000000;
    Queue<int> a = createSequence(n);
    Queue<int> b = createSequence(n);
    TIME_OPERATION(a.size() + b.size(), binaryMerge(a, b));
}

PROVIDED_TEST("Time naiveMultiMerge operation") {
    int n = 11000;
    int k = n/10;
    Queue<int> input = createSequence(n);
    Vector<Queue<int>> all(k);
    distribute(input, all);
    TIME_OPERATION(input.size(), naiveMultiMerge(all));
}
PROVIDED_TEST("Time recMultiMerge operation") {
    int n = 90000;
    int k = n/10;
    Queue<int> input = createSequence(n);
    Vector<Queue<int>> all(k);
    distribute(input, all);
    TIME_OPERATION(input.size(), recMultiMerge(all));
}


/* Test helper to fill queue with sorted sequence */
Queue<int> createSequence(int size) {
    Queue<int> q;
    for (int i = 0; i < size; i++) {
        q.enqueue(i);
    }
    return q;
}

/* Test helper to distribute elements of sorted sequence across k sequences,
   k is size of Vector */
void distribute(Queue<int> input, Vector<Queue<int>>& all) {
    while (!input.isEmpty()) {
        all[randomInteger(0, all.size()-1)].enqueue(input.dequeue());
    }
}

/* * * * * * Student Tests * * * * * */

STUDENT_TEST("binaryMerge, two short sequences (different length)") {
    Queue<int> a = {2, 4, 5};
    Queue<int> b = {1, 3, 3, 7, 8, 10};
    Queue<int> expected = {1, 2, 3, 3, 4, 5, 7, 8, 10};
    EXPECT_EQUAL(binaryMerge(a, b), expected);
    EXPECT_EQUAL(binaryMerge(b, a), expected);
}

STUDENT_TEST("binaryMerge, two short sequences (one is empty)") {
    Queue<int> a = {2, 4, 5};
    Queue<int> b = {};
    Queue<int> expected = {2, 4, 5};
    EXPECT_EQUAL(binaryMerge(a, b), expected);
    EXPECT_EQUAL(binaryMerge(b, a), expected);
}

STUDENT_TEST("binaryMerge, unsorted sequences") {
    Queue<int> a = {4, 2, 5}; // unsorted
    Queue<int> b = {1, 3, 3};
    EXPECT_ERROR(binaryMerge(a, b));
    EXPECT_ERROR(binaryMerge(b, a));
}

STUDENT_TEST("binaryMerge, analyze the time") {
    for (int n = 1000000; n <= 10000000; n = 2 * n) {
        Queue<int> a = createSequence(n);
        Queue<int> b = createSequence(n);
        TIME_OPERATION(a.size() + b.size(), binaryMerge(a, b));
    }
}

STUDENT_TEST("naiveMultiMerge, empty Vector") {
    Vector<Queue<int>> all = {};
    Queue<int> expected = {};
    EXPECT_EQUAL(naiveMultiMerge(all), expected);
}

STUDENT_TEST("naiveMultiMerge, Vector include several empty sequences") {
    Vector<Queue<int>> all = {{3, 6, 9, 9, 100}, {}, {}};
    Queue<int> expected = {3, 6, 9, 9, 100};
    EXPECT_EQUAL(naiveMultiMerge(all), expected);
}

STUDENT_TEST("Time naiveMultiMerge operation, loop for n, keeping k fixed") {
    int k = 1000;
    for (int n = 10000; n <= 100000; n = n * 2){
        Queue<int> input = createSequence(n);
        Vector<Queue<int>> all(k);
        distribute(input, all);
        TIME_OPERATION(n, naiveMultiMerge(all));
    }
}

STUDENT_TEST("Time naiveMultiMerge operation, loop for k, keeping n fixed") {
    int n = 100000;
    for (int k = 100; k <= 1000; k = k * 2) {
        Queue<int> input = createSequence(n);
        Vector<Queue<int>> all(k);
        distribute(input, all);
        TIME_OPERATION(k, naiveMultiMerge(all));
    }
}

STUDENT_TEST("recMultiMerge, compare to naiveMultiMerge") {
    int n = 10000;
    Queue<int> input = createSequence(n);
    Vector<Queue<int>> all(n);
    distribute(input, all);
    EXPECT_EQUAL(recMultiMerge(all), naiveMultiMerge(all));
}

STUDENT_TEST("Time recMultiMerge operation, loop for n, keeping k fixed") {
    int k = 100;
    for (int n = 100000; n <= 1000000; n = n * 2){
        Queue<int> input = createSequence(n);
        Vector<Queue<int>> all(k);
        distribute(input, all);
        TIME_OPERATION(n, recMultiMerge(all));
    }
}

STUDENT_TEST("Time recMultiMerge operation, loop for k, keeping n fixed") {
    int n = 1000000;
    for (int k = 10; k <= 100000; k = k * 10) {
        Queue<int> input = createSequence(n);
        Vector<Queue<int>> all(k);
        distribute(input, all);
        TIME_OPERATION(k, recMultiMerge(all));
    }
}
