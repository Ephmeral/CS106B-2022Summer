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
    if (a.size() == 0) {
        return b;
    } else if (b.size() == 0) {
        return a;
    }
    Queue<int> result;
    /* TODO: Implement this function. */
    int last = a.peek() < b.peek() ? a.peek() : b.peek();
    while (!a.isEmpty() && !b.isEmpty()) {
        int dq;
        if (a.peek() < b.peek()) {
            dq = a.dequeue();
        } else {
            dq = b.dequeue();
        }
        if (dq < last) {
            error("the order is not correct!");
        }
        result.enqueue(dq);
        last = dq;
    }
    while (!a.isEmpty()) {
        if (a.peek() < last) {
            error("the order is not correct!");
        }
        last = a.peek();
        result.enqueue(a.dequeue());
    }
    while (!b.isEmpty()) {
        if (b.peek() < last) {
            error("the order is not correct!");
        }
        last = b.peek();
        result.enqueue(b.dequeue());
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
        return result;
    } else if (all.size() == 1) {
        return all[0];
    }
    /* TODO: Implement this function. */
    int n = all.size(), k = n / 2;
    Vector<Queue<int>> left = all.subList(0, k);
    Vector<Queue<int>> right = all.subList(k, n - k);
    return binaryMerge(recMultiMerge(left), recMultiMerge(right));
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

STUDENT_TEST("test for binaryMerge") {
    Queue<int> a = {1, 2, 4, 5, 6, 9, 10, 15};
    Queue<int> b = {1, 3, 3, 5, 7, 8, 8, 20};
    Queue<int> expected = {1, 1, 2, 3, 3, 4, 5, 5, 6, 7, 8, 8, 9, 10, 15, 20};
    EXPECT_EQUAL(binaryMerge(a, b), expected);
    EXPECT_EQUAL(binaryMerge(b, a), expected);


    Queue<int> c = {1, 2, 4, 5, 6, 9, 30, 15};
    Queue<int> d = {1, 3, 3, 5, 7, 8, 8, 20};
    EXPECT_ERROR(binaryMerge(c, d));
}

STUDENT_TEST("test for binaryMerge of one is short") {
    Queue<int> c = {1, 2, 4, 5, 6, 9, 30, 15};
    Queue<int> d = {1, 3, 3};
    EXPECT_ERROR(binaryMerge(c, d));
}

STUDENT_TEST("naiveMultiMerge, small collection of 0 sequences") {
    Vector<Queue<int>> all = {};
    Queue<int> expected = {};
    EXPECT_EQUAL(naiveMultiMerge(all), expected);
}
