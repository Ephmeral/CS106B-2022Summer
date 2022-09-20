#include "datapoint.h"
#include "testing/SimpleTest.h"
#include "strlib.h"
using namespace std;

/* A taskList is an array of DataPoints.
 * Each DataPoint is labeled with task you need to do and has an associated
 * priority of its importance
 *
 * The DataPoint struct is defined in datapoint.h if you want to inspect the
 * declaration of the struct.
 */

PROVIDED_TEST("Test valid use of array, new/delete") {
    DataPoint* taskList = new DataPoint[6]; // allocate

    for (int i = 0; i < 3; i++) {   // assign entries 0,1,2 (others uninitialized)
        taskList[i].priority = 1;  // set struct fields using dot notation
        taskList[i].label = "pset " + integerToString(i+1);
    }
    taskList[0].priority += 5;
    delete[] taskList; // deallocate
}

/* The special value nullptr is the zero memory address. We use
 * nullptr as a sentinel value to indicate that a pointer has not
 * be assigned address of valid memory. Any attempt to read or write
 * the value at the nullptr (zero) address is an illegal operation.
 * Uncomment the test case below and see how that illegal
 * access is reported on your system.
 */
// PROVIDED_TEST("Test case that writes to nullptr address") {
//     DataPoint* taskList = nullptr; // taskList assigned zero address
//     taskList[0].label = "essay";   // this statement attemps to write to zero address
// }


/* A C++ array does not know its length, nor is there bounds-checking
 * when accessing by index. What then is the effect of attempt to
 * read/write an array element that is it out of bounds?
 * Uncomment the test case below and run to observe what happens
 * on your system.
 */
//PROVIDED_TEST("Test case that access indexes beyond array bounds") {
//    DataPoint* taskList = new DataPoint[5](); // ctor () init to 0/empty
//
//    EXPECT_EQUAL(taskList[0].label, ""); // in bounds, initialized, ok
//    taskList[974].label += "banana"; // out of bounds, bad, but what _does_ happen?
//    EXPECT_EQUAL(taskList[974].label, "banana"); // did our bad statement actually succeed?
//
//    delete[] taskList;
//}


/* The effect of calling delete twice on the same memory location has
 * unpredictable behavior. On some systems, the redundant call may be ignored.
 * On other systems, trying to delete already deleted memory can crash the program.
 * Uncomment the test case below to try on your system and see what happens.
 */
//PROVIDED_TEST("Test case that deletes same memory address twice") {
//    DataPoint* taskList = new DataPoint[3];
//    DataPoint* ptr = taskList; // ptr holds same memory address as taskList
//    delete[] taskList;         // delete memory once
//    delete[] ptr;              // executing this line deletes same memory a second time!
//}


/* When deleting a pointer, the memory location is marked as available to
 * be recycled. Typically the memory contents are left as-is, and if you
 * access the deleted memory, it can actually look "ok", at least for a while
 * Uncomment the test case below to see what happens on your system.
 */
//PROVIDED_TEST("Test case that accesses memory after it was deleted") {
//    DataPoint* taskList = new DataPoint[4];
//    taskList[0].label = "sleep";
//    delete[] taskList;
//        /* taskList points to memory address that is now deleted, so
//           executing the line below accesses deleted memory */
//    EXPECT_EQUAL(taskList[0].label, "sleep"); // should label still be sleep? why or why not?
//}

