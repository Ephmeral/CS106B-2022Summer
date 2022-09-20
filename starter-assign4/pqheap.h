#pragma once
#include "testing/MemoryUtils.h"
#include "datapoint.h"

/**
 * Priority queue of DataPoints implemented using a binary heap.
 */
class PQHeap {
public:
    /**
     * Creates a new, empty priority queue.
     */
    PQHeap();

    /**
     * Cleans up all memory allocated by this priority queue.
     */
    ~PQHeap();

    /**
     * Adds a new element into the queue. This operation runs in time O(log n),
     * where n is the number of elements in the queue.
     *
     * @param element The element to add.
     */
    void enqueue(DataPoint element);

    /**
     * Removes and returns the element that is frontmost in this priority queue.
     * The frontmost element is the one with the most urgent priority. A priority
     * of 1 is more urgent than priority 2 which is more urgent than priority 7
     * and so on. If the priority queue contains two or more elements of equal
     * priority, the order those elements are dequeued is arbitrary, i.e. there
     * is no required tie-break handling.
     *
     * If the priority queue is empty, this function calls error().
     *
     * This operation must run in time O(log n).
     *
     * @return The frontmost element, which is removed from queue.
     */
    DataPoint dequeue();

    /**
     * Returns, but does not remove, the element that is frontmost.
     *
     * If the priority queue is empty, this function calls error().
     *
     * This operation must run in time O(1).
     *
     * @return frontmost element
     */
    DataPoint peek() const;

    /**
     * Returns whether this priority queue is empty.
     *
     * This operation must run in time O(1).
     *
     * @return true if contains no elements, false otherwise.
     */
    bool isEmpty() const;

    /**
     * Returns the count of elements in this priority queue.
     *
     * This operation must run in time O(1).
     *
     * @return The count of elements in the priority queue.
     */
    int size() const;

    /**
     * Removes all elements from the priority queue.
     *
     * This operation must run in time O(1).
     */
    void clear();

    /*
     * This function exists purely for testing purposes. You can have it do whatever you'd
     * like and we won't be invoking it when grading. In the past, students have had this
     * function print out the array representing the heap, or information about how much
     * space is allocated, etc. Feel free to use it as you see fit!
     */
    void printDebugInfo(std::string msg) const;

    /*
     * This function exits purely for testing purposes. It verifies
     * that the internal state of the queue is valid/consistent.
     * If a problem is detected, this function calls error().
     * If no problem, the function returns normally.
     */
    void validateInternalState() const;

private:
    /* TODO: Add whatever fields, types, and helper functions that you'd like! */

    /* While not a strict requirement, we strongly recommend implementing the
     * helper functions declared below. They will make your code much cleaner, and
     * will aid in the debuggin process.
     */
    int getParentIndex(int child) const;
    int getLeftChildIndex(int parent) const;
    int getRightChildIndex(int parent) const;

    //by Ephmeral
    bool isFull() const ;

    void realloc();

    DataPoint* _elements;   // dynamic array
    int _numAllocated;      // number of slots allocated in array
    int _numFilled;         // number of slots filled in array

    bool isValidIndex(int index);
    int smaller(int indexA, int indexB); //取两个当中优先级大的
    int properParent(int parent);
    void swap(int indexA, int indexB);
    void bubbleDown(int index);
    void bubbleUp(int index);

    /* Weird C++isms: C++ loves to make copies of things, which is usually a good thing but
     * for the purposes of this assignment requires some C++ knowledge we haven't yet covered.
     * This next line disables all copy functions to make sure you don't accidentally end up
     * debugging something that isn't your fault.
     *
     * Curious what this does? Take CS106L!
     */
    DISALLOW_COPYING_OF(PQHeap);
};
