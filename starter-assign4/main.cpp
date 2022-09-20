#include <iostream>
#include "console.h"
#include "pqclient.h"
#include "pqarray.h"
#include "pqheap.h"
#include "testing/SimpleTest.h"
using namespace std;

// You are free to edit the main in any way that works
// for your testing/debugging purposes.
// We will supply our own main() during grading

int main()
{
    if (runSimpleTests(SELECTED_TESTS)) {
        return 0;
    }
    cout << "All done, exiting" << endl;
    return 0;
}


// Do not remove or edit below this line. It is here to confirm that your code
// conforms to the expected function prototypes needed for grading
void confirmFunctionPrototypes() {
    {
        PQArray pq;
        pq.enqueue({ "", 1 });
        DataPoint n = pq.peek();
        n = pq.dequeue();
        int size = pq.size();
        pq.isEmpty();
        pq.clear();
    }
    Vector<DataPoint> v;
    pqSort(v);
    v = topK(cin, 1);
    {
        PQHeap pq;
        pq.enqueue({ "", 1 });
        DataPoint n = pq.peek();
        n = pq.dequeue();
        int size = pq.size();
        pq.isEmpty();
        pq.clear();
    }
}
